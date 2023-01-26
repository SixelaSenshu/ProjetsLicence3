/**
* @file SDL_interface.h
* @author Alexis Bernard & William Dubois
* @brief Fichier qui permet de générer la partie graphique du puissance 4.
    Elle contient:
        * Les structures pour la SDL ainsi que les paramètres pour celle-ci
            - SDL_Interface : L'instance qui contient toutes les composantes pour afficher et faire tourner la SDL
            - SDL_Interface_Coord : Toutes les coordonées de la SDL pour les textures qui seront misent en place
        * Les fonctions pour gérer les ressources de la SDL
            - La création de chaque composante via SDL_Interface_creer[Le nom de la composante]
            { Cas à part pour SDL_Window * -> creerFenetreSDL}
            - Les fonctions de SDL_Interface_creer et SDL_Interface_Coord_creer
            - La fonction de désallocation de SDL_Interface et SDL_Interface_Coord
        * Les fonctions pour gérer les textures et le renderer de la fenêtre
            - Les différentes fonctions pour créer : Liste de boutons, Grille, Information
            - Les différentes fonctions pour actualiser : Texture , Coordonées
            - Les différents fonctions utilitaire : Nettoyer, Changer le mode d'affichage
        * Un système d'erreur
* @date 31.12.202
* @version 1.0
*/
#ifndef _SDL_INTERFACE_H_
#define _SDL_INTERFACE_H_ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "../macros.h"
#include "../Jeu/Puissance4.h"
#include "SDL_palette.h"

#define SDL_INTERFACE_NB_COLONNE 7 //!< Le nombre de colonne du jeu
#define SDL_INTERFACE_NB_LIGNE 6   //!< Le nombre de ligne du jeu

///@brief Macro qui permet de raccourcir le code pour le cas ou l'instance SDL_Interface n'est pas allouée
#define SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(error, output, msg, val_return) \
    fprintf(output, msg);                                                     \
    setErreur(error);                                                         \
    return val_return;

/// @brief Paramètres pour la gestion de la fenètre de la SDL
typedef enum sdl_interface_param_window
{
    SDL_INTERFACE_WINDOW_MAX,     //!< Maximise l'interface
    SDL_INTERFACE_WINDOW_MIN,     //!< Minimise l'interface
    SDL_INTERFACE_WINDOW_RESTORE, //!< Restaure l'interface
    SDL_INTERFACE_WINDOW_SHOW,    //!< Affiche l'interface actuelle
} SDL_Interface_Param_Window;

/// @brief Structure qui contient tous les coordonées x, y, h , l de la fenêtre
/// À chaque update, les coordonées seront mis à jours afin de garder des valeurs flexibles pour tous changements
/// qu'ils soient des agrandissements, retrécissement,...
///  @warning Pour désallouer directement, il existe la fonction SDL_Interface_Coord_detruire
/// Sinon on peut passer par la fonction SDL_Interface_detruire qui supprimera l'instance dans l'objet SDL_Interface
typedef struct _sdl_interface_coord
{
    SDL_Rect *grille;  //!< Les positions pour la grille qui contient le jeu
    SDL_Rect *info;    //!< Les positions pour les information { dernière position , type de jeu, couleur ,...}
    SDL_Rect *boutons; //!< Les positions pour les bouttons et autres composantes qui permet le lancement du jeu
    unsigned nbPos;    //!< Le nombre de position
} SDL_Interface_Coord;

/// @brief Structure qui contient tous les éléments de la SDL ainsi que le jeu pour permettre l'affichage du puissance4 sur l'affichage graphique via SDL.
/// La structure SDL_Window * contient l'adresse mémoire pour la fenêtre qui permet de stocker toutes les informations.
/// La structure SDL_Renderer * contient l'adresse mémoire pour la zone d'affichage ou on écrira et dessinera. Elle appartient à la SDL_Window *
/// La structure SDL_Texture * contient le tableau de pixels (soit le calque) qui sera modifier et coller sur le renderer.
/// La structure SDL_Palette * contient toutes les composantes de couleurs pour la fenêtre.
/// La structure SDL_Interface_Coord* contient toutes les coordonées pour les dessins et calques. Tous est calculés par défaut et à chaque évenement,
/// la structure se doit d'être à jour via la fonction SDL_Interface_ActualiserTexture(SDL_interface * itf).
/// @attention La structure initialise en même temps toute les composantes, il n'y a pas besoin de le faire manuellement
/// @warning Les composantes SDL_Window *, SDL_ Renderer * et SDL_ Texture * sont alloués dans cet ordre.
/// Si l'interface n'est pas alloué, alors rien n'est possible et une erreur se lance.
/// Dans l'autre cas , on considère que toutes les fonctions sont possible
typedef struct _sdl_interface
{
    SDL_Window *fenetre;                    //!< La fenêtre qui contient tous les éléments
    int hauteur, largeur;                   //!< La hauteur et largeur de la fenêtre
    SDL_Interface_Param_Window paramwindow; //!< Le paramètre d'affichage et dimension de la fenêtre
    SDL_Renderer *renderer;                 //!< Le renderer pour la fenêtre
    SDL_Texture *texture;                   //!< La texture de la fenêtre
    SDL_Palette_Interface *couleurs;        //!< Palette de couleur pour la fenêtre
    SDL_Interface_Coord *coordonnes;        //!< Coordonnées de la fenêtre
    Jeu *jeu;                               //!< Jeu contenant le puissance4
    IA ia1;                                 //!< Le niveau de difficulté de l'ia1
    IA ia2;                                 //!< Le niveau de difficulté de l'ia2
    ModeDeJeu mode_jeu;                     //!< Mode de jeu du puissance4
} SDL_Interface;

/// @brief Initialise la structure SDL_interface, sa hauteur et sa largeur.
/// Met à zéro la valeur paramWindow en mode vue simple sans modification
/// à partir de la hauteur et largeur donné. Les composantes de la SDL_Interface
/// Sont mise à zéro par défaut.
/// @param hauteur Hauteur de la fenêtre.
/// @param largeur Largeeur de la fenêtre.
/// @return L'adresse mémoire de la fenêtre si l'allocation est une réussite.
/// Sinon, la fonction retourne NULL.
extern SDL_Interface *SDL_Interface_Creer(unsigned hauteur, unsigned largeur);

/// @brief Initialise la structure SDL_Window de la structure SDL_Interface.
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @param interface L'instance courante SDL_Interface ou sera donnée l'adresse mémoire de la SDL_Window.
/// @param titre Le titre de la fenêtre .
/// @return Retourne si la création à été un succès ou non.
extern bool SDL_Interface_creerFenetreSDL(SDL_Interface *interface, char *titre);

/// @brief Initialise la structure SDL_Renderer de la structure SDL_Interface.
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @param interface L'instance courante SDL_Interface ou sera donnée l'adresse mémoire de la SDL_Renderer.
/// @return Retourne si la création à été un succès ou non.
extern bool SDL_Interface_creerRenderer(SDL_Interface *interface);

/// @brief Initialise la structure SDL_Texture de la structure SDL_interface.
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @param interface L'instance courante SDL_Interface ou sera donnée l'adresse mémoire de la SDL_Texture.
/// @return Retourne si la création à été un succès ou non.
extern bool SDL_Interface_creerTexture(SDL_Interface *interface);

/// @brief Initialise la structure SDL_Palette de la structure SDL_interface.
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @param interface L'instance courante SDL_Interface ou sera donnée l'adresse mémoire de la SDL_Palette.
/// @return Retourne si la création à été un succès ou non.
extern bool SDL_Interface_creerPalette(SDL_Interface *interface);

/// @brief Initialise la structure Jeu de la structure SDL_interface.
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @param interface L'instance courante SDL_Interface ou sera donnée l'adresse mémoire de la Jeu.
/// @return Retourne si la création à été un succès ou non.
extern bool SDL_Interface_creerJeu(SDL_Interface *interface);

/// @param interface L'instance courante SDL_Interface ou sera donnée l'adresse mémoire de la Jeu.
/// @param mode Le mode de jeu qui sera lancer.
/// @param ia1 Le niveau de l'ia 1
/// @param ia2 Le niveau de l'ia 2
/// @return Si le changement est un succès
extern bool SDL_Interface_modifierJeu(SDL_Interface *interface, ModeDeJeu mode, IA ia1, IA ia2);

/// @brief Initialise la structure SDL_Interface_Coord de la structure SDL_interface.
/// @param interface L'instance courante SDL_Interface ou sera donnée l'adresse mémoire de la SDL_Interface_Coord.
/// @return Retourne si la création à été un succès ou non.
extern bool SDL_Interface_Coord_creer(SDL_Interface *interface);

/// @brief Désalloue l'addresse mémoire pour la structure SDL_interface_Coord à partir d'un instance SDL_Interface.
/// @warning Cette function ne produit rien si l'instance est NULL ou que SDL_interface->jeu est null.
///  @param interface L'instance courante SDL_Interface qui sera se vera désalloué sa composante SDL_Interface_Coord.
/// @return Si la désallocation de l'instance est un succès ou non.
extern bool SDL_Interface_Coord_Detruire(SDL_Interface *interface);

/// @brief Désalloue l'addresse mémoire pour la structure SDL_interface.
/// @warning Cette function ne produit rien si l'instance est NULL.
///  @param interface L'instance courante SDL_Interface qui sera désalloué.
/// @return Si la désallocation de l'instance est un succès ou non.
extern bool SDL_Interface_detruire_(SDL_Interface **interface);

/// @brief Change la position et taille de la fenêtre de SDL_Interface.
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @warning Changer l'affichage peut provoquer un affichage et un rendu non souhaité, il est donc préférable de ne pas minimiser trop la taille.
///  @param interface L'instance courante SDL_Interface qui sera modifié.
/// @param itf_option L'option de la SDL_window que l'on veut appliquer.
extern bool SDL_Interface_changerModeVisuel(SDL_Interface *interface, SDL_Interface_Param_Window itf_option);

/// @brief Nettoie l'écran de la SDL_renderer de l'instance courante SDL_Interface.
/// @warning Cette function ne produit rien si l'instance est NULL.
///  @param interface L'instance courante SDL_interface qui sera modifié.
/// @return Retourne si le nettoyage à été un succès ou non.
extern bool SDL_Interface_nettoyerEcran(SDL_Interface *interface);

/// @brief Permet de mettre une image sur l'interface graphique.extern bool SDL_Interface_mettreImage(SDL_Interface **interface, const char filename[], SDL_Rect *rect).
/// @param interface L'instance SDL_Interface sur laquelle on met une image.
/// @param filename Le nom ( ainsi que le chemin ) du fichier.
/// @param rect La zone ou on pose l'image.
/// @return Si l'image à bien été mis ou non
extern bool SDL_Interface_mettreImage(SDL_Interface *interface, const char filename[], SDL_Rect *rect);

/// @brief Créer la grille du Puissance4.
/// @warning Cette fonction peut avoir un rendu non voulu ou non affichable si les paramètres de taille sont mauvais ou trop petit.
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @param interface L'instance courante SDL_interface ou sera créer la grille.
/// @param coord Les coordonées x, y, la hauteur et la largeur ou on va faire la grille.
/// @return Si la création est un succès ou non.
extern bool SDL_Interface_dessinerGrille(SDL_Interface **interface, SDL_Rect *coord);

/// @brief Créer les buttons pour l'interface du Puissance4.
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @param interface L'instance courante SDL_interface ou sera créer les buttons.
/// @param coord Les coordonées x, y, la hauteur et la largeur ou on va faire les buttons.
/// @param choice Affichage des niveaux de difficultés ou des modes de jeux.
/// @return Si la création est un succès ou non.
extern bool SDL_Interface_dessinerBouttons(SDL_Interface **interface, SDL_Rect *coord, int choice);

/// @brief Créer les info pour l'interface du Puissance4
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @param interface L'instance courante SDL_interface ou sera créer les informations.
/// @param coord Les coordonées x, y, la hauteur et la largeur ou on va faire les information.
/// @return Si la création est un succès ou non.
extern bool SDL_Interface_dessinerInfo(SDL_Interface **interface, SDL_Rect *coord);

// @brief Créer le résultat pour l'interface du Puissance4
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @param interface L'instance courante SDL_interface ou sera créer les informations.
/// @param coord Les coordonées x, y, la hauteur et la largeur ou on va faire les information.
/// @return Si la création est un succès ou non.
extern bool SDL_Interface_dessinerResultat(SDL_Interface *interface, SDL_Rect *coord);

/// @brief Actualise l'interface du Puissance4.
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @param itf L'instance courante SDL_interface ou sera actualiser les composantes.
/// @param mode Le mode d'affichage de l'interface.
/// @param choice Affichage des niveaux de difficultés ou des modes de jeux.
/// @return Si l'actualisation est un succès.
extern bool SDL_Interface_actualiserTexture(SDL_Interface *itf, SDL_Interface_Param_Window mode, int choice);

/// @brief Actualise les coordonées de l'interface.
/// @warning Cette function ne produit rien si l'instance est NULL.
/// @param itf L'instance courante SDL_interface ou sera actualiser les coordonnées.
/// @return Si l'actualisation est un succès.
extern bool SDL_Interface_actualiserCoordonees(SDL_Interface *itf);

/// @brief Actualise les information du jeu.
/// @param itf L'instance courante SDL_interface ou sera actualiser les coordonnées.
/// @return Si l'actualisation est un succès.
extern bool SDL_Interface_actualiserCoup(SDL_Interface *itf);

/// @brief Fonction qui démarre la SDL.
/// @param titre Nom de la fenêtre.
/// @param hauteur Hauteur de la fenêtre.
/// @param largeur Taille de la fenêtre.
extern SDL_Interface *startSDL(char *titre, int hauteur, int largeur);

#endif