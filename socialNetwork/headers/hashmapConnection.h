#ifndef _HASHMAPCONNECTION_H_
#define _HASHMAPCONNECTION_H_ 1

#include <stdlib.h>
#include <stdio.h>

#include "contact.h"
#include "listContact.h"
#include "linkedList.h"
#include "macros.h"
#include "stack.h"

typedef struct _mapContact HashmapConnection;
struct _mapContact{
    ArrayListContact * listContact;
    int ** relationships;
    unsigned int relation_size;
    unsigned int relation_capacity;
};

/// @brief Créer une matrice de distance pour une liste de contact.
/// @param map Liste des contacts.
/// @return La nouvelle matrice de distance.
HashmapConnection * createHashmapConnection(ArrayListContact * map);

/// @brief Créer une liste de distance avec un contact.
/// @param contact Contact sur lequel on calcul les distances.
/// @param list Liste de contact qui garde les pointeurs.
/// @param ind Indice ou est le contact dans la liste
/// @param listchecked Liste des contacts déja testés.
/// @return l'indice de distance.
int updateHashmapConnectionContact(Contact* contact, ArrayListContact * list, unsigned ind , int * listchecked );

/// @brief met à jour une matrice de distance pour une liste de contact.
/// @param map La matrice de distance.
void updateHashmapConnection(HashmapConnection * map);

/// @brief désalloue une matrice de distance pour une liste de contact.
/// @param map La matrice de distance.
void destroyHashmapConnection(HashmapConnection * map);

/// @brief affiche une matrice de contact
/// @param map La matrice de distance
void toStringHashmapConnection(HashmapConnection * map);

/// @brief Calcule la distance entre deux personnes.
/// @param contact Contact numéro 1.
/// @param contact2 Contact numéro 2.
/// @param map la liste des distances.
/// @return la distance entre les 2 contacts.
int getBetweenDistanceContacts(Contact * contact, Contact * contact2, HashmapConnection * map);

/// @brief Calcule la moyenne des distances entre les personnes ( sauf ceux n'etant pas en lien ou eux même )
/// @param map la liste des distances.
/// @return la distance en moyenne.
float getAverageDistanceContacts(HashmapConnection * map);

#endif