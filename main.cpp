#include <iostream>
#include <fstream> // pour ifstream
#include <iomanip> // pour setw

using namespace std;

/**
 * @brief Donne la nature de chaque joueur.
 * @param[in] nb_joueurs -> le nombre de joueur dans la partie
 * @param[in] liste_joueurs -> la liste contenant les joueurs : leur ordre et leur nature
 * @param[in] joueurs -> chaque joueurs indépendamment
 */
void determineJoueurs(unsigned int *nb_joueurs, const char *liste_joueurs, char joueurs[]) {
    *nb_joueurs = strlen(liste_joueurs);
 //   char joueurs[nb_joueurs];
    for (int i = 0; i < *nb_joueurs; i++) {
        joueurs[i] = liste_joueurs[i];
    }
}


/**
 * @brief Teste si le mot entré appartient au dictionnaire.
 * @param motCherche -> mot entré par le joueur
 * @return 0 -> le mot n'appartient pas ; 1 -> il appartient ; 2 -> le dictionnaire ne s'est pas ouvert
 */
int appartientDico(const char motCherche[]) {
    ifstream dico;
    dico.open("./ods4.txt", ios::in | ios::binary);
    if (!dico) {
        cerr << "Le dictionnaire n'a pu etre ouvert" << endl;
        return 2;
    }
    const int MAX = 26;
    char motLu[MAX];
    dico >> setw(MAX) >> motLu;
    while (dico) {
        if (strcmp(motCherche, motLu) == 0) {
            cout << "il appartient" << endl;
            return 1;
        }
        dico >> setw(MAX) >> motLu;
    }
    dico.close();
    return 0;
}


/**
 * @brief Joue une manche.
 * @param nbJoueurs -> le nombre de joueurs de la manche
 * @param joueurs -> la liste des joueurs contenant leur nature (et, par leur indice, leur ordre)
 * @return scoresJoueurs -> la liste contenant les scores des joueurs
 */
unsigned int manche(const unsigned int nbJoueurs, const char *joueurs, float scoresJoueurs[], unsigned int joueurActuel) {
    char coupJoue;
    char lettresJouees[26] = "";
    char motDonne[26];
    bool estPremierJoueur = true;
    unsigned int joueurPrecedent;
    unsigned int joueurPerdant;
    bool mancheFinie = false;

    while (!mancheFinie) {
        if (joueurActuel > nbJoueurs) joueurActuel = 1;
        cout << joueurActuel << joueurs[joueurActuel - 1] << ", ("<< lettresJouees << ") > ";
        if (joueurs[joueurActuel - 1] == 'H') { // Joueur humain
            cin >> coupJoue;
            if (coupJoue == '!') {
                scoresJoueurs[joueurActuel - 1] += 0.25;
                joueurPerdant = joueurActuel;
                cout << "le joueur " <<  joueurActuel << joueurs[joueurActuel - 1] << " abandonne la manche et prend un quart de singe" << endl;
                mancheFinie = true;
            }
            else if (coupJoue == '?') {
                if (estPremierJoueur) {
                    cout << "coup impossible au premier tour. veuillez rejouer." << endl;
                    joueurActuel--;
                }
                else {
                    joueurPrecedent = joueurActuel - 1;
                    if (joueurPrecedent == 0) joueurPrecedent = nbJoueurs;
                    cout << joueurPrecedent << joueurs[joueurPrecedent - 1] << ", saisir le mot > ";
                    cin >> motDonne;
                    for (int i = 0; i < strlen(motDonne); i++) {
                        motDonne[i] = (char) toupper(motDonne[i]);
                    }
                    if (strncmp(motDonne, lettresJouees, strlen(lettresJouees)) != 0) {
                        cout << "le mot " << motDonne << " ne commence pas par les lettres attendues, le joueur "
                            << joueurPrecedent << joueurs[joueurPrecedent - 1] << " prend un quart de singe" << endl;
                        scoresJoueurs[joueurPrecedent - 1] += 0.25;
                        joueurPerdant = joueurPrecedent;
                        mancheFinie = true;
                    }
                    else {
                        if (appartientDico(motDonne) == 1) {
                            cout << "le mot " << motDonne << " existe, le joueur " << joueurActuel << joueurs[joueurActuel-1]
                            << " prend un quart de singe" << endl;
                            scoresJoueurs[joueurActuel - 1] += 0.25;
                            joueurPerdant = joueurActuel;
                            mancheFinie = true;

                        }
                        else {
                            cout << "le mot " << motDonne << " n’existe pas, le joueur " << joueurPrecedent << joueurs[joueurPrecedent-1]
                                 << " prend un quart de singe" << endl;
                            scoresJoueurs[joueurPrecedent - 1] += 0.25;
                            joueurPerdant = joueurPrecedent;
                            mancheFinie = true;

                        }
                    }
                }
            }
            else {
                lettresJouees[strlen(lettresJouees)] = (char) toupper(coupJoue);
            }
        }
        else {
            cout << "Joueur Robot : " <<  joueurActuel << joueurs[joueurActuel - 1] << endl;
        }
        joueurActuel++;
        if (estPremierJoueur && joueurActuel > 1) estPremierJoueur = false;
    }
    return joueurPerdant;
}


void partie(const unsigned int nbJoueurs, const char *joueurs) {
    float scoresJoueurs[nbJoueurs];
    for (int j = 0; j < nbJoueurs; ++j) scoresJoueurs[j] = 0.;
    int joueurPerdant;
    unsigned int premierJoueur = 1;

    do {
        joueurPerdant = manche(nbJoueurs, joueurs, scoresJoueurs, premierJoueur);
        premierJoueur = joueurPerdant;
        for (int i = 1; i <= nbJoueurs; ++i) {
            cout << i << joueurs[i - 1] << " : " << scoresJoueurs[i - 1];
            if (i != nbJoueurs)
                cout << "; ";
            else
                cout << endl;
        }
    } while (scoresJoueurs[joueurPerdant - 1] != 1.);
    cout << "La partie est finie" << endl;
}


/**
 * @brief Programme principal.
 * @param[in] argc -> nombre de paramètres sur la ligne de commande (+ 1)
 * @param[in] argv -> paramètres de la ligne de commande (argv[0] : la cde, argv[1] : le 1er paramètre, etc)
 * @return 0 -> le programme s'est exécuté sans problème
 */
//int main(int argc, const char* argv[]) {
int main() {
    int argc;
    char argv[26];
    unsigned int nbJoueurs = 0;

    cin >> argc;
    //cin >> argv[1] >> argv[2];
    cin >> argv;
    // paramètre sur la ligne de commande
    if (argc >= 2) {
        // determineJoueurs(&nbJoueurs,argv,joueurs);
        nbJoueurs = strlen(argv);
        char joueurs[nbJoueurs];
        strcpy(joueurs, argv);
        //appartientDico("CHEVAL");
        partie(nbJoueurs, joueurs);
    }
    else
        cout << "Il n'y a pas de parametre" << endl;

    return 0;
}