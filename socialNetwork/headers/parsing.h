#ifndef _READWRITTER_H_
#define _READWRITTER_H_ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "contact.h"
#include "linkedList.h"
#include "cities.h"
#include "listContact.h"

typedef struct _readerbuffer Reader;
    
struct _readerbuffer{
    ArrayListContact * contacts;
    Cities * cities;
    FILE * file;
    char * buffer; 
    unsigned buffer_capacity;
    unsigned buffer_size;
};

/// @brief Creer un lecteur de fichier.
/// @return Retourne le pointeur de celui-ci.
Reader * createReader();

/// @brief Detruit le lecteur de fichier.
/// @param reader Le lecteur qui est détruit.
void destroyReader(Reader * reader);

/// @brief Permet de lire une ligen dans un fichier et de créer un buffer.
/// @param reader Le lecteur qui analyse le fichier.
/// @return Si le bufffer n'est pas null.
bool readLine(Reader * reader);

/// @brief Permet de lire un fichier et met à jour une map et une liste de contact (Obligatoirement à vide).
/// @param namefile Nom du fichier qui sera en ouverture.
/// @param mode Mode d'ouverture du fichier.
/// @param reader Le lecteur qui analyse le fichier/
/// @return Si la lecture est un succes;
bool readFile(char * namefile, char * mode, Reader * reader);

/// @brief Permet de générer une liste de contact ( sans les villes )
/// @param file Fichier que l'on lit
/// @param reader Le lecteur qui analyse le fichier.
void parsingContacts(Reader *reader);

/// @brief Permet de générer une liste de ville.
/// @param reader Le lecteur qui analyse le fichier.
/// @return La liste des villes.
char ** parsingCities(Reader *reader);

/// @brief Permet de générer une liste des abonnements et la ville pour chaque personne. 
/// @param reader Le lecteur qui analyse le fichier.
/// @return La ville de la personne;
char * parsingSubscriptions(Reader *reader);

/// @brief Permet de générer une matrices de villes et les distances.
/// @param list Liste des villes parser avant
/// @param reader Le lecteur qui analyse le fichier.
void parsingDistances(char ** list, Reader *reader);

#endif