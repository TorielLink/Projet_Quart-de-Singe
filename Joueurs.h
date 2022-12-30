#ifndef PROJETQUARTDESINGE_JOUEURS_H
#define PROJETQUARTDESINGE_JOUEURS_H


/**
 * @file Joueurs.h
 * SAÉ S1.02 : Le quart de singe
 * @authors PROUX Clothilde, TASSIN Nicolas
 * @version 1 - 30/12/2022
 * @brief Composant de joueurs d'une partie du jeu
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
 * @brief Teste si le joueur à qui on a demandé le mot auquel il pensait gagne la manche.
 * @param joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param lettresJouees -> la liste des lettres déjà jouées par les joueurs précédents
 * @return
 */
bool humainPrecedentGagne(const Joueurs &joueursManche, const char *lettresJouees);


/**
 * @brief Affiche le mot auquel "pensait" le robot
 * @param joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param lettresJouees -> la liste des lettres déjà jouées
 */
void reponseRobotPrecedent(const Joueurs &joueursManche, const char *lettresJouees);


/**
 * @brief Joue une manche.
 * @param joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 */
void manche(Joueurs &joueursManche);


/**
 * @brief Joue une partie.
 * @param joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 */
void partie(Joueurs &joueursManche);


#endif //PROJETQUARTDESINGE_JOUEURS_H
