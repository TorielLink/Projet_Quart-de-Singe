#ifndef PROJETQUARTDESINGE_PARTIE_H
#define PROJETQUARTDESINGE_PARTIE_H


/**
 * @file Partie.h
 * SAÉ S1.02 : Le quart de singe
 * @authors PROUX Clothilde
 * @version 1 - 30/12/2022
 * @brief Jouer une partie de Quart de Singe.
 * Comparaison d'approches algorithmiques -- BUT Paris - Rives de Seine
*/


#include <iostream>
#include <fstream> // pour ifstream
#include <iomanip> // pour setw
#include <cstdlib> // pour rand

using namespace std;


/**
 * @brief Ouvre le dictionnaire.
 * @return dico -> le-dit dictionnaire
 */
ifstream ouvertureDico();


/**
 * @brief Teste si le mot entré appartient au dictionnaire.
 * @param[in] motCherche -> mot entré par le joueur
 * @return 0 -> le mot n'appartient pas ; 1 -> il appartient ; 2 -> le dictionnaire ne s'est pas ouvert
 */
bool appartientDico(const char motCherche[]);


/**
 * @brief Donne le prochain coup d'un joueur robot.
 * @param[in] lettresJouees -> la liste des lettres données par les joueurs précédents
 * @return coupJoue -> le caractère joué par le robot
 */
char jeuRobot(const char *lettresJouees);


#endif //PROJETQUARTDESINGE_PARTIE_H
