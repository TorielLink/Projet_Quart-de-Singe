#include <iostream>
#include <fstream> // pour ifstream
#include <iomanip> // pour setw
#include "Joueurs.h" // Structure joueurs

using namespace std;
const int MAX_MOT = 26;


/**
 * @brief Ouvre le dictionnaire.
 * @return dico -> le-dit dictionnaire
 */
ifstream ouvertureDico() {
    ifstream dico("ods4.txt",ios::in | ios::binary);

    if (!dico) {
        cerr << "Le dictionnaire n'a pu etre ouvert" << endl;
        exit(2);
    }

    return dico;
}


/**
 * @brief Teste si le mot entré appartient au dictionnaire.
 * @param motCherche -> mot entré par le joueur
 * @return 0 -> le mot n'appartient pas ; 1 -> il appartient ; 2 -> le dictionnaire ne s'est pas ouvert
 */
int appartientDico(const char motCherche[]) {
    ifstream dico = ouvertureDico();
    char motLu[MAX_MOT];

    dico >> setw(MAX_MOT) >> motLu;
    while (dico) {
        if (strcmp(motCherche, motLu) == 0) { // Le mot cherché appartient au dictionnaire
            return 1;
        }
        dico >> setw(MAX_MOT) >> motLu;
    }
    dico.close();
    return 0;
}


/**
 * @brief Donne le prochain coup d'un joueur robot.
 * @param lettresJouees -> la liste des lettres données par les joueurs précédents
 * @return coupJoue -> le caractère joué par le robot
 */
char jeuRobot(const char lettresJouees[]) {
    ifstream dico = ouvertureDico();
    char coupJoue;
    char motLu[MAX_MOT];
    char motTrouve[MAX_MOT] = "";
    char debutMotInterdit[MAX_MOT] = "";

    dico >> setw(MAX_MOT) >> motLu;
    if (strlen(lettresJouees) == 0) { // Le robot doit jouer en premier
        coupJoue = motLu[0];
    }
    else {
        while (dico) {
            if (strncmp(lettresJouees, motLu, strlen(lettresJouees)) == 0
            && (strncmp(motLu, debutMotInterdit, strlen(debutMotInterdit)) != 0 || strlen(debutMotInterdit) == 0)) {
                if (strlen(motLu) == strlen(lettresJouees) + 1) {
                    strcpy(debutMotInterdit, motLu);
                }
                else if (strlen(motLu) > strlen(lettresJouees) + 1
                && (strlen(motTrouve) == 0 || strlen(motTrouve) > strlen(motLu))) {
                    strcpy(motTrouve,motLu);
                }
            }
            dico >> setw(MAX_MOT) >> motLu;
        }

        if (strlen(motTrouve) == 0)
            coupJoue = '?';
        else
            coupJoue = motTrouve[strlen(lettresJouees)];
        dico.close();
    }

    return coupJoue;
}


/**
 * @brief Teste si le joueur à qui on a demandé le mot auquel il pensait gagne la manche.
 * @param joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param lettresJouees -> la liste des lettres déjà jouées par les joueurs précédents
 * @return
 */
bool humainPrecedentGagne(const Joueurs &joueursManche, const char *lettresJouees) {
    char motSaisi[MAX_MOT];
    bool gagne;

    cin >> setw(MAX_MOT) >> motSaisi;
    cin.ignore(INT_MAX, '\n');
    for (int i = 0; i < strlen(motSaisi); i++) { // Mettre en majuscule le mot entré
        motSaisi[i] = (char) toupper(motSaisi[i]);
    }
    if (strncmp(motSaisi, lettresJouees, strlen(lettresJouees)) != 0) {
        cout << "le mot " << motSaisi << " ne commence pas par les lettres attendues, le joueur "
             << joueursManche.joueurPrecedent << (char) toupper(joueursManche.joueurs[joueursManche.joueurPrecedent - 1])
             << " prend un quart de singe" << endl;
        gagne = false;
    }
    else {
        // Teste si le mot est dans le dictionnaire
        if (appartientDico(motSaisi) == 1) { // Le mot appartient au dictionnaire
            cout << "le mot " << motSaisi << " existe, le joueur " << joueursManche.joueurActuel
                 << (char) toupper(joueursManche.joueurs[joueursManche.joueurActuel-1]) << " prend un quart de singe" << endl;
            gagne = true;
        }
        else { // Le mot n'appartient pas
            cout << "le mot " << motSaisi << " n’existe pas, le joueur " << joueursManche.joueurPrecedent
                 << (char) toupper(joueursManche.joueurs[joueursManche.joueurPrecedent - 1])
                 << " prend un quart de singe" << endl;
            gagne = false;
        }
    }
    return gagne;
}


/**
 * @brief Affiche le mot auquel "pensait" le robot
 * @param joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param lettresJouees -> la liste des lettres déjà jouées
 */
void reponseRobotPrecedent(const Joueurs &joueursManche, const char *lettresJouees) {
    ifstream dico = ouvertureDico();

    char motLu[MAX_MOT];
    char motTrouve[MAX_MOT] = "";
    dico >> setw(MAX_MOT) >> motLu;
    while (dico) {
        if (strncmp(lettresJouees, motLu, strlen(lettresJouees)) == 0) {
            strcpy(motTrouve,motLu);
            cout << motTrouve << endl;
            cout << "le mot " << motTrouve << " existe, le joueur " << joueursManche.joueurActuel
                 << (char) toupper(joueursManche.joueurs[joueursManche.joueurActuel-1])
                 << " prend un quart de singe" << endl;
            break;
        }
        dico >> setw(MAX_MOT) >> motLu;
    }
    dico.close();
}


/**
 * @brief Joue une manche.
 * @param joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 */
void manche(Joueurs &joueursManche) {
    char coupJoue;
    char lettresJouees[MAX_MOT] = "";
    bool mancheFinie = false;

    while (!mancheFinie) {
        if (joueursManche.joueurActuel > joueursManche.nbJoueurs) joueursManche.joueurActuel = 1;
        // le joueur après le dernier est le premier
        cout << joueursManche.joueurActuel << (char) toupper(joueursManche.joueurs[joueursManche.joueurActuel - 1])
            << ", ("<< lettresJouees << ") > ";
        if (toupper(joueursManche.joueurs[joueursManche.joueurActuel - 1]) == 'H') { // Joueur humain
            cin >> coupJoue;
            cin.ignore(INT_MAX, '\n');
        }
        else if (toupper(joueursManche.joueurs[joueursManche.joueurActuel - 1]) == 'R') { // Joueur robot
            coupJoue = jeuRobot(lettresJouees);
            cout << coupJoue << endl;

        }
        else { // Joueur alien
            cerr << "joueur invalide. veuillez réitérer une partie" << endl;
            exit(1);
        }
        // Coups possibles
        if (coupJoue == '!') {
            joueursManche.scoresJoueurs[joueursManche.joueurActuel - 1] += 0.25;
            joueursManche.joueurPerdant = joueursManche.joueurActuel;
            cout << "le joueur " <<  joueursManche.joueurActuel
            << (char) toupper(joueursManche.joueurs[joueursManche.joueurActuel - 1])
            << " abandonne la manche et prend un quart de singe" << endl;
            mancheFinie = true;
        }
        else if (coupJoue == '?') {
            joueursManche.joueurPrecedent = joueursManche.joueurActuel - 1;
            if (joueursManche.joueurPrecedent == 0) joueursManche.joueurPrecedent = joueursManche.nbJoueurs;
            cout << joueursManche.joueurPrecedent
                 << (char) toupper(joueursManche.joueurs[joueursManche.joueurPrecedent - 1])
                 << ", saisir le mot > ";
            if (toupper(joueursManche.joueurs[joueursManche.joueurPrecedent - 1]) == 'H') {
                if (humainPrecedentGagne(joueursManche, lettresJouees))
                    joueursManche.joueurPerdant = joueursManche.joueurActuel;
                else
                    joueursManche.joueurPerdant = joueursManche.joueurPrecedent;
            }
            else {
                reponseRobotPrecedent(joueursManche, lettresJouees);
                joueursManche.joueurPerdant = joueursManche.joueurActuel;
            }
            joueursManche.scoresJoueurs[joueursManche.joueurPerdant - 1] += 0.25;
            mancheFinie = true;
        }
        else { // Lettre jouée
            if (toupper(joueursManche.joueurs[joueursManche.joueurActuel - 1]) == 'H') { // Par un humain
                if (toupper(coupJoue) >= 65 && toupper(coupJoue) <= 90) { //la lettre est non-accentuée
                    lettresJouees[strlen(lettresJouees)] = (char) toupper(coupJoue);
                    if (appartientDico(lettresJouees)) {
                        cout << "le mot " << lettresJouees << " existe, le joueur " << joueursManche.joueurActuel
                             << (char) toupper(joueursManche.joueurs[joueursManche.joueurActuel-1])
                             << " prend un quart de singe" << endl;
                        joueursManche.scoresJoueurs[joueursManche.joueurActuel - 1] += 0.25;
                        joueursManche.joueurPerdant = joueursManche.joueurActuel;
                        mancheFinie = true;
                    }
                }
                else {
                    cout << "veuillez entrer une lettre non-accentuée" << endl;
                    joueursManche.joueurActuel--;
                }
            }
            else { // Par un robot
                lettresJouees[strlen(lettresJouees)] = coupJoue;
            }
        }
        joueursManche.joueurActuel++;
    }
}


/**
 * @brief Joue une partie.
 * @param joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 */
void partie(Joueurs &joueursManche) {
    joueursManche.scoresJoueurs = new float[joueursManche.nbJoueurs];
    joueursManche.joueurActuel = 1;
    for (int j = 0; j < joueursManche.nbJoueurs; ++j) joueursManche.scoresJoueurs[j] = 0.;

    do {
        manche(joueursManche);
        joueursManche.joueurActuel = joueursManche.joueurPerdant;
        for (int i = 1; i <= joueursManche.nbJoueurs; ++i) {
            cout << i << (char) toupper(joueursManche.joueurs[i - 1]) << " : " << joueursManche.scoresJoueurs[i - 1];
            if (i != joueursManche.nbJoueurs)
                cout << "; ";
            else
                cout << endl;
        }
    } while (joueursManche.scoresJoueurs[joueursManche.joueurPerdant - 1] != 1.);
    cout << "La partie est finie" << endl;
}


/**
 * @brief Programme principal.
 * @param[in] argc -> nombre de paramètres sur la ligne de commande (+ 1)
 * @param[in] argv -> paramètres de la ligne de commande (argv[0] : la cde, argv[1] : le 1er paramètre, etc)
 * @return 0 -> le programme s'est exécuté sans problème
 */
int main(int argc, const char* argv[]) {
    Joueurs joueursManche{};

    // paramètre sur la ligne de commande
    if (argc >= 2) {
        joueursManche.nbJoueurs = (int) strlen(argv[1]);
        joueursManche.joueurs = new char[joueursManche.nbJoueurs];
        strcpy(joueursManche.joueurs, argv[1]);
        partie(joueursManche);
    }
    else
        cout << "Il n'y a pas de parametre" << endl;

    return 0;
}