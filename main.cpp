#include <iostream>
#include <fstream> // pour ifstream
#include <iomanip> // pour setw

using namespace std;

void determineJoueurs(unsigned int nb_joueurs, const char *liste_joueurs, char joueurs[]) {
    nb_joueurs = strlen(liste_joueurs);
 //   char joueurs[nb_joueurs];
    for (int i = 0; i < nb_joueurs; i++) {
        joueurs[i] = liste_joueurs[i];
        cout << joueurs[i] << endl;
    }
}

int appartientDico(const char motCherche[]) {
    ifstream dico("ods4.txt");
    if (!dico) {
        cout << "Le dictionnaire n'a pu etre ouvert" << endl;
        return 2;
    }
    const int MAX = 26;
    char motLu[MAX];
    dico >> setw(MAX) >> motLu;
    while (dico) {
        if (strcmp(motCherche, motLu) == 0) {
            cout << "il appartient mdr" << endl;
            return 1;
        }
        dico >> setw(MAX) >> motLu;
    }
    dico.close();
    return 0;
}

void manche(int nb_joueurs, const char *joueurs) {
    char coupJoue;
    char lettresJouees[26];
    char motDonne[26];
    int joueurActuel = 1;
    int joueurPrecedent;
    float scoreJoueur[nb_joueurs];
    bool mancheFinie = false;
    while (!mancheFinie) {
        cin >> coupJoue;
        if (coupJoue == '!') {
            scoreJoueur[joueurActuel - 1] += 0.25;
            cout << "le joueur " <<  joueurActuel << joueurs[joueurActuel - 1] << " abandonne la manche et prend un quart de singe" << endl;
            mancheFinie = true;
        }
        else if (coupJoue == '?') {
            joueurPrecedent = joueurActuel - 1;
            cin >> motDonne;
            if (strncmp(motDonne, lettresJouees, strlen(lettresJouees)) != 0) {
                cout << "le mot " << motDonne << " ne commence pas par les lettres attendues, le joueur "
                    << joueurPrecedent << joueurs[joueurPrecedent - 1] << " prend un quart de singe" << endl;
                scoreJoueur[joueurPrecedent - 1] += 0.25;
                mancheFinie = true;
            }
            else {
                if (appartientDico(motDonne) == 1) {
                    cout << "le mot " << motDonne << " existe, le joueur " << joueurActuel << joueurs[joueurActuel-1]
                    << " prend un quart de singe" << endl;
                }
                else {
                    cout << "le mot " << motDonne << " n’existe pas, le joueur " << joueurPrecedent << joueurs[joueurPrecedent-1]
                         << " prend un quart de singe" << endl;

                }
            }
        }
        else {
            lettresJouees[strlen(lettresJouees)] = coupJoue;
            cout << lettresJouees << endl; // Erreur dans l'entrée des lettres
        }
        joueurActuel++;
    }
}
/*
   argc -> nombre de paramètres sur la ligne de commande (+ 1)
   argv -> paramètres de la ligne de commande
           (argv[0] : la cde, argv[1] : le 1er paramètre, etc)
*/
//int main(int argc, const char* argv[]) {
int main() {
    int argc;
    char argv[26];
    int nbjoueurs = 0;
    char joueurs[10];

    cin >> argc;
    //cin >> argv[1] >> argv[2];
    cin >> argv;
    // paramètre sur la ligne de commande
    if (argc >= 2) {
        determineJoueurs(nbjoueurs,argv,joueurs);
        }

    else
        cout << "Il n'y a pas de parametre" << endl;

    appartientDico("CHEVAL");
    manche(nbjoueurs, joueurs);

    return 0;
}