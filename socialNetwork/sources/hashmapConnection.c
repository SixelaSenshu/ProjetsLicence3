#include "../headers/hashmapConnection.h"

HashmapConnection * createHashmapConnection(ArrayListContact * map){
    HashmapConnection * new_map = NULL;
    int i, j; 
    new_map = (HashmapConnection *)malloc(sizeof(HashmapConnection));
    if(map){
        if(!new_map){
            ERROR_EXIT("Erreur d'allocation de la mémoire pour la matrice de distance",EXIT_FAILURE);
        }
        new_map->listContact = map;
        new_map->relation_size = map->size;
        new_map->relation_capacity = map->capacity;
        new_map->relationships = (int **)malloc(sizeof(int *) * new_map->relation_capacity);
        if(!new_map->relationships){
            ERROR_EXIT("Erreur d'allocation de la mémoire pour la matrice de la struct HashmapConnection",EXIT_FAILURE);
        } 
        for( i = 0; i < new_map->relation_capacity; i++){
            new_map->relationships[i] = (int *)malloc(sizeof(int) * new_map->relation_capacity);
            if(!new_map->relationships[i]){
                ERROR_EXIT("Erreur d'allocation de la mémoire pour la sous-matrice de la struct HashmapConnection",EXIT_FAILURE);
            } 
        }
        for( i = 0; i < new_map->relation_capacity; i++ ){
            for(j = 0; j < new_map->relation_capacity; j++){
                if(i != j) new_map->relationships[i][j] = -1;
                else new_map->relationships[i][j] = 0;
            }
        }
    }
    return new_map;
}

int updateHashmapConnectionContact(Contact* contact ,ArrayListContact * list, unsigned ind, int * listchecked){
    Link * followers = contact->subscriptions->root;
    int * next = (int *)calloc(sizeof(int),list->capacity);
    int i = 0;
    while(followers != NULL){
        for(i = 0; i < list->size; i++){
            if(list->users[i] == followers->contact){
                next[i] = 1;
            }
        }
        followers = followers->next;
    }
    for(i = 0; i < list->size; i++){
        if(listchecked[i] == -1 && next[i] != 0){
            listchecked[i] = ind;
            updateHashmapConnectionContact(list->users[i],list,ind+1,listchecked);
        }
    }
    return 1;
}  

void updateHashmapConnection(HashmapConnection * map){
    int * tab;
    Contact * contact = NULL;
    for(int i = 0; i < map->listContact->size; i++){
        tab = (int *)calloc(sizeof(int),map->listContact->size);
        for(int j = 0; j < map->listContact->size; j++){
            tab[j] = -1;
        }
        tab[i] = 0;
        contact = map->listContact->users[i];
        updateHashmapConnectionContact(contact,map->listContact, 1,tab);
        map->relationships[i]=tab;
    }
}

void destroyHashmapConnection(HashmapConnection * map){
    for(int i=0; i < map->relation_size; i++){
        free(map->relationships[i]);
    }
    free(map->relationships);
    free(map);
}

void toStringHashmapConnection(HashmapConnection * map){
    printf("     ");
    for(int i = 0; i < map->relation_size; i++){
        printf("| %s |", map->listContact->users[i]->name); 
    }
    printf("|\n");
    for(int j = 0 ; j < map->relation_size; j++){
        printf("------------------------------------------------------------------------------------------------------\n");
        printf("  [%d]  ",j);
        for(int k = 0; k < map->relation_size ; k++){
            printf("|   %d   |",map->relationships[j][k]);
        }
        printf("|\n");
    }
}


int getBetweenDistanceContacts(Contact * contact, Contact * contact2,HashmapConnection * map){
    Contact ** current =  map->listContact->users;
    int ind1 = -1, ind2 = -1;
    for(int i = 0; i < map->listContact->size ;i++){
        if(!strcmp(current[i]->name, contact->name)){
            ind1 = i; 
        }
        if(!strcmp(current[i]->name, contact2->name)){
            ind2 = i; 
        }
    }
    if(ind1 == -1 || ind2 == -1){
        return -1;
    }else{
        return map->relationships[ind1][ind2];
    }
}

float getAverageDistanceContacts(HashmapConnection * map){
    float sum = 0;
    int nb = 0;
    int res=0;
    for(int i = 0; i < map->listContact->size;i++){
        for(int j = 0; j< map->listContact->size;j++){
            if(j != i){
                res = getBetweenDistanceContacts(map->listContact->users[i],map->listContact->users[j],map);
                if(res != -1){
                    sum += res;
                    nb++;
                }
            }   
        }
    }
    return (float)(sum/nb);
}
