#ifndef _CONTACT_H_
#define _CONTACT_H_ 1


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "macros.h"
#include "linkedList.h"

#define CONTACT_MAX_SIZE_NAME 50
#define CONTACT_MAX_SIZE_TOWN 40
#define CONTACT_FOLLOW_BLOCK_SIZE_ 10

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

/// @brief Alloue de la mémoire pour un Contact.
/// @param name Nom qui sera défini pour le contact.
/// @param town Ville qui sera défini pour le contact.
/// @return Retourn le nouveau Contact fraichement créer.
Contact * createContact(char * name, char * town);

/// @brief Ajoute en abonnement le receiver pour follower.
/// @param receiver Le contact sur lequel le follower sera abonné.
/// @param follower Le contact qui va suivre le un autre contact.
/// @return Si l'ajout est un succès.
bool addSubscriber(Contact * receiver ,Contact * follower);

/// @brief Ajoute un follower pour le receiver.
/// @param receiver Le contact sur lequel le follower sera abonné.
/// @param follower Le contact qui va suivre le un autre contact.
/// @return Si l'ajout est un succès.
bool addFollower(Contact * receiver, Contact * follower);

/// @brief Enleve en abonnement le receiver pour follower.
/// @param receiver Le contact sur lequel le follower sera désabonné.
/// @param follower Le contact qui ne suivra plus le un autre contact.
/// @return Si la suppresion est un succès.
bool deleteSubscriber(Contact * receiver, Contact * follower);

/// @brief Enleve en abonnement le receiver pour follower.
/// @param receiver Le contact sur lequel le follower sera désabonné.
/// @param follower Le contact qui ne suivra plus le un autre contact.
/// @return Si la suppresion est un succès.
bool deleteFollower(Contact * receiver, Contact * follower);

/// @brief Désalloue la mémoire pour un contact.
/// @param contact Le contact qui sera supprimé de la mémoire
void destroyContact(Contact * contact);

/// @brief Affiche le contact et tous ces abonnements.
/// @param contact Le contact qui sera affiché
void toStringContact(Contact * contact);

#endif