/**
 * @file main.cpp
 * SAÉ S1.02 : Le quart de singe
 * @authors PROUX Clothilde, TASSIN Nicolas
 * @version finale - 04/01/2023
 * @brief Jouer une partie de Quart de Singe.
 * Comparaison d'approches algorithmiques -- BUT Paris - Rives de Seine
*/

#include <iostream>
#include "Joueurs.h" // Structure joueurs

using namespace std;

/**
 * @brief Programme principal.
 * @param[in] argc -> nombre de paramètres sur la ligne de commande (+ 1)
 * @param[in] argv -> paramètres de la ligne de commande (argv[0] : la cde, argv[1] : le 1er paramètre, etc)
 * @return 0 -> le programme s'est exécuté sans problème
 */
int main(int argc, const char* argv[]) {
    Joueurs joueursManche{};

    // Paramètre sur la ligne de commande
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