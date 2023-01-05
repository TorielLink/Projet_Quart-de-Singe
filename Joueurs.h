#ifndef PROJETQUARTDESINGE_JOUEURS_H
#define PROJETQUARTDESINGE_JOUEURS_H


/**
 * @file Joueurs.h
 * SAÉ S1.02 : Le quart de singe
 * @authors PROUX Clothilde, TASSIN Nicolas
 * @version 1 - 30/12/2022
 * @brief Toutes les fonctions des joueurs.
 * Comparaison d'approches algorithmiques -- BUT Paris - Rives de Seine
*/


/**
 * @brief Structure de données de type Joueurs
 */
struct Joueurs {
    int nbJoueurs;
    char *joueurs;
    unsigned int joueurActuel;
    unsigned int joueurPrecedent;
    unsigned int joueurPerdant;
    float *scoresJoueurs;
};


/**
 * @brief Le joueur abandonne la manche.
 * @param joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 */
void abandonManche(Joueurs &joueursManche);


/**
 * @brief Le joueur abandonne la manche.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 */
void reponseJoueurPrecedent(Joueurs &joueursManche, const char *lettresJouees);


/**
 * @brief Vérifie si la lettre jouée par le joueur appartient au dictionnaire.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres déjà jouées
 * @param[in] coupJoue -> le coup joué par le joueur actuel
 */
bool lettreJoueeHumain(Joueurs &joueursManche, char *lettresJouees, char coupJoue);


/**
 * @brief Renseigne la nature du joueur actuel.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres données par les joueurs précédents
 * @return coupJoue -> le coup joué par le joueur actuel
 */
char jeuJoueurActuel(Joueurs &joueursManche, const char *lettresJouees);


/**
 * @brief Le joueur à qui on a demandé le mot auquel il pensait gagne ou non la manche.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres déjà jouées par les joueurs précédents
 */
void reponseHumainPrecedent(Joueurs &joueursManche, const char *lettresJouees);


/**
 * @brief Affiche le mot auquel "pensait" le robot
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres déjà jouées
 */
void reponseRobotPrecedent(Joueurs &joueursManche, const char *lettresJouees);


/**
 * @brief Vérifie si la lettre entrée est correcte.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres déjà jouées
 * @param[in] coupJoue -> le coup joué par le joueur actuel
 * @return lettreBonne -> le booléen qui vérifie si la lettre est bonne
 */
bool lettreJoueeJoueurActuel(Joueurs &joueursManche, char *lettresJouees, char coupJoue);


/**
 * @brief Joue une manche.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 */
void manche(Joueurs &joueursManche);


/**
 * @brief Joue une partie entière.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 */
void partie(Joueurs &joueursManche);


#endif //PROJETQUARTDESINGE_JOUEURS_H
