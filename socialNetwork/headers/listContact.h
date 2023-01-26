#ifndef _ARRAYLISTCONTACT_H_
#define _ARRAYLISTCONTACT_H_ 1

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "stack.h"
#include "contact.h"
#include "linkedList.h"
#include "macros.h"

#define BLOCK_ARRAYLISTCONTACT_SIZE 30

typedef struct _listContact{
    Contact ** users;
    int * components;
    int * componentsStrong;
    unsigned int nbComponente;
    unsigned int nbComponenteStrong;
    unsigned int capacity, size;
    unsigned int modeComponetsStrong;
}ArrayListContact;

typedef struct tarjan{
    ArrayListContact * contacts;
    Contact ** hasviews;
    Contact * currentContact;
    StackContact * stackContact;
    unsigned int size_hasviews,rank,numberComponent;
}Tarjan;

/// @brief Creer une liste de contact.
/// @return La nouvelle list.
ArrayListContact * createArrayListContact();

/// @brief Ajoute un contact à la liste des pointeurs.
/// @param contact Contact que l'on veut ajouter.
/// @param list Liste de contact qui garde les pointeurs.
/// @return Si l'ajout est effectuer
bool addArrayListContact(Contact * contact, ArrayListContact * list);

/// @brief Retourne un contact parmis la liste si il existe.
/// @param list Liste de contact qui contient les contacts.
/// @param name Nom de la personne que l'on souhaite chercher.
/// @param ville Ville de la personne que l'on souhaite chercher.
/// @return Le contact si il existe dans la liste.
Contact * getContact(ArrayListContact * list, char * name, char * ville);

/// @brief Affichage de tous les contact de la liste.
/// @param list Liste de contact qui garde les pointeurs.
void toStringArrayListContact(ArrayListContact * list);

/// @brief Désallocation de la liste de contact.
/// @param list Liste de contact qui garde les pointeurs.
void destroyArrayListContact(ArrayListContact * list);

/// @brief Met à jour le champ suivi_par de toutes les personnes d'un graphe.
/// @param contacts Liste de contact 
void symmetrizerHashmapConnection(ArrayListContact * contacts);

/// @brief Calcule une composante connexe sur les noeuds qui ne sont pas déjà 
/// rattachés et leur attribue num_compo comme numéro de composante.
/// @param contact Le contact en question
/// @param contacts La liste de contacts en question
/// @param number numéro de la composante
void createComponent(ArrayListContact * contacts, Contact * contact, int number);

/// @brief  Calcule_les_composantes_connexes() qui calcule toutes les composantes 
/// connexes du graphe.Le tableau tab_composantes doit être initialisé à -1.
/// @param contacts La liste de contact en question
void createComponents(ArrayListContact * contacts);

/// @brief Affiche tous les noeuds de la composante numéro num_compo.
/// Pour chaque personne, on veut connaître son nom, sa ville, les noms 
/// des personnes à qui elle est abonnée et les noms des personnes qui la suivent.
/// @param contacts La liste de contact en question
/// @param number numéro de la composante
void toStringComponent(ArrayListContact * contacts, int number);

/// @brief Affiche toutes les composantes.
/// @param contacts La liste de contact en question
void toStringComponents(ArrayListContact * contact);

/// @brief Calcule une composante fortement connexe sur les noeuds qui ne sont pas
/// déjà rattachés et leur attribue num_compo comme numéro de composante.
/// @param contact Le contact en question
/// @param contacts La liste de contacts en question
/// @param number numéro de la composante
/// @param res Liste de personne dont ils sont accesible à partir du contact
void createComponentStrong(ArrayListContact * contacts, Contact * contact, int number, int * resSub, int * resfollow);

/// @brief alcule toutes les composantes fortement connexes du graphe.
/// Le tableau tab_composantes doit être initialisé à -1.
/// @param contacts La liste de contact en question
void createComponentsStrong(ArrayListContact * contacts);

/// @brief Affiche tous les noeuds de la composante numéro num_compo.
/// Pour chaque personne, on veut connaître son nom, sa ville, les noms 
/// des personnes à qui elle est abonnée et les noms des personnes qui la suivent.
/// @param contacts La liste de contact en question
/// @param number numéro de la composante
void toStringComponentStrong(ArrayListContact * contacts, int number);

/// @brief Affiche toutes les composantes.
/// @param contacts La liste de contact en question
void toStringComponentsStrong(ArrayListContact * contact);

/// @brief Créer les composantes à partir de deux fonctions static :
/// - createComponentsStrongWithTarjan : Créer les composantes connexes
/// - exploreSubscriberWithTarjan : Explore les abonnements de chaque personne
/// @param contact La liste de contact.
void createComponentsStrongTarjan(ArrayListContact * contact);

/// @brief Affiche les composantes (uniquement si générer avec l'algorithm de tarjan)
/// @param contact La liste de contact.
void toStringComponentsStrongTarjan(ArrayListContact * contacts);

#endif
