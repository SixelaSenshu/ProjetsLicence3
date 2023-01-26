#include "../headers/listContact.h"

ArrayListContact * createArrayListContact(){
    ArrayListContact * new_list = NULL;
    Contact ** listContact = NULL;

    new_list = (ArrayListContact *)malloc(sizeof(ArrayListContact));
    if(!new_list){
        ERROR_EXIT("Erreur d'allocation de la mémoire pour la struct ArrayListContact",EXIT_FAILURE);
    }
    new_list->size = 0;
    new_list->capacity = BLOCK_ARRAYLISTCONTACT_SIZE;
    listContact = (Contact **)malloc(sizeof(Contact*)*BLOCK_ARRAYLISTCONTACT_SIZE);
    if(!listContact){
        ERROR_EXIT("Erreur d'allocation de la mémoire pour la liste de la struct ArrayListContact",EXIT_FAILURE);
    }
    new_list->users = listContact;
    for(int i =0; i < new_list->capacity ;i++){
        new_list->users[i] = NULL;
    }
    new_list->components = NULL;
    new_list->nbComponente = 0;
    new_list->componentsStrong = NULL;
    new_list->nbComponenteStrong = 0;
    new_list->modeComponetsStrong = 0;
    return new_list;
}

bool addArrayListContact(Contact * contact, ArrayListContact * list){
    if(list->size < list->capacity){
        list->users[list->size++] = contact;
        return true;
    }else{
        ArrayListContact * listtmp = realloc(listtmp, list->capacity + BLOCK_ARRAYLISTCONTACT_SIZE);
        if(listtmp == NULL){
            ERROR_EXIT("Erreur de réallocation de mémoire pour la list de contacts",EXIT_FAILURE);
        }
        list = listtmp;
        list->users[list->size++] = contact;
        return true;    
    }
    return false;
}

Contact * getContact(ArrayListContact * list, char * name, char * ville){
    assert(name && ville && list);
    int res, res2, i;
    for(i = 0; i < list->size; i++){
        res = strcmp(list->users[i]->name,name);
        res2 = strcmp(list->users[i]->town,ville);
        if(!res && !res2){
            return list->users[i];
        }
    }
    return NULL;
}

void toStringArrayListContact(ArrayListContact * list){
    for(int i = 0; i < list->size ; i++){
        printf("[%d]", i);
        toStringContact(list->users[i]);
    }
    printf("\n");
}

void destroyArrayListContact(ArrayListContact * list){
    assert(list && list->users);
    int i; 
    for( i = 0; i < list->size; i++){
        destroyLinkedList(list->users[i]->subscriptions);
        destroyLinkedList(list->users[i]->followers);
    }
    for( i = 0; i < list->size; i++){
        destroyContact(list->users[i]);
    }
    if(list->components)free(list->components);
    if(list->componentsStrong)free(list->componentsStrong);
    if(list->users)free(list->users);
    if(list)free(list);
}

void symmetrizerHashmapConnection(ArrayListContact * contacts){
    Contact * contact = NULL;
    Contact * contactCmp = NULL;
    Link * subscription = NULL;
    for(int i = 0; i < contacts->size;i++){
        contact = contacts->users[i];
        for(int j = 0 ; j < contacts->size;j++){
            contactCmp = contacts->users[j];
            if(contact != contactCmp){
                subscription = contactCmp->subscriptions->root;
                while(subscription){
                    if(contact == subscription->contact){
                        addFollower(contact,contactCmp);
                    }
                    subscription = subscription->next;
                }
            }
        }
    }
}

////////////////////////////////////////////// Algorithme pour composantes connexes 

void createComponent(ArrayListContact * contacts, Contact * contact , int number){
    Link * followers = contact->followers->root;
    Link * subscribers = contact->subscriptions->root;
    int i;
    while(followers){
        for(i = 0; i < contacts->size; i++){
            if(followers->contact == contacts->users[i] && contacts->components[i] == 0){
                contacts->components[i] = number;
                createComponent(contacts,followers->contact,number);
            }
        }
        followers = followers->next;
    }
    while(subscribers){
         for(i = 0; i < contacts->size; i++){
            if(subscribers->contact == contacts->users[i] && contacts->components[i] == 0){
                contacts->components[i] = number;
                createComponent(contacts,subscribers->contact,number);
            }
        }
        subscribers = subscribers->next;
    }
}

void createComponents(ArrayListContact * contacts){
    unsigned nb = 0;
    if(contacts->components == NULL){
        contacts->components = (int *)calloc(sizeof(int ),contacts->size);
        if(!contacts->components){
            ERROR_EXIT("Erreur d'allocation pour les composantes connexes",EXIT_FAILURE);
        }
    }
    for(int i = 0; i < contacts->size;i++){
        if(contacts->components[i] == 0){
            nb++;
            createComponent(contacts,contacts->users[i],nb);
        }
    }
    contacts->nbComponente = nb;
}

void toStringComponent(ArrayListContact * contacts, int number){
    if(contacts->components == NULL){
        printf("Il n'y aucune composantes connexes");
    }
    int i,j;
    Link * followers = NULL;
    Link * subscribers = NULL;
    for(i = 0 ; i < contacts->size; i++){
        if(contacts->components[i] == number){
            printf("--------------------\n");
            printf("[%d]:%s\n",i, contacts->users[i]->name);
            printf("|_;>\n");
            followers = contacts->users[i]->followers->root;
            subscribers = contacts->users[i]->subscriptions->root;
            while(followers){   
                for(j = 0; j <contacts->size;j++){
                    if(followers->contact == contacts->users[j] && contacts->components[j] == number){
                        printf("Abonné à : %s, habitant à %s \n", followers->contact->name, followers->contact->town);
                    }
                }
                followers=followers->next;
            }
            while(subscribers){   
                for(j = 0; j <contacts->size;j++){
                    if(subscribers->contact == contacts->users[j] && contacts->components[j] == number){
                        printf("Suivi par : %s, habitant à %s \n", subscribers->contact->name, subscribers->contact->town);
                    }
                }
                subscribers=subscribers->next;
            }
        }
    }        
}

void toStringComponents(ArrayListContact * contact){
    if(contact->components == NULL){
        printf("Il n'y aucune composantes connexes");
    }
    for(int i = 0; i < contact->nbComponente ;i++){
        printf("\n|------------------------------------------------------");
        printf("Composante connexe n°%d\n",i+1);
        toStringComponent(contact,i+1);
    }
}


////////////////////////////////////////////// Algorithme pour composantes connexes fortes

static void resetComponentsStrong(ArrayListContact* contact){
    for(int i = 0; i < contact->size; i++){
        contact->componentsStrong[i]=0;
    }
    contact->nbComponenteStrong = 0;
}

void createComponentStrong(ArrayListContact * contacts, Contact * contact , int number, int * resSub, int * resfollow){
    Link * subscribers = contact->subscriptions->root;
    Link * followers = contact->subscriptions->root;
    int * subscribersRes = resSub;
    int * followersRes = resfollow;
    int i;
    while(subscribers){
         for(i = 0; i < contacts->size; i++){
            if(subscribers->contact == contacts->users[i] && subscribersRes[i] == 0){
                subscribersRes[i]=1;
                createComponentStrong(contacts,subscribers->contact,number,subscribersRes,followersRes);
            }
        }
        subscribers = subscribers->next;
    }
    while(followers){
         for(i = 0; i < contacts->size; i++){
            if(followers->contact == contacts->users[i] && followersRes[i] == 0){
                followersRes[i]=1;
                createComponentStrong(contacts,followers->contact,number,subscribersRes,followersRes);
            }
        }
        followers = followers->next;
    }
    for(i=0;i<contacts->size;i++){
        if(subscribersRes[i] >= 1 && followersRes[i] >=1 && contacts->componentsStrong[i] == 0){
            contacts->componentsStrong[i]=number;
        }
    }
}

void createComponentsStrong(ArrayListContact * contacts){
    unsigned nb = 0;
    int * res = (int *)calloc(sizeof(int),contacts->size);
    int * res2 = (int *)calloc(sizeof(int),contacts->size);
    if(contacts->componentsStrong == NULL){
        contacts->componentsStrong = (int *)calloc(sizeof(int),contacts->size);
        if(!contacts->componentsStrong){
            ERROR_EXIT("Erreur d'allocation pour les composantes connexes",EXIT_FAILURE);
        }
    }else{
        resetComponentsStrong(contacts);
    }
    contacts->modeComponetsStrong = 1;
    for(int i = 0; i < contacts->size;i++){
        if(contacts->componentsStrong[i] == 0){
            nb++;
            createComponentStrong(contacts,contacts->users[i],nb,res,res2);
            for(int j = 0; j < contacts->size; j++){
                // remise à zéro
                res[j] = 0;
                res2[j] = 0;
            }
            if(contacts->componentsStrong[i] == 0){
                contacts->componentsStrong[i] = nb;
            }
        }
    }
    
    contacts->nbComponenteStrong = nb;
    free(res);
    free(res2);
}

void toStringComponentStrong(ArrayListContact * contacts, int number){
    if(contacts->componentsStrong == NULL){
        printf("Il n'y aucune composantes connexes");
        return;
    }
    int i,j;
    Link * subscribers = NULL;
    for(i = 0 ; i < contacts->size; i++){
        if(contacts->componentsStrong[i] == number){
            printf("--------------------\n");
            printf("[%d]:%s\n",i, contacts->users[i]->name);
            printf("|_;>\n");
            subscribers = contacts->users[i]->subscriptions->root;
            
            while(subscribers){   
                for(j = 0; j <contacts->size;j++){
                    if(subscribers->contact == contacts->users[j] && contacts->componentsStrong[j] == number){
                        printf("Suivi par : %s, habitant à %s \n", subscribers->contact->name, subscribers->contact->town);
                    }
                }
                subscribers=subscribers->next;
            }
        }
    }        
}

void toStringComponentsStrong(ArrayListContact * contact){
    if(contact->componentsStrong == NULL){
        printf("Il n'y aucune composantes connexes");
        return;
    }
    if(contact->modeComponetsStrong != 1){
        printf("Aucune composantes créer ou générer à partir de Tarjan\n");
        return;
    }
    for(int i = 0; i < contact->nbComponenteStrong ;i++){
        printf("\n|------------------------------------------------------");
        printf("Composante connexe forte n°%d\n",i+1);
        toStringComponentStrong(contact,i+1);
    }
}

////////////////////////////////////////////// Algorithme de tarjan 

static unsigned int minCurrentLinkrank(unsigned int valueFollower , unsigned int valueCurrent){
    if(valueFollower < valueCurrent)return valueFollower;
    else return valueCurrent;
}

static unsigned int exploreSubscriberWithTarjan(Tarjan * info){
    //////////// variables 
    unsigned int currentRank ,currentLinkrank, pos, res, rank , nextLinkRank;
    StackContact * p2;
    Link * subscribers = NULL;
    Contact * currentCmp = NULL;
    Contact * getByStack = NULL;
    //////////// Gestion des variables 

    push(info->stackContact,info->currentContact,info->rank+1);
    info->hasviews[info->size_hasviews++] = info->currentContact;
    info->rank++;

    currentRank = info->rank;
    currentLinkrank = info->rank;
    subscribers = info->currentContact->subscriptions->root;
 
    while(subscribers && subscribers->contact){
        res = 0;
        if(info->size_hasviews != 0){     
            for(pos = 0; pos < info->size_hasviews;pos++){
                if(info->hasviews[pos] == subscribers->contact){
                    res = 1;
                }
            }
        }
        if(res == 0){
            info->currentContact = subscribers->contact;
            nextLinkRank = exploreSubscriberWithTarjan(info);
            currentLinkrank = minCurrentLinkrank(currentLinkrank,nextLinkRank);
        }
        else if(res == 1){
            p2 = createStackContact();
            getByStack = top(info->stackContact);
            while(getByStack != NULL){
                rank = topRank(info->stackContact);
                getByStack = pop(info->stackContact);
                if(!getByStack)break;
                if(subscribers->contact == getByStack){
                    currentLinkrank = minCurrentLinkrank(currentLinkrank,rank);
                    push(p2,getByStack,rank); 
                    break;
                }
                push(p2,getByStack,rank); 
            }
            while(top(p2)){
                rank = topRank(p2);
                getByStack = pop(p2);
                push(info->stackContact,getByStack,rank);
            }
            destroyStackContact(p2);
        }
        subscribers = subscribers->next;
    }
    if(currentRank == currentLinkrank){
        info->numberComponent++;
        currentCmp = top(info->stackContact);
        while(currentCmp){
            currentCmp = pop(info->stackContact);
            for(int  i = 0; i < info->contacts->size;i++){
                if(info->contacts->users[i] == currentCmp){
                    info->contacts->componentsStrong[i] = info->numberComponent;
                    break;
                }
            } 
        }    
    }
    return currentLinkrank;
}

static void createComponentsStrongWithTarjan(ArrayListContact * contact){
    //////////// Variables
    Tarjan * tarjan = (Tarjan *)malloc(sizeof(Tarjan));
    unsigned int pos = 0 ,res = 0;
    Contact * currentCmp = NULL;
    
    //////////// Initialisation des variables pour l'aglo de tarjan
    tarjan->hasviews = (Contact **)malloc(sizeof(Contact *) * contact->size);
    for(int j = 0; j < contact->size; j++) tarjan->hasviews[j] = NULL;
    tarjan->size_hasviews=0;

    tarjan->numberComponent = 0 ;
    tarjan->stackContact = createStackContact();

    tarjan->contacts=contact;
    tarjan->currentContact = NULL;
    //////////// Algorithme
    while(tarjan->size_hasviews < contact->size){
        tarjan->rank = 0;
        tarjan->currentContact = contact->users[pos];
        res = 0;
        for(int i = 0; i < tarjan->size_hasviews;i++)if(tarjan->currentContact == tarjan->hasviews[i])res = 1;
        if(res == 0){exploreSubscriberWithTarjan(tarjan);}
        pos++;
    }
    destroyStackContact(tarjan->stackContact);
    free(tarjan->hasviews);
    free(tarjan);
}

void createComponentsStrongTarjan(ArrayListContact * contacts){
    if(contacts->componentsStrong == NULL){
        contacts->componentsStrong = (int *)calloc(sizeof(int),contacts->size);
        if(!contacts->componentsStrong){
            ERROR_EXIT("Erreur d'allocation pour les composantes connexes",EXIT_FAILURE);
        }
    }else resetComponentsStrong(contacts);
    contacts->modeComponetsStrong = 2;
    createComponentsStrongWithTarjan(contacts);
}

void toStringComponentsStrongTarjan(ArrayListContact * contacts){
    if(contacts->modeComponetsStrong != 2){
        printf("Aucune composantes créer ou générer à partir de la méthode simple\n");
        return;
    }
    if(contacts->componentsStrong == NULL){
        printf("Il n'y aucune composantes connexes");
        return;
    }
    /// recherche du nombre exact de composantes
    for(int j = 0; j < contacts->size; j++){
        if(contacts->componentsStrong[j] > contacts->nbComponenteStrong){
            contacts->nbComponenteStrong = contacts->componentsStrong[j];
        }
    }
    for(int i = 0; i < contacts->nbComponenteStrong ;i++){
        printf("\n|------------------------------------------------------");
        printf("Composante connexe forte via Tarjan n°%d\n",i+1);
        toStringComponentStrong(contacts,i+1);
    }
}



