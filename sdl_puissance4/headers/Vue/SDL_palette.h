/**
* @file SDL_palette.h
* @author Alexis Bernard & William Dubois
* @brief Fichier qui permet de générer la palette de couleur de la partie graphique du puissance 4.
    Elle contient:
        * Les structures pour la SDL ainsi que les paramètres pour celle-ci
            - SDL_Palette : Contient tous les paramètres de couleurs pour la SDL_Interface
        * Les fonctions pour gérer les ressources de la palette
            - Creer : Alloue l'instance SDL_Palette 
            - Detruire : Désalloue l'instance SDL_Palette
        * Les fonctions pour gérer les couleurs;  
            - Permet de changer les couleurs de la palette
            - D'appliquer une couleur à un renderer
            - Permet de donner la couleur d'une composante
        * Un système d'erreur
* @date 
* @version 1.0
*/
#ifndef _SDL_PALETTE_H_
#define _SDL_PALETTE_H_ 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <SDL2/SDL.h>

#include "../macros.h"

/// @brief Palette de couleur qui contient toutes les fonctions en lien avec le changement de couleur. 
/// Chaque composantes { Mur , Joueur1, Joueur2, Case Vide, Boutton et Texte i.e ligne} à une couleur qui peut être modifiée.
/// @warning Chaque changement de couleur n'affecte pas les anciennes couleurs sur l'ecran hormis si il y a une mise à jour
typedef struct sdl_palette{
    SDL_Color mur;            //!< Couleur pour le mur de l'applications
    SDL_Color j1;             //!< Couleur pour le joueur 1
    SDL_Color j2;             //!< Couleur pour le joueur 2
    SDL_Color texte;          //!< Couleur pour le texte
    SDL_Color bouton;         //!< Couleur pour les buttons
    SDL_Color casevide;       //!< Couleur pour les cases vides
}SDL_Palette_Interface;

/// @brief L'énumération pour la palette de couleur
typedef enum sdl_palette_type_couleur {
    SDL_PALET_TYPE_BACKGROUND,     //!< Couleur pour le mur de l'application
    SDL_PALET_TYPE_PLAYER1,        //!< Couleur pour le joueur 1
    SDL_PALET_TYPE_PLAYER2,        //!< Couleur pour le joueur 2
    SDL_PALET_TYPE_JETONVIDE,      //!< Couleur pour les cases vides
    SDL_PALET_TYPE_TEXT,           //!< Couleur pour le texte
    SDL_PALET_TYPE_BUTTON          //!< Couleur pour les buttons
} SDL_Palette_Type;

/// @brief Créer l'instance SDL_Palette_Interface pour un ensemble de couleur destinée à une autre instance SDL_Interface.
/// @return Retourne l'instance qui vient d'être créer.
extern SDL_Palette_Interface* SDL_Palette_creer();

/// @brief Détruit l'instance courante de la palette destinée à la SDL_Interface.
/// @param palette L'instance qui sera détruit.
/// @return Si la destruction c'est bien passée ou non.
extern bool SDL_Palette_detruire(SDL_Palette_Interface* palette);

/// @brief Change dans l'instance SDL_Palette_Interface une couleur pour un de ces paramètres à partir d'une couleur et du type.
/// @warning Si le type est incorrect, il n'y a aucun changement.
/// @param palette L'instance SDL_Palette_Interface qui subit le changment.
/// @param type Le type d'élement sur lequel on changera la couleur.
/// @param couleur La couleur que l'on applique.
/// @return Retourne vrai ou faux si le changement c'est bien déroulé.
extern bool SDL_Palette_changeCouleurElement(SDL_Palette_Interface* palette, SDL_Palette_Type type, SDL_Color couleur);

/// @brief Donne l'une des couleurs de la palette de l'instance SDL_Palette_Interface.
/// @warning Si le type est incorrect, la valeur donnée sera {0,0,0,255}.
/// @param palette La palette sur laquel on récupère la couleur.
/// @param type Le type d'élement sur lequel on veut la couleur.
/// @return Le type d'élement sur lequel on veut la couleur.
extern SDL_Color SDL_Palette_donneCouleurElement(SDL_Palette_Interface* palette, SDL_Palette_Type type);

/// @brief Applique sur une instance SDL_Renderer la couleur d'un élement de la palette.
/// @param palette La palette sur laquelle on récupère la couleur. 
/// @param type Le type de la couleur que l'on veut récupèrer.
/// @param fenetre La fenêtre sur laquelle on change le stylo.
/// @return Retourne vrai ou faux si le changement s'est appliqué.
extern bool SDL_Palette_appliqueCouleurElement(SDL_Palette_Interface* palette, SDL_Palette_Type type, SDL_Renderer * fenetre);

#endif 