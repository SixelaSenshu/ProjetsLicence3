#ifndef _STACK_H_
#define _STACK_H_ 1

#include <stdio.h>
#include <stdlib.h>

#include "contact.h"

/// @brief Structure qui créer une pile de contact
typedef struct stack{
    unsigned capacity, size;
    Contact ** contact;
    unsigned int * ranks;
}StackContact;

/// @brief créer une pile de contact pour permettre de les stocker.
/// @return la nouvelle pile
StackContact * createStackContact();

/// @brief detruit une pile de contact
/// @param stack la pile en question
void destroyStackContact(StackContact * stack);

/// @brief affiche l'état d'une pile de contact 
/// @param stack la pile en question
void toStringStackContact(StackContact * stack);

/// @brief ajoute un contact à la pile
/// @param stack la pile en question
/// @param contact Le contact qui sera ajouter
/// @param rank Le rank du contact
void push(StackContact * stack,  Contact * contact, unsigned int rank);

/// @brief Check si la pile est vide
/// @param stack la pile en question*
/// @return Retourne vrai si oui
unsigned isEmptyStackContact(StackContact * stack);

/// @brief Retire un élement de la pile et le renvoie
/// @param stack la pile en question*
/// @return Le contact en question
Contact * pop(StackContact * stack);

/// @brief Donne le dernier élement de la pile
/// @param stack la pile en question*
/// @return Le contact en question
Contact * top(StackContact * stack);

/// @brief Donne le dernier rank de la pile
/// @param stack la pile en question*
/// @return Le rank en question
unsigned int topRank(StackContact * stack);

#endif