#ifndef _CITIES_H_
#define _CITIES_H_ 1


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "macros.h"
#include "contact.h"
#include "listContact.h"


#define CITIES_MAX_SIZE_NAME 60
#define BLOCK_CITIES_SIZE 30

typedef struct _Cities Cities;
struct _Cities{
    // Paramètres pour stocker les villes
    char ** name;
    unsigned int name_capacity;
    unsigned int name_size;

    // Paramètres pour stocker la matrices qui contient les distances
    float ** distance;
    unsigned int distance_capacity;
    unsigned int distance_size;
};

/// @brief Permet la création d'un réseaux de villes selon leurs distances.
/// @param size la taille de la liste
/// @return Le pointeur vers le nouveau réseau.
Cities * createCities(unsigned size);

/// @brief Ajout d'une lien entre deux villes.
/// @param dest La ville destinataire.
/// @param source La ville de départ.
/// @param dist La distance entre les deux villes.
/// @param map Le réseaux des villes.
/// @return Si l'ajout à été un succes ou non.
bool addCities(char * dest, char * source, float dist, Cities * map );

/// @brief Affiche l'état d'une carte.
/// @param map Le réseaux de la villes qui sera afficher.
void toStringCities(Cities * map);

/// @brief Permet de désallouer la mémoire pour la map.
/// @param map Le réseaux de villes qui sera désallouer.
void destroyCities(Cities * map);

/// @brief Retourne la distance entre 2 personne
/// @param c1 Contact 1.
/// @param c2 Contact 2.
/// @param map Le réseaux de villes.
/// @return Retourne la distance.
float getDistanceBetween(Contact * c1, Contact * c2, Cities * map);

/// @brief Retourne la distance en moyenne des gens.
/// @param list la liste des contacts.
/// @param map Le réseaux de villes.
/// @return Retourne la moyenne.
float getDistanceAverage(ArrayListContact * list, Cities * map);
#endif