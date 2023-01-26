/**
 * @file InfoIA.h
 * @author William Dubois & Alexis Bernard
 * @brief Implémente la notion de Sections, toutes les fonctions qui les utilisent, ainsi que l'enum TypeLigne qui est utilisée dans celles-ci.
 * @brief Contient également toutes les fonctions nécessaires pour la prédiction de coups pour l'IA Difficile.
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef INFOIA_H
#define INFOIA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "Board.h"
#include "Puissance4.h"
#include "IA.h"

/**
 * @brief Type de section trouvée dans la grille
 * @param COLONNE 2 ou 3 jetons de la même couleur alignés en colonne
 * @param DIAGONALE 2 ou 3 jetons de la même couleur alignés en diagonale
 * @param LIGNE 2 ou 3 jetons de la même couleur alignés en ligne
 * @param POINT Jeton seul, n'appartient impérativement à aucune autre section (jeton isolé de sa couleur dans chaque direction)
 * 
 */
typedef enum _typeLigne {COLONNE, DIAGONALE, LIGNE, POINT} TypeLigne;

/**
 * @brief Partie de ligne de jetons de la même couleur. Chaque jeton peut appartenir à plusieurs sections avec des types différents, sauf pour le type POINT
 * @param type Type de ligne (verticale, horizontale, diagonale, ou seulement un point)
 * @param couleur Couleur des jetons évalués (1 ou 2 selon le joueur)
 * @param jetons Tableau de taille 2 à 6, contenant les coordonnées x et y de 1 à 3 jetons
 * @param nbJetons Nombre de jetons stockés dans le tableau de jetons
 * @param ext1 Extremité n°1 contient la couleur du jeton à la première extrémité de la section (hors de la section), et vaut 0 quand la case est vide
 * @param ext2 Extremité n°2 contient la couleur du jeton à la deuxième extrémité de la section (hors de la section), et vaut 0 quand la case est vide
 * @warning Les extrémités vaudront -1 par défaut. Elles seront toujours à -1 tant le cas d'un POINT. Dans le cas d'une colonne, l'extrémité 2 vaudra -1 (inutile)
 * @warning Si la case à l'extrémité est hors des limites de jeu, elle vaut -1.
 * @param e1 Coordonnées de l'extrémité n°1
 * @param e2 Coordonnées de l'extrémité n°2
 * 
 */
typedef struct _section {
    TypeLigne type;
    int couleur;
    int *jetons;
    int nbJetons;
    int ext1, ext2;
    int e1[2], e2[2];
} Section;

/**
 * @brief Rassemble toutes les sections calculées ainsi que leur nombre
 * 
 */
typedef struct _tabSections {
    Section **s;
    int nbSections;
} TableauSections;

/**
 * @brief Crée une section ainsi qu'initialise ses valeurs
 * 
 * @param t Type de ligne
 * @param nbJetons Nombre de jetons
 * @param x Coordonnée x du premier jeton
 * @param y Coordonnée y du premier jeton
 * @param ... Coordonnées (x, y) des potentiels 2ème et 3ème jetons
 * @return Section* ou NULL dans le cas d'un problème d'allocation
 */
Section* creerSection (TypeLigne t, int couleur, int nbJetons, int x, int y, ...);

/**
 * @brief Crée un tableau de section initialisé à 0 sections et et avec un tableau de sections NULL
 * 
 * @return TableauSections* ou NULL si erreur d'allocation
 */
TableauSections* creerTableauSections ();

/**
 * @brief Calcule les extrémités d'une section selon leur type
 * 
 * @param b Board
 * @param s Section
 */
void calculeExtremite (Board *b, Section **s);

/**
 * @brief Appelle calculeExtremite pour chaque section
 * 
 * @param b Board
 * @param ts Tableau de sections
 */
void calculeExtremiteTS (Board *b, TableauSections **ts);

/**
 * @brief Ajoute une section a un Tableau de sections, malloc si NULL, realloc si déjà défini
 * 
 * @param section Une section
 * @param ts L'adresse d'un pointeur de tableau de section
 */
void ajouteSectionTS (Section* section, TableauSections **ts);

/**
 * @brief Cherche, crée, et ajoute au tableau de sections toutes les sections en colonne
 * 
 * @param b Le board où l'on cherche les sections
 * @param ts L'adresse d'un pointeur de tableau de section
 */
void chercherSectionsColonnes (Board *b, TableauSections **ts);

/**
 * @brief Cherche, crée, et ajoute au tableau de sections toutes les sections en ligne
 * 
 * @param b Le board où l'on cherche les sections
 * @param ts L'adresse d'un pointeur de tableau de section
 */
void chercherSectionsLignes (Board *b, TableauSections **ts);

/**
 * @brief Cherche, crée, et ajoute au tableau de sections toutes les sections en diagonale.
 * @brief Appelle HGBD_Gauche, HGBD_Haut, HDBG_Haut et HDBG_Droite pour cela.
 * 
 * @param b Le board où l'on cherche les sections
 * @param ts L'adresse d'un pointeur de tableau de section
 */
void chercherSectionsDiagonales (Board *b, TableauSections **ts);

/**
 * @brief Diagonale Haut Gauche - Bas Droite / Cherche ces sections parmi les diagonales partant du côté gauche du board (sauf la 1ère, fait dans HGBD_Haut)
 * 
 * @param b Le board où l'on cherche les sections
 * @param ts L'adresse d'un pointeur de tableau de section
 */
void HGBD_Gauche (Board *b, TableauSections **ts);

/**
 * @brief Diagonale Haut Gauche - Bas Droite / Cherche ces sections parmi les diagonales partant du haut du board
 * 
 * @param b Le board où l'on cherche les sections
 * @param ts L'adresse d'un pointeur de tableau de section
 */
void HGBD_Haut (Board *b, TableauSections **ts);

/**
 * @brief Diagonale Haut Droite - Bas Gauche / Cherche ces sections parmi les diagonales partant du haut du board
 * 
 * @param b Le board où l'on cherche les sections
 * @param ts L'adresse d'un pointeur de tableau de section
 */
void HDBG_Haut (Board *b, TableauSections **ts);

/**
 * @brief Diagonale Haut Droite - Bas Gauche / Cherche ces sections parmi les diagonales partant du côté droit du board (sauf la 1ère, fait dans HDBG_Haut)
 * 
 * @param b Le board où l'on cherche les sections
 * @param ts L'adresse d'un pointeur de tableau de section
 */
void HDBG_Droite (Board *b, TableauSections **ts);

/**
 * @brief Cherche, crée, et ajoute au tableau de sections toutes les sections restantes: jetons isolés n'appartenant à aucune section
 * 
 * @param b Le board où l'on cherche les sections
 * @param ts L'adresse d'un pointeur de tableau de section
 */
void chercherSectionsRestantes (Board *b, TableauSections **ts);

/**
 * @brief Cherche si un jeton appartient à une section
 * 
 * @param x Coordonnées x du jeton
 * @param y Coordonnées y du jeton
 * @param ts Une section
 * @return true si on trouve le jeton
 * @return false sinon
 */
bool cherchePointSection (int x, int y, Section* s);

/**
 * @brief Cherche si un jeton appartient aux extrémité d'une section
 * 
 * @param x Coordonnées x du jeton
 * @param y Coordonnées y du jeton
 * @param ts Une section
 * @return true si on trouve le jeton
 * @return false sinon
 */
bool chercheExtremiteSection (int x, int y, Section* s);

/**
 * @brief Cherche si un point (ou un jeton) appartient déjà à une section parmi le tableau de sections. Utilisée pour chercherSectionsRestantes.
 * 
 * @param x Coordonnées x du jeton
 * @param y Coordonnées y du jeton
 * @param ts L'adresse d'un pointeur de tableau de section
 * @return true si on trouve le jeton
 * @return false sinon
 */
bool cherchePointTS (int x, int y, TableauSections **ts);

/**
 * @brief Appelle chercherSectionsColonnes, chercherSectionsLignes, chercherSectionsDiagonales et chercherSectionsRestantes
 * 
 * @param b Le board où l'on cherche les sections
 * @param ts L'adresse d'un pointeur de tableau de section où l'on ajoute les sections
 */
void chercherSections (Board *b, TableauSections **ts);

/**
 * @brief Affiche une section
 * 
 * @param s Section valide
 */
void afficherSection (Board *b, Section* s);

/**
 * @brief Affiche toutes les sections d'un tableau de sections
 * 
 * @param ts Tableau de sections à afficher
 */
void afficherTableauSections (Board *b, TableauSections *ts);

/**
 * @brief Free une section
 * 
 * @param s Section
 */
void freeSection (Section* s);

/**
 * @brief Free un tableau de sections ainsi que toutes les sections qu'il contient
 * 
 * @param ts Tableaus de sections
 */
void freeTableauSections (TableauSections* ts);

/**
 * @brief Free un tableau de section
 * 
 * @param ts Tableau de sections
 */
void softFreeTableauSections (TableauSections* ts);

/**
 * @brief Regarde si un des jetons de la section contient une coordonnée x correspondante
 * 
 * @param x Coordonnée recherchée
 * @param s Section
 * @return true si elle présente
 * @return false sinon
 */
bool coordonneeXestDansSection (int x, Section* s);

/**
 * @brief Regarde si un des jetons de la section contient une coordonnée y correspondante
 * 
 * @param x Coordonnée recherchée
 * @param s Section
 * @return true si elle présente
 * @return false sinon
 */
bool coordonneeYestDansSection (int y, Section* s);

/**
 * @brief Informe de la direction de la diagonale enregistrée dans la section
 * 
 * @param s 
 * @return 0 pour erreur, 1 pour HGBD, 2 pour HDBG
 */
int directionSectionDiagonale (Section* s);

/**
 * @brief Vérifie si la section entrée donne une double victoire à l'un des joueurs (double victoire = coup gagnant des deux côtés, garantissant une victoire)
 * 
 * @param b Board pour vérifier si des cases sont vides ou pleines
 * @param s Section
 * @return true si oui
 * @return false sinon
 */
bool estSectionDoubleVictoire (Board *b, Section* s);

/**
 * @brief Regarde chaque section contenue dans ts et vérifie si elle donne une double victoire au joueur actuel (stocké dans le board)
 * 
 * @param b Board
 * @param ts Tableau de Sections observé
 */
bool chercheSectionsDoubleVictoire (Board *b, TableauSections *ts);

/**
 * @brief Regarde si le point (x, y) est une des extrémités de la Section donnée
 * 
 * @param x Coordonnée x
 * @param y Coordonnée y
 * @param s Section
 * @return true si oui
 * @return false sinon
 */
bool cherchePointExtremiteSection (int x, int y, Section *s);

/**
 * @brief Il a été trouvé que le dernier jeton joué est à l'extrémité de la section donnée, et que c'est la même couleur.
 * @brief Vérifie dans cette fonction, en recalculant les sections avec le dernier jeton, 
 * @brief si cela empêche l'adversaire de jouer dans une des colonnes à l'extrémité de la nouvelle section (si il joue dans cette colonne coup gagnant pour vous).
 * 
 * @param b Board pour recalculer les sections
 * @param s Section affectée par le dernier jeton
 * @return true si vous bloquez une colonne à l'adversaire
 * @return false sinon
 */
bool emplacementInterditAdversaire (Board *b, Section *s);

/**
 * @brief Regarde toutes les sections contenant le dernier jeton joué dans le Board.
 * 
 * @param b Board
 * @param ts Tableau de sections
 * @return 0 pour aucune section importante (seulement des POINT), 1 pour avoir bloqué une section adverse, 2 pour avoir agrandi une section, 
 * @return 3 pour avoir agrandi une section et crée 1 ou 2 emplacement interdits à l'adversaire, 4 pour avoir fait 1 et 2, 5 pour avoir fait 1 et 3.
 */
int chercheSectionsDernierJeton (Board *b, TableauSections *ts);

/**
 * @brief Remplis grade avec des int allant de 0 à 4, chacun indiquant un placement de plus en plus avantageux.
 * @brief Fait une copie du jeu, puis évalue le l'attractivité de jouer (grade) de chaque colonne.
 * @brief Fonctionnement: joue dans une case, évalue les possibilités, si l'adversaire gagne dans un cas, grade[i] = 1.
 * @brief Si l'adversaire ne gagne dans aucun cas, regarde si vous avez agrandi une de vos sections ou empéché l'adversaire de
 * @brief former une section de longueur 3, reliée ou non (grade[i] = 4 si oui pour les deux, 3 pour un seul, 2 sinon)
 * @brief Si l'adversaire ne peut plus jouer dans une    colonne car cela vous donnerait un coup gagnant, grade[i] = 5
 * @brief Si vous avez une chance de former une double victoire, ou si l'adversaire n'a plus aucune colonne dans laquelle jouer
 * @brief sans vous faire gagner, grade[i] = 5. Après tout ça, passe à la colonne suivante.
 * 
 * @param grade 0 si vous permettez à l'adversaire de vous bloquer une ligne, 1 pour NE PAS JOUER (donne un coup gagnant à l'adversaire),
 * @param grade 2 pour OK, rien de spécial, 3 pour AVANTAGEUX, agrandit une de vos lignes ou bloque une section adverse,
 * @param grade 4 pour les deux: agrandit une ligne + bloque l'adversaire,
 * @param grade 5 pour double victoire ou bloquage d'une colonne pour l'adversaire
 * @param grade -1 pour colonne pleine.
 * @param jeu 
 */
void predictionCoups (int **grade, Jeu* jeu);

#endif