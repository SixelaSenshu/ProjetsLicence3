/**
 * @file Joueur.h
 * @author William Dubois & Alexis Bernard
 * @brief Contient l'enum IA, la struct Joueur et quelques fonctions en rapport.
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef JOUEUR_H
#define JOUEUR_H

#include "Board.h"
#include "Puissance4.h"
#include "../macros.h"

/**
 * @brief Contient les niveaux de difficultés de l'ia (FACILE, NORMAL et DIFFICILE)
 * @brief USER ne correspond à aucune ia, seulement les commande de l'utilisateur sont prises en compte dans ce cas
 * @brief ERREUR n'est utilisé que quand l'ia demandée par l'utilisateur est incorrecte dans le cas où il crée une partie JvsIA ou IAvsIA
 */
typedef enum ia_ {FACILE, NORMAL, DIFFICILE, USER, ERREUR} IA;

/**
 * @brief Représente un joueur avec son nom param name, son score param score, l'ia utilisée param ia, et le 
 * pointeur de fonction param play correspondant à l'ia pour jouer un jeton
 * 
 * @param play La fonction qui permet de jouer.
 * @param ia La difficulté de l'IA.
 * @param score Le score de joueur.
 * @param name Le nom du joueur.
 * @param numero 1 ou 2 selon si il est le Joueur 1 ou le joueur 2.
 */
typedef struct joueur {
    char* name;
    unsigned short int score, numero;
    IA ia;
    void (*play)();
} Joueur;

/**
 * @brief Crée un joueur, assigne le nom param name au joueur, assigne l'ia param ia au joueur, et assigne 
 * la fonction correspondante à l'ia au pointeur de fonction du joueur.
 * 
 * @param name Le nom du joueur.
 * @param ia La difficulté de l'ia.
 * @return Joueur* si success, NULL sinon (problème malloc Joueur* ou char*, ou param ia invalide).
 */
Joueur* creerJoueur (char* name, IA ia);

/**
 * @brief Free param j s'il existe.
 * @param j L'instance du joueur qui sera free.
 */
void freeJoueur (Joueur* j);

/**
 * @brief Retourne une copie complète du joueur mais un nouveau pointeur.
 * 
 * @param j Joueur à copier
 * @return Joueur* 
 */
Joueur* copyJoueur (Joueur* j);

#endif