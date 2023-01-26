/**
* @file Board.h
* @author Alexis Bernard & William Dubois
* @brief Fichier qui permet de générer la tableau de jeu du puissance 4.
* @date 31.12.2022
* @version 1.0
*/
#ifndef BOARD_H
#define BOARD_H

#include "../macros.h"
#include <stdbool.h>

/**
 * @brief Structure représentant un plateau de jeu du puissance 4
 * 
 * @param tour 1 ou 2, représente le joueur 1 ou 2, 1 si c'est au tour du Joueur 1 de jouer, 2 sinon
 * @param jetons matrice des jetons, case à 0 si vide, et à 1 ou 2 si elle possède un jeton, représentant les joueurs 1 et 2 respectivement
 * @param dernierJeton stoque la case contenant le dernier jeton joué, utile pour vérifier une victoire
 */
typedef struct board {
    short int jetons[6][7];  
    unsigned short int tour;
    short int dernierJeton[2];
} Board;

/**
 * @brief malloc un Board, initialise sa matrice statique de jetons à 0, initialise le dernierJeton en -1, -1

 * @return Board* si success, NULL si problème de malloc (changement de l'erreur dans ce cas)
 */
Board* creerBoard ();

/**
 * @brief free param b s'il existe
 * @param b tableau que l'on check
 */
void freeBoard (Board *b);

/**
 * @brief ajoute un jeton de valeur param b->tour le plus bas possible dans la matrice de jetons de param b en colonne param col
 * 
 * @param b tableau que l'on modifie
 * @param col la colonne ou l'on ajoute un jeton
 * @return true si le jeton a pu être ajouté
 * @return false si la colonne param col est pleine
 * @return false si param col est invalide, et change l'erreur
 * @return false si param b est NULL, et change  l'erreur
 */
bool jouerJeton (Board **b, int col);

/**
 * @brief affichage textuel de la matrice de jetons de param b sur la console. Si param b est NULL, n'affiche rien et change l'erreur
* @param b l'instance Board que l'on check
 */
void afficherBoard (Board *b);

/**
 * @brief échange le tour de param b (1 vers 2 ou 2 vers 1)
 * @param b tableau que l'on modifie
 */
void swapTour (Board **b);

/**
 * @brief regarde si une victoire a été réalisée selon le dernier pion joué (enregistré dans param b); regarde lignes et colonne pui appelle checkVictoireDiagonales
 * 
 * @param b l'instance Board que l'on check
 * @return true 
 * @return false 
 */
bool checkVictoire (Board *b);

/**
 * @brief comme checkVictoire mais pour les diagonales (séparés pour clarté)
 * 
 * @param b l'instance Board que l'on check
 * @return true 
 * @return false 
 */
bool checkVictoireDiagonales (Board *b);

/**
 * @brief regarde si le board param b est complet
 * 
 * @param b l'instance Board que l'on check
 * @return true si oui
 * @return false sinon
 */
bool boardComplet (Board *b);

/**
 * @brief regarde si il y a une victoire ou si le board est complet
 * 
 * @param b l'instance Board que l'on check
 * @return true 
 * @return false 
 */
bool finJeu (Board *b);

/**
 * @brief fonction de debug, permet de rentrer le tableau de jetons voulu dans un board
 * 
 * @param b l'instance Board que l'on check
 * @param tab le tableau de jeton
 */
void DEBUG_jetonsManuel (Board **b, int tab[6][7]);

/**
 * @brief Retourne une copie du board rentré en paramètre (nouveau pointeur)
 * 
 * @param b Board à copier
 * @return Board* 
 */
Board* copyBoard (Board *b);

/**
 * @brief Retire le dernier jeton ajouté dans le Board grâce au coordonnées stockées dans b->dernierJeton.
 * @warning Possible une seule fois ! Appeler cette fonction un 2ème fois ne marchera pas.
 * 
 * @param b 
 */
void annuleCoup (Board **b);

#endif