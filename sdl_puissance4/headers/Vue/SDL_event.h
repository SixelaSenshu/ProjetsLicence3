/**
* @file SDL_event.h
* @author Alexis Bernard & William Dubois
* @brief Fichier qui permet de générer les evenements de la partie graphique du puissance 4.
    Elle contient:
        * Les structures pour les evenements de la SDL ainsi que les paramètres pour celle-ci
          - Creer & detruire :  Alloue et desalloue l'instance présente
        * Les fonctions pour gérer les evenements de la SDL
            - Attendre un evenement :
                * Mouse : Evenements via la souris et le pad
                * Window : Evemenents via la fenêtre
                * Button : Evenements via certaines touche du clavier
        * Un système d'erreur
* @date
* @version 1.0
*/
#ifndef _SDL_EVENT_H_
#define _SDL_EVENT_H_ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <SDL2/SDL.h>
#include <unistd.h>
#include <time.h>

#include "../macros.h"
#include "SDL_interface.h"

/// @brief Structure qui gère les évenements de la fenêtre du puissance4
/// Elle contient la structure SDL_Interface et une instance SDL_Event stocké dans une adresse mémoire.
/// L'instance permet via ces fonctions de faire tourner le jeu du puissance4 et faire des modifications sur le jeu / la fenêtre
/// @attention L'instance génére automatiquement la fenêtre et l'adresse de l'evenement, il n'y a donc aucune fonction hormis créer et détruire
/// Pour gérer cette structure
typedef struct evt
{
    SDL_Interface *fenetre; //!< interface du puissance4
    SDL_Event *event;       //!< gestionnaire des evenements du puissance4
    int choice;             //!< le choix d'affichage des boutons
    int joueur;             //!< joueur actuelle
} SDL_Event_Interface;

/// @brief Fonction qui initialise la fenêtre et les gestionnaires des évenements
///  @param interface L'interface sur la quelle on aura la gestion des évennements
/// @return Retourne le pointeur de l'instance courante
extern SDL_Event_Interface *SDL_Event_Interface_creer(SDL_Interface *interface);

/// @brief Détruit l'instance courante SDL_Event_Interface
/// @param evt_itf L'instance qui sera désallouée
extern void SDL_Event_Interface_detruire(SDL_Event_Interface *evt_itf);

/// @brief Lance un appel d'evenement à l'utilisateur
/// @param evt_itf L'instance qui sera désallouée
extern void SDL_Event_Interface_attendre(SDL_Event_Interface *evt_itf);

/// @brief Donne le numéro de la zone ou est situé le couple(x,y)
/// @param evt_itf L'instance sur laquelle on check le click
/// @param x La coordonée x ou on clique
/// @param y La coordonée y ou on clique
/// @return le numéro de la zone
/// @warning le numéro -1 sera donné si le couple x y est inaccesible
extern int SDL_Event_Interface_donneZoneClic(SDL_Event_Interface *evt_itf, Uint32 x, Uint32 y);

/// @brief L'instance ou sera défini les informations pour le jeu.
/// @param mdj Le mode de jeu.
/// @param ia1 Le niveau de l'ia1.
/// @param ia2 Le niveau de l'ia2.
/// @param evt_itf L'instance SDL_Event_Interface.
/// @param choice Le choix d'affichage des buttons.
extern void SDL_Event_Interface_setInfoJeu(ModeDeJeu mdj, IA ia1, IA ia2, SDL_Event_Interface *evt_itf, int choice);

/// @brief L'instance ou sera défini les informations pour le jeu ainsi que la création du nouveau jeu et de ces paramètre
/// @param evt_itf L'instance SDL_Event_Interface.
/// @return si la création est un succès
extern bool SDL_Event_Interface_recreerJeu(SDL_Event_Interface *evt_itf);

///@brief Fonction qui lance la fenêtre, attend la fin du programme ou la fermeture de la fenêtre puis désalloue toutes les instances
extern void lancementMVC();
#endif
