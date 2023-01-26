/**
 * @file Puissance4.h
 * @author William Dubois & Alexis Bernard
 * @brief Contient le nécessaire pour créer un objet Jeu qui est le coeur du jeu. Utilise la struct Jeu et l'enum ModeDeJeu pour cela.
 * @version 0.1
 * @date 2023-01-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef PUISSANCE4_H
#define PUISSANCE4_H

#include "Board.h"
#include "Joueur.h"
#include "../macros.h"

/**
 * @brief Enumeration des différents modes de jeu
 * @param JvsJ Joueur contre Joueur
 * @param JvsIA Joueur contre IA
 * @param IAvsIA IA contre IA
 *
 */
typedef enum mode_
{
    JvsJ,
    JvsIA,
    IAvsIA
} ModeDeJeu;

/**
 * @brief Structure du jeu de Puissance 4
 * @param b Plateau de jeu contenant les jetons et autres
 * @param j1 Joueur 1
 * @param j2 Joueur 2
 * @param mode Mode de jeu
 *
 */
typedef struct _jeu
{
    Board *b;
    struct joueur *j1;
    struct joueur *j2;
    ModeDeJeu mode;
} Jeu;

/**
 * @brief Crée un jeu: crée le board et appelle creerJoueursSelonMode.
 *
 * @param mode Le mode de jeu.
 * @return Jeu* ou NULL si erreur.
 */
Jeu *creerJeu(ModeDeJeu mode);

/**
 * @brief Crée un jeu: crée le board et appelle creerJoueursSelonMode.
 *
 * @param mode Le mode de jeu.
 * @param ia1 Le niveau de l'ia1
 * @param ia2 Le niveau de l'ia2
 * @return Jeu* ou NULL si erreur.
 */

Jeu *creerJeuGraphique(ModeDeJeu mode, enum ia_ ia1, enum ia_ ia2);

/**
 * @brief Crée 2 joueurs selon le mode de jeu de param jeu, si création d'une ia: appel de demandeDifficulteIA
 *
 * @param jeu L'instance de Jeu.
 * @param ia1 Le niveau de l'ia1
 * @param ia2 Le niveau de l'ia2
 */
void creerJoueursSelonModeGraphique(Jeu **jeu, enum ia_ ia1, enum ia_ ia2);

/**
 * @brief Crée 2 joueurs selon le mode de jeu de param jeu, si création d'une ia: appel de demandeDifficulteIA
 *
 * @param jeu L'instance de Jeu.
 */
void creerJoueursSelonMode(Jeu **jeu);

/**
 * @brief Demande à l'utilisateur la difficulté de l'IA qu'il veut créer.
 *
 * @return Enum ia_, ERREUR si mauvaise entrée, ce qui ne crée pas de joueur.
 */
enum ia_ demandeDifficulteIA();

/**
 * @brief Free le jeu: les 2 joueurs, le board er lui-même.
 *
 * @param jeu L'instance de Jeu.
 */
void freeJeu(Jeu *jeu);

/**
 * @brief Fait une copie du jeu, utilisée pour la prédiction des coup pour l'ia difficile
 *
 * @param jeu Jeu à copier
 * @return Jeu*
 */
Jeu *copyJeu(Jeu *jeu);

#endif