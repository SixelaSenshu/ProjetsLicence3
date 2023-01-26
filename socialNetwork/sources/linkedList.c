#include "../headers/linkedList.h"

LinkedList * createLinkedList(){
     LinkedList * new_linkedlist = NULL;
     new_linkedlist = (LinkedList *)malloc(sizeof(LinkedList));
     if(!new_linkedlist){
         ERROR_EXIT("Erreur d'allocation sur la LinkedList",EXIT_FAILURE);
     }
     new_linkedlist->root = (Link *)malloc(sizeof(Link));
     if(!new_linkedlist->root){
         ERROR_EXIT("Erreur d'allocation sur la racine de la LinkedList",EXIT_FAILURE);
     }
     new_linkedlist->root->contact=NULL;
     new_linkedlist->root->next=NULL;
     return new_linkedlist;
}

Link * createLink(){
    Link * new_link = NULL;
    new_link = (Link *)malloc(sizeof(Link));
    if(!new_link){
        ERROR_EXIT("Erreur d'allocation pour un element de la liste chainée", EXIT_FAILURE);
    }
    new_link->next = NULL;
    new_link->contact = NULL;
    return new_link;
}

bool addLinkedList(LinkedList * list, Contact * contact){
    assert(list);
    Link * current = list->root;
    Link * previous = list->root;
    if(list->root->contact == NULL){
        list->root->contact = contact;
        return true;
    }
    while(current->next){
        if(current->contact && current->contact == contact) return false;
        previous = current;
        current = current->next;
    }
    current->next = createLink();
    current->next->contact = contact;
    return true;
}

bool deleteLinkedList(LinkedList * list, Contact * contact){
    Link * current = list->root;
    Link * previous = NULL;
    if(contact == list->root->contact){
        Link * tmp = list->root;
        free(tmp);
        list->root=list->root->next;
        return true;
    }
    previous = current;
    current = current->next;
    while(current){
        if(contact == current->contact){
            Link * tmp = current->next;
            free(current);
            previous->next = tmp;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}


void toStringLinkedList(LinkedList * list, char* type ){
    LinkedList * l = list;
    Link * current = NULL;
    printf("%s:{",type);
    if(l){
        if(!l->root);
        else{
            current = l->root;
            while(current != NULL && current->contact != NULL){
                printf("%s",current->contact->name);
                if(current->next && current->next->contact) printf(", ");
                current = current->next;
            }
        }
        printf("}\n");
    }
    
}

void destroyLinkedList(LinkedList * list){
    assert(list && list->root);
    Link * current = NULL;
    Link * previous = NULL;
    current=list->root;
    while(current){
        previous = current;
        current = current->next;
        if(previous)free(previous);
    }
    if(current)free(current);
    if(list)free(list);
}