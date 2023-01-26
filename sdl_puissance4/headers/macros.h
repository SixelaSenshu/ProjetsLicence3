/**
 * @file macros.h
 * @author Alexis Bernard & William Dubois
 * @brief Fichier qui permet de générer les erreurs et de les gérer pour le puissance4
 * @date 31.12.202
 * @version 1.0
 */
#ifndef _MACROS_H_
#define _MACROS_H_

#include <stdio.h>

/// @brief Gestion des erreurs du programme
typedef enum err
{
    ERR_NO_ERROR,         //!< No error
    ERR_INVALID_POINTER,  //!< Un paramètre entré ne possède pas de pointeur et n'est donc pas alloué
    ERR_ERROR_ALLOCATION, //!< Erreur d'allocation pour une adresse mémoire
    ERR_INVALID_OPTION,   //!< Option/s invalide/s saisie/s
    ERR_FULL,             //!< Capacité d'élement maximal atteint
    ERR_MODIF,            //!< Modification d'un élement non réussi
    ERR_ERROR_INIT_SDL,   //!< Problème d'allocation de mémoire pour une composante de la SDL
    ERR_ERROR_SDL,        //!< Problème avec la SDL
    ERR_ERROR_SDL_BREAK,  //!< Problème avec la SDL qui cause un arrêt
    ERR_EVENT_SDL         //!< Problème d'évenement de la SDL
} Erreur;

/// @brief Retourne l'erreur actuelle
/// @return Retourne l'etat du programme
extern enum err getErreur();

///  @brief Affiche l'erreur actuelle à l'ecran
extern void afficherErreur();

/// @brief Met à jour l'erreur
/// @param err : Erreur qui est générer.
/// @warning Le paramètre d'erreur peut être différent , mais la precision ne sera pas la même.
extern void setErreur(enum err err);

#endif