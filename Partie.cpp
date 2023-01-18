/**
 * @file main.cpp
 * SAÉ S1.02 : Le quart de singe
 * @authors PROUX Clothilde
 * @version finale - 04/01/2023
 * @brief Jouer une partie de Quart de Singe.
 * Comparaison d'approches algorithmiques -- BUT Paris - Rives de Seine
*/

#include <iostream>
#include <iomanip> // pour setw
#include <cstdlib> // pour rand
#include "Partie.h" // Fonctions de la partie

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
char jeuRobot(const char *lettresJouees) {
    ifstream dico = ouvertureDico();
    char coupJoue;
    char motLu[MAX_MOT];
    char motTrouve[MAX_MOT] = "";
    char debutMotInterdit[MAX_MOT] = "";

    dico >> setw(MAX_MOT) >> motLu;
    if (strlen(lettresJouees) == 0) { // Le robot joue en premier
        coupJoue = (char) (rand() % 26 + 65); // Il sort une lettre aléatoire
    }
    else {
        while (dico) {
            if (strncmp(lettresJouees, motLu, strlen(lettresJouees)) == 0
                // Si les premières lettres de mot lu correspondent aux lettres jouées
                && (strncmp(motLu, debutMotInterdit, strlen(debutMotInterdit)) != 0 || strlen(debutMotInterdit) == 0)) {
                // Et que ça ne commence pas par les lettres interdites ou qu'il n'y en a pas
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