/**
 * @file main.h
 * @author Alexis Bernard & William Dubois
 * @brief Fichier principal du programme. Permet de lancer les deux modes de puissances4.
 *          -s permet de lancer le mode graphique
 *          -c permet de lancer le mode standard
 * @date 31.12.202
 * @version 1.0
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "macros.h"
#include "Vue/SDL_event.h"
#include "Vue/SDL_interface.h"
#include "Jeu/Puissance4.h"
#include "Vue/jeuTextuel.h"
#include "Jeu/IA.h"
#include "Jeu/InfoIA.h"

/// @brief Fonction principal du programme.
/// @param argc Nombre de paramètres sur la sortie standard.
/// @param argv Les paramètres sur la sortie standard mis par l'utilisateur
/// @return Si le programme s'est bien lancé et terminé.
/// @warning Les paramètres sur la sortie standard mis par l'utilisateur sont facultatif, il sont mis par défaut.
int main(int argc, char **argv);