#include "../headers/cities.h"

Cities * createCities(unsigned size){
    //--------------------------- Variables && Conditions 
    Cities * new_Cities = new_Cities = (Cities *)malloc(sizeof(Cities));
    char ** list_cities = NULL;
    char * elt_cities = NULL;
    float ** array_distance = NULL;
    float * list_float = NULL;

    //--------------------------- Allocationscde mémoires
    
    if(!new_Cities){
        ERROR_EXIT("Problème d'allocation pour la struct Cities",EXIT_FAILURE);
    }
    //--------------------------- Liste des villes
    new_Cities->name_size = 0;
    new_Cities->name_capacity = size;
    list_cities = (char **) malloc(sizeof(char *) * size);
    if(!list_cities){
        ERROR_EXIT("Problème d'allocation de liste des villes pour la struct Cities",EXIT_FAILURE);
    }   

    for(int i = 0; i < size; i++){
        elt_cities = (char *)malloc(sizeof(char)*60);
        if(!elt_cities){
            ERROR_EXIT("Problème d'allocation des élements de la liste des villes pour la struct Cities",EXIT_FAILURE);
        }   
        list_cities[i] = elt_cities;
    }
    new_Cities->name = list_cities;
    //--------------------------- Matrice
    // - On alloue la mémoire pour la liste de pointeur
    new_Cities->distance_size = 1;
    new_Cities->distance_capacity = size;
    array_distance = (float **)calloc(sizeof(float *),size);
    if(!array_distance){
        ERROR_EXIT("Problème d'allocation de la map pour la struct Cities",EXIT_FAILURE);
    }    

    for(int i = 0; i < new_Cities->distance_capacity; i++){
        //- Pour chaque pointeur , on vérifie que l'allocation est réussi
        list_float = (float *)calloc(sizeof(float),size);
        array_distance[i] = list_float;
        if(!array_distance[i]){
            ERROR_EXIT("Problème d'allocation des élements de la sous-map pour la struct Cities",EXIT_FAILURE);
        }
    }
    new_Cities->distance = array_distance;
    // -> On aura donc une matrice carré de distances
    return new_Cities;
}

bool addCities(char * dest, char * source, float dist, Cities * map){
    assert(map && dist > 0.0 && dest && source);
    int ind_dest = -1, ind_sources= -1, i;
    for( i = 0; i < map->name_size;i++) if(!strcmp(map->name[i],dest)) ind_dest =i;
    for( i = 0; i < map->name_size;i++) if(!strcmp(map->name[i],source)) ind_sources =i;
    if(ind_dest == -1){
        if(map->name_size < map->name_capacity){
            ind_dest = map->name_size;
            strcpy(map->name[map->name_size++],dest);
        }else{
            return false;
        }    
    }
    if(ind_sources == -1){
        if(map->name_size < map->name_capacity){
            ind_sources = map->name_size;
            strcpy(map->name[map->name_size++],source);
        }else{
            return false;
        } 
    }
    map->distance[ind_dest][ind_sources] = dist;
    map->distance[ind_sources][ind_dest] = dist;
    return true;
}

void toStringCities(Cities * map){
    for(int i = 0; i < map->name_size;i++){
        printf("Ville : %s - %d : ( ", map->name[i], i);
        for(int j = 0; j < map->name_size;j++){
            if(!map->distance[i][j]) map->distance[i][j] = 0.0;
            printf("%s - %s = %.2f | " ,map->name[i], map->name[j], map->distance[i][j]);
        }
        printf("end )\n");
    }
}

void destroyCities(Cities * map){
    int i;
    for(i = 0 ; i < map->distance_size;i++){
        if(map->distance[i])free(map->distance[i]);
    }
    /*for(i = 0; i < map->name_size;i++){
        if(map->name[i])free(map->name[i]);
    }*/
    if(map->name)free(map->name);
    if(map->distance)free(map->distance);
    if(map)free(map);
}

float getDistanceBetween(Contact * c1, Contact * c2, Cities * map){
    int ind1,ind2,i;
    for(i = 0; i < map->name_size ; i++){
        if(!strcmp(c1->town,map->name[i])){
            ind1 = i;
            break;
        }
    }
    for(i = 0; i < map->name_size ; i++){
        if(!strcmp(c2->town,map->name[i])){
            ind2 = i;
            break;
        }
    }
    return map->distance[ind1][ind2];
}

float getDistanceAverage(ArrayListContact * list, Cities * map){
    float average = 0;
    Contact * p1;
    Contact * p2;
    for(int i = 0; i < list->size; i++){
        p1 = list->users[i];
        for(int j = 0; j < list->size; j++){
            p2 = list->users[j];
            average += getDistanceBetween(p1,p2,map);
        }
    }
    return average / (list->size * list->size);
}

