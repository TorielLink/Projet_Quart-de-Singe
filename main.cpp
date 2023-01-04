#include <iostream>
#include <fstream> // pour ifstream
#include <iomanip> // pour setw
#include <cstdlib> // pour rand
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
 * @param[in] motCherche -> mot entré par le joueur
 * @return 0 -> le mot n'appartient pas ; 1 -> il appartient ; 2 -> le dictionnaire ne s'est pas ouvert
 */
bool appartientDico(const char motCherche[]) {
    ifstream dico = ouvertureDico();
    bool trouve = false;
    char motLu[MAX_MOT];

    dico >> setw(MAX_MOT) >> motLu;
    while (dico) {
        if (strcmp(motCherche, motLu) == 0) { // Le mot cherché appartient au dictionnaire
            trouve = true;
        }
        else if (strcmp(motCherche, motLu) < 0) { // Le mot lu est classé dans l'ordre alphabétique après le mot cherché
            break; // Le mot n'appartient pas au dictionnaire. On sort de la boucle
        }
        dico >> setw(MAX_MOT) >> motLu;
    }
    dico.close();
    return trouve;
}


/**
 * @brief Donne le prochain coup d'un joueur robot.
 * @param[in] lettresJouees -> la liste des lettres données par les joueurs précédents
 * @return coupJoue -> le caractère joué par le robot
 */
char jeuRobot(const char lettresJouees[]) {
    ifstream dico = ouvertureDico();
    char coupJoue;
    char motLu[MAX_MOT];
    char motTrouve[MAX_MOT] = "";
    char debutMotInterdit[MAX_MOT] = "";

    dico >> setw(MAX_MOT) >> motLu;
    if (strlen(lettresJouees) == 0) { // Le robot joue en premier
        coupJoue = (char) (rand() % 26 + 65); // Sort une lettre aléatoire
    }
    else {
        while (dico) {
            if (strncmp(lettresJouees, motLu, strlen(lettresJouees)) == 0
            // Si les premières lettres de mot lu correspondent aux lettres jouées
            && (strncmp(motLu, debutMotInterdit, strlen(debutMotInterdit)) != 0 || strlen(debutMotInterdit) == 0)) {
                if (strlen(motLu) == strlen(lettresJouees) + 1) {
                    // Pour ne pas donner une lettre qui terminerait un mot
                    strcpy(debutMotInterdit, motLu);
                }
                else if (strlen(motLu) > strlen(lettresJouees) + 1
                && (strlen(motTrouve) == 0 || strlen(motTrouve) > strlen(motLu))) {
                    // Pour trouver le mot le plus court
                    strcpy(motTrouve,motLu);
                }
            }
            else if (strncmp(lettresJouees, motLu, strlen(lettresJouees)) < 0) {
                // Le mot lu est classé dans l'ordre alphabétique après les lettres jouées
                break;
            }
            dico >> setw(MAX_MOT) >> motLu;
        }

        if (strlen(motTrouve) == 0) // Aucun mot n'a été trouvé
            coupJoue = '?';
        else
            coupJoue = motTrouve[strlen(lettresJouees)];
        dico.close();
    }

    return coupJoue;
}


/**
 * @brief Renseigne la nature du joueur actuel.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres données par les joueurs précédents
 * @return coupJoue -> le coup joué par le joueur actuel
 */
char jeuJoueurActuel(Joueurs &joueursManche, const char lettresJouees[]) {
    char coupJoue;

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

    return coupJoue;
}


/**
 * @brief Le joueur abandonne la manche.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 */
void abandonManche(Joueurs &joueursManche) {
    joueursManche.scoresJoueurs[joueursManche.joueurActuel - 1] += 0.25;
    joueursManche.joueurPerdant = joueursManche.joueurActuel;
    cout << "le joueur " <<  joueursManche.joueurActuel
         << (char) toupper(joueursManche.joueurs[joueursManche.joueurActuel - 1])
         << " abandonne la manche et prend un quart de singe" << endl;
}


/**
 * @brief Le joueur demande au précédent le mot auquel il pensait.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres déjà jouées par les joueurs précédents
 */
void reponseJoueurPrecedent(Joueurs &joueursManche, const char *lettresJouees) {
    joueursManche.joueurPrecedent = joueursManche.joueurActuel - 1;
    if (joueursManche.joueurPrecedent == 0) joueursManche.joueurPrecedent = joueursManche.nbJoueurs;
    cout << joueursManche.joueurPrecedent
         << (char) toupper(joueursManche.joueurs[joueursManche.joueurPrecedent - 1]) << ", saisir le mot > ";

    if (toupper(joueursManche.joueurs[joueursManche.joueurPrecedent - 1]) == 'H')
         reponseHumainPrecedent(joueursManche, lettresJouees);
    else
         reponseRobotPrecedent(joueursManche, lettresJouees);
    joueursManche.scoresJoueurs[joueursManche.joueurPerdant - 1] += 0.25;
}


/**
 * @brief Teste si le joueur à qui on a demandé le mot auquel il pensait gagne la manche.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres déjà jouées par les joueurs précédents
 */
void reponseHumainPrecedent(Joueurs &joueursManche, const char *lettresJouees) {
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
        if (appartientDico(motSaisi)) { // Le mot appartient au dictionnaire
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
    if (gagne)
        joueursManche.joueurPerdant = joueursManche.joueurActuel;
    else
        joueursManche.joueurPerdant = joueursManche.joueurPrecedent;
}


/**
 * @brief Affiche le mot auquel "pensait" le robot
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres déjà jouées
 */
void reponseRobotPrecedent(Joueurs &joueursManche, const char *lettresJouees) {
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
    joueursManche.joueurPerdant = joueursManche.joueurActuel;
}


/**
 * @brief Le joueur joue une lettre. On vérifie si elle appartient au dico.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres déjà jouées
 * @param[in] coupJoue -> le coup joué par le joueur actuel
 */
bool lettreJoueeHumain(Joueurs &joueursManche, char *lettresJouees, const char coupJoue) {
    bool lettreBonne = true;

   if (toupper(coupJoue) >= 65 && toupper(coupJoue) <= 90) { //la lettre est non-accentuée
          lettresJouees[strlen(lettresJouees)] = (char) toupper(coupJoue);
          if (strlen(lettresJouees) > 2) { // Mot testé à partir de trois lettres
              if (appartientDico(lettresJouees)) {
                cout << "le mot " << lettresJouees << " existe, le joueur " << joueursManche.joueurActuel
                     << (char) toupper(joueursManche.joueurs[joueursManche.joueurActuel-1])
                     << " prend un quart de singe" << endl;
                joueursManche.scoresJoueurs[joueursManche.joueurActuel - 1] += 0.25;
                joueursManche.joueurPerdant = joueursManche.joueurActuel;
                lettreBonne = false;
              }
          }
   }
   else {
      cout << "veuillez entrer une lettre non-accentuée" << endl;
           joueursManche.joueurActuel--;
   }

 return lettreBonne;
}


/**
 * @brief Vérifie si la lettre entrée est correcte.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres déjà jouées
 * @param[in] coupJoue -> le coup joué par le joueur actuel
 * @return lettreBonne -> le booléen qui vérifie si la lettre est bonne
 */
bool lettreJoueeJoueurActuel(Joueurs &joueursManche, char *lettresJouees, const char coupJoue) {
    bool lettreBonne = true;

    if (toupper(joueursManche.joueurs[joueursManche.joueurActuel - 1]) == 'H')  // Par un humain
        lettreBonne = lettreJoueeHumain(joueursManche, lettresJouees, coupJoue);
    else { // Par un robot
        lettresJouees[strlen(lettresJouees)] = coupJoue;
    }
    return lettreBonne;
}


/**
 * @brief Joue une manche.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 */
void manche(Joueurs &joueursManche) {
    char coupJoue;
    char lettresJouees[MAX_MOT] = "";
    bool mancheFinie = false;

    while (!mancheFinie) {
            coupJoue = jeuJoueurActuel(joueursManche, lettresJouees);
        // Coups possibles
        if (coupJoue == '!') { // Abandon de la manche
            abandonManche(joueursManche);
            mancheFinie = true;
        }
        else if (coupJoue == '?') { // Demande le mot du joueur précédent
            reponseJoueurPrecedent(joueursManche, lettresJouees);
            mancheFinie = true;
        }
        else { // Lettre jouée → Ajout d'une lettre à celles déjà jouées		
           if (!lettreJoueeJoueurActuel(joueursManche, lettresJouees, coupJoue))
               mancheFinie = true;
        }
        joueursManche.joueurActuel++;
    }
}


/**
 * @brief Joue une partie.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
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
        if (strlen(argv[1]) > 1) {
            joueursManche.nbJoueurs = (int) strlen(argv[1]);
            joueursManche.joueurs = new char[joueursManche.nbJoueurs];
            strcpy(joueursManche.joueurs, argv[1]);
            partie(joueursManche);
        }
        else
            cout << "Il faut au minimum deux joueurs" << endl;
    }
    else
        cout << "Il n'y a pas de parametre" << endl;

    return 0;
}