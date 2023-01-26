/**
 * @file IA.h
 * @author William Dubois & Alexis Bernard
 * @brief Contient les fonctions pour jouer selon l'IA choisie ainsi que la panoplie de fonctions en rapport avec les coup gagnants.
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef IA_H
#define IA_H

#include "../macros.h"
#include "Puissance4.h"
#include <stdbool.h>
#include "InfoIA.h"

/**
 * @brief Fonction jouer pour l'ia facile.
 * @brief Joue aléatoirement.
 * @param jeu L'instance de Jeu. 
 */
void jouer_FACILE (Jeu* jeu);

/**
 * @brief Fonction jouer pour l'ia normale.
 * @brief Si l'ia peut gagner, elle choisit de gagner, sinon, si elle peut perdre, bloque la victoire adverse, sinon joue aléatoirement.
 * @param jeu L'instance de Jeu.
 */
void jouer_NORMAL (Jeu* jeu);

/**
 * @brief Fonction jouer pour l'ia difficile.
 * 
 * @param jeu L'instance de Jeu.
 */
void jouer_DIFFICILE (Jeu* jeu);

/**
 * @brief Fonction jouer pour l'utilisateur.
 * 
 * @param jeu L'instance de Jeu.
 */
void jouer_USER (Jeu* jeu);

/**
 * @brief Vérifie si il y a un coup gagnant pour l'un des joueurs.
 * 
 * @warning Ne retient que 1 case gagnante par joueur, si une autre est trouvée, écrase la première dans le tableau.
 *
 * @return Tableau de int de taille 4 contenant 2 fois des coordonnées (x, y) (indices 0 et 1 pour joueur 1, 2 et 3 pour joueur 2),
 * correspondant à la case où le joueur 1 (respectivement 2) doit jouer pour gagner (-1, -1 si aucun coup gagnant) ().
 */
int* coupGagnant (Board* b);

/**
 * @brief vérifie si il y a un coup gagnant pour chaque colonne.
 * 
 * @param b L'instance Board que l'on check.
 * @param gagne Tableau des coordonnées gagnantes.
 */
void coupGagnantColonne (Board* b, int** gagne);

/**
 * @brief Vérifie si il y a un coup gagnant pour chaque ligne.
 * @param b L'instance Board que l'on check.
 * @param gagne Tableau des coordonnées gagnantes.
 */
void coupGagnantLigne (Board* b, int** gagne);

/**
 * @brief Vérifie si il y a un coup gagnant pour chaque diagonale.
 * 
 * @param b L'instance Board que l'on check.
 * @param gagne Tableau des coordonnées gagnantes.
 */
void coupGagnantDiagonale (Board* b, int** gagne);

/**
 * @brief En partant vers le haut à gauche, s'il y 3 jetons identique (celui en (x, y) inclu), et un placement possible au bout de ceux-ci, ajoute le coup à param gagne.
 * 
 * @param b L'instance Board que l'on check.
 * @param x La position x du tableau.
 * @param y La position y du tableau.
 * @param gagne Tableau des coordonnées gagnantes.
 */
void coupGagnantDiagonaleHautGauche (Board* b, int x, int y, int** gagne);

/**
 * @brief Explicite.
 * 
 * @param b L'instance Board que l'on check.
 * @param x La position x du tableau.
 * @param y La position y du tableau.
 * @param gagne Tableau des coordonnées gagnantes.
 */
void coupGagnantDiagonaleHautDroite (Board* b, int x, int y, int** gagne);

/**
 * @brief Explicite.
 * 
 * @param b L'instance Board que l'on check.
 * @param x La position x du tableau.
 * @param y La position y du tableau.
 * @param gagne Tableau des coordonnées gagnantes.
 */
void coupGagnantDiagonaleBasGauche (Board* b, int x, int y, int** gagne);

/**
 * @brief Explicite.
 * 
 * @param b L'instance Board que l'on check.
 * @param x La position x du tableau.
 * @param y La position y du tableau.
 * @param gagne Tableau des coordonnées gagnantes.
 */
void coupGagnantDiagonaleBasDroite (Board* b, int x, int y, int** gagne);

/**
 * @brief Si la case param b->jetons[x][y] est vide et qu'elle crée une victoire en étant remplie, ajoute le coup à param gagne (seulement si placement possible)
 * @brief DiagoHGBD = Diagonale partant d'en Haut à Gauche jusqu'en Bas à Droite 
 * @param b L'instance Board que l'on check.
 * @param x La position x du tableau.
 * @param y La position y du tableau.
 * @param gagne Tableau des coordonnées gagnantes.
 */
void coupGagnantMilieuDiagoHGBD (Board *b, int x, int y, int** gagne);

/**
 * @brief Comme coupGagnantMilieuDiagoHGBD mais pour la diagonale Haut Droite Bas Gauche.
 * 
 * @param b L'instance Board que l'on check.
 * @param x La position x du tableau.
 * @param y La position y du tableau.
 * @param gagne Tableau des coordonnées gagnantes.
 */
void coupGagnantMilieuDiagoHDBG (Board *b, int x, int y, int** gagne);

/**
 * @brief Vérifie si cette case est vide et si poser un pion ici (si possible) crée une victoire grace aux jetons des 2 côtés (sur la ligne)
 * 
 * @param b L'instance Board que l'on check.
 * @param x La position x du tableau.
 * @param y La position y du tableau.
 * @param gagne Tableau des coordonnées gagnantes.
 */
void coupGagnantMilieuLigne (Board *b, int x, int y, int **gagne);

/**
 * @brief Appelle coupGagnant. Si le joueur actuel peut gagner ou bloquer un coup gagnant adverse, joue et change le tour, sinon ne fait rien.
 * 
 * @param j 
 * @return true si le joueur a joué
 * @return false si il n'a pas joué
 */
bool jouerCoupGagnant (Jeu *j);

/**
 * @brief Regarde si le joueur en question peut gagner (simple vérification avec coupGagnant).
 * 
 * @param jeu Jeu
 * @param joueur 1 ou 2 selon le joueur
 * @return true si oui
 * @return false sinon
 */
bool coupGagnantJoueur (Jeu *jeu, int joueur);

#endif