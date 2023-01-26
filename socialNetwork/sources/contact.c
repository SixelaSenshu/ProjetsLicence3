#include "../headers/contact.h"

Contact * createContact(char * name, char * town){
    Contact * contact = (Contact *)malloc(sizeof(Contact));
    LinkedList * list;
    LinkedList * listfollowers;
    if(!contact){
        ERROR_EXIT("Erreur d'allocation sur struct Contact",EXIT_FAILURE);
    }
    contact->name = (char *)malloc(sizeof(char)*CONTACT_MAX_SIZE_NAME);
    if(!contact->name){
        ERROR_EXIT("Erreur d'allocation sur le nom de struct Contact",EXIT_FAILURE);
    }
    contact->name = name;
    contact->town = (char *)malloc(sizeof(char)*CONTACT_MAX_SIZE_TOWN);
    if(!contact->town){
        ERROR_EXIT("Erreur d'allocation sur la ville de struct Contact",EXIT_FAILURE);
    }
    contact->town = town;
    list = createLinkedList();
    if(!list){
        ERROR_EXIT("Erreur d'allocation sur la liste chainée de struct Contact",EXIT_FAILURE);
    }
    contact->subscriptions = list;
    listfollowers = createLinkedList();
    if(!listfollowers){
        ERROR_EXIT("Erreur d'allocation sur la liste chainée de struct Contact",EXIT_FAILURE);
    }
    contact->followers = listfollowers;
    return contact;
}

bool addSubscriber(Contact * receiver, Contact * follower){
    return addLinkedList(follower->subscriptions,receiver);
}

bool addFollower(Contact * receiver, Contact * follower){
    return addLinkedList(receiver->followers,follower);
}

bool deleteSubscriber(Contact * receiver, Contact * follower){
    return deleteLinkedList(follower->subscriptions,receiver);   
}

bool deleteFollower(Contact * receiver, Contact * follower){
    return deleteLinkedList(receiver->followers,follower);   
}

void destroyContact(Contact * contact){
    assert(contact);
    //if(contact->name != NULL)free(contact->name);
    if(contact->town != NULL)free(contact->town);
    if(contact != NULL )free(contact);
}

void toStringContact(Contact * contact){
    if(contact){
        printf("|------------------------------------\n");
        printf("Name = %s\n",contact->name);
        printf("Ville = %s\n",contact->town);
        toStringLinkedList(contact->subscriptions,"Subscriptions");
        toStringLinkedList(contact->followers,"Followers");
    }
}

