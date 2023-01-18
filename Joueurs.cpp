/**
 * @file Joueurs.cpp
 * SAÉ S1.02 : Le quart de singe
 * @authors PROUX Clothilde
 * @version finale - 05/01/2023
 * @brief Jouer une partie de Quart de Singe.
 * Comparaison d'approches algorithmiques -- BUT Paris - Rives de Seine
*/

#include <iostream>
#include <iomanip> // pour setw
#include <cstdlib> // pour rand
#include "Joueurs.h" // Structure joueurs
#include "Partie.h" // Fonctions de la partie


using namespace std;
const int MAX_MOT = 26;


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
    // Le premier joueur demande au dernier
    cout << joueursManche.joueurPrecedent
         << (char) toupper(joueursManche.joueurs[joueursManche.joueurPrecedent - 1]) << ", saisir le mot > ";

    if (toupper(joueursManche.joueurs[joueursManche.joueurPrecedent - 1]) == 'H') // Jouer Humain
        reponseHumainPrecedent(joueursManche, lettresJouees);
    else // Joueur Robot
        reponseRobotPrecedent(joueursManche, lettresJouees);
    joueursManche.scoresJoueurs[joueursManche.joueurPerdant - 1] += 0.25;
}


/**
 * @brief Vérifie si la lettre jouée par le joueur appartient au dictionnaire.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres déjà jouées
 * @param[in] coupJoue -> le coup joué par le joueur actuel
 */
bool lettreJoueeHumain(Joueurs &joueursManche, char *lettresJouees, const char coupJoue) {
    bool lettreBonne = true;

    if (toupper(coupJoue) >= 65 && toupper(coupJoue) <= 90) { // La lettre est non-accentuée
        if (strlen(lettresJouees) < MAX_MOT - 1)
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
 * @brief Renseigne la nature du joueur actuel.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 * @param[in] lettresJouees -> la liste des lettres données par les joueurs précédents
 * @return coupJoue -> le coup joué par le joueur actuel
 */
char jeuJoueurActuel(Joueurs &joueursManche, const char *lettresJouees) {
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
 * @brief Le joueur à qui on a demandé le mot auquel il pensait gagne ou non la manche.
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
        // Si ça ne commence pas par les lettres déjà jouées
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
            // Recherche un mot qui commence par les lettres jouées
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
 * @brief Joue une partie entière.
 * @param[in,out] joueursManche -> la structure contenant les joueurs, leur nombre et leur score
 */
void partie(Joueurs &joueursManche) {
    joueursManche.scoresJoueurs = new float[joueursManche.nbJoueurs];
    joueursManche.joueurActuel = 1;
    for (int j = 0; j < joueursManche.nbJoueurs; ++j) joueursManche.scoresJoueurs[j] = 0.;
    // Initialise les scores à zéro

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