#ifndef _LINKEEDLIST_H_
#define _LINKEEDLIST_H_ 1

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "macros.h"
#include "contact.h"
#ifndef _CONTACT_STRUCT_
#define _CONTACT_STRUCT_ 1
typedef struct _contact Contact;
typedef struct _linkedlist LinkedList;
typedef struct _link Link;

struct _contact{
    char * name;
    char * town;
    LinkedList * subscriptions;
    LinkedList * followers;
};
struct _link{
    Contact * contact;
    Link * next;
};

struct _linkedlist{
    Link * root;
};
#endif

/// @brief Alloue de la mémoire pour une liste chainée.
/// @return Retourne la liste fraichement allouée.
LinkedList * createLinkedList();

/// @brief Alloue de la mémoire pour un noeud
/// @return Retourne le noeud fraichement allouée 
Link * createLink();

/// @brief Ajoute un maillon à la liste si le contact n'est pas présent dans la liste.
/// @param list Liste chainée qui subit l'ajout.
/// @param contact Contact qui sera ajouté dans la liste.
/// @return Retourne si l'ajout est un succès ou non.
bool addLinkedList(LinkedList * list, Contact * contact);


/// @brief Retire un element de la liste si il est présent dans celle-ci, sinon ne fais rien.
/// @param list Liste chainée qui subit la suppresion
/// @param contact Contact qui sera supprimé de la liste
/// @return  Retourne si la suppresion est un succès ou non.
bool deleteLinkedList(LinkedList * list, Contact * contact);

/// @brief Liste qui sera affiche sur la sortie standard.
/// @param list La liste que l'on affiche
/// @param type Type de la liste ( abonnements ou abonnés)
void toStringLinkedList(LinkedList * list,char * type);

/// @brief Liste qui sera désalloué de la mémoire ram.
/// @param list Liste qui est supprimé
void destroyLinkedList(LinkedList * list);

#endif