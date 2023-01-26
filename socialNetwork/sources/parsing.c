#include "../headers/parsing.h"

Reader * createReader(){  
    Reader * reader;
    reader = (Reader *)malloc(sizeof(Reader));
    if(!reader){
        ERROR_EXIT("Problème d'allocation pour le reader",EXIT_FAILURE);
    }
    reader->buffer_capacity = 1000;
    reader->buffer_size = 0;
    reader->contacts = NULL;
    reader->cities = NULL;
    reader->file = NULL;
    reader->buffer = NULL;
    return reader;
}

void destroyReader(Reader * reader){
    destroyArrayListContact(reader->contacts);
    destroyCities(reader->cities);
    if(reader->buffer)free(reader->buffer);
    if(reader)free(reader);
}

bool readLine(Reader * reader){
    assert(reader && reader->file);
    int current =fgetc(reader->file);;
    char * res =  (char *)malloc(sizeof(char)*1);
    if(!res){
        ERROR_EXIT("Problème d'allocation pour le buffer de la struct Reader",EXIT_FAILURE);
    }
    reader->buffer_size=0;
    while(current != '\n' && current != EOF && current != '\0'){
        if(current != ' ') {
            res = (char *)realloc(res,sizeof(char)*reader->buffer_size+1);
            if(!res){
                ERROR_EXIT("Problème d'allocation pour le buffer de la struct Reader",EXIT_FAILURE);
            }
            res[reader->buffer_size++]=current;
        }
        current = fgetc(reader->file);
    }
    res = (char *)realloc(res,sizeof(char)*reader->buffer_size+1);
    res[reader->buffer_size++]='\0';
    reader->buffer = res;
    if(!reader->buffer){
        ERROR_EXIT("Problème d'allocation pour le buffer de la struct Reader",EXIT_FAILURE);
    }
    return true;
}

bool readFile(char * namefile, char * mode, Reader * reader){
    //__________________ Variables 
    char ** towns = NULL;
    char **towns_users = NULL;
    unsigned size = 0;
    unsigned size_users = 0;
    unsigned capacity_users = 30;
    
    reader->file = fopen(namefile,mode);
    if(reader->file != NULL){
        printf("\n[1]---------------------------------------------------- Ouverture du fichier \n\n");
        //__________________ Lecture des noms
        readLine(reader);
        if(!strcmp(reader->buffer,"{")){
            parsingContacts(reader);
            towns = parsingCities(reader);
            while(towns[size] != NULL){
                size++;
            }
            readLine(reader);
        }
        //__________________Lecture des villes et des abonnements
        if(!strcmp(reader->buffer,"[")){
            towns_users = (char **)calloc(sizeof(char*),30);
            for(int i = 0; i < 30; i++){
                towns_users[i] = (char *)calloc(sizeof(char),50);
                if(!towns_users[i]){
                    ERROR_EXIT("Erreur d'allocation de mémoire pour les villes (elements de liste",false);
                }
            }
            if(!towns_users){
                ERROR_EXIT("Erreur d'allocation de mémoire pour les villes",false);
            }
            //__________________ Lecture des abonnements
            while(strcmp(reader->buffer,"],")){
                readLine(reader);
                if(size_users == capacity_users){
                    capacity_users += 30;
                    char ** tmptows_users = realloc(towns_users,capacity_users*sizeof(char*));
                    if(tmptows_users == NULL){
                        ERROR_EXIT("Erreur d'allocation de mémoire pour les villes",false);
                    }
                    towns_users = tmptows_users;
                }
                if(strcmp(reader->buffer,"],"))towns_users[size_users++]=parsingSubscriptions(reader);
             }
             //__________________ Lecture des villes
             for(int i = 0; i < size_users; i++){
                reader->contacts->users[i]->town = towns_users[i];
             }
        }
        readLine(reader);
        //__________________Lecture des distances
        if(!strcmp(reader->buffer,"[")){
            reader->cities = createCities(size);
            reader->cities->name = towns;
            for(int i = 0; i < size; i++)reader->cities->name[i]=towns[i];
            int breaking = 0;
            while(breaking == 0){
                readLine(reader);
                if(strcmp(reader->buffer,"]"))parsingDistances(towns,reader);
                else breaking = 1;
            }
        }
        readLine(reader);
        //_________________Libération de mémoire
        if(!strcmp(reader->buffer,"}")){
            if(fclose(reader->file) == EOF){
                ERROR_EXIT("Erreur de fermeture de fichier",false);
            }
        }
    }else{
        printf("Erreur de lecture de fichier");
        return false;
    }
    return true;
}

void parsingContacts(Reader * reader){
    reader->contacts = createArrayListContact();
    Contact * contact = NULL;
    char * sep = "[], ";
    readLine(reader);
    char * strToken = strtok(reader->buffer,sep);
    while(strToken != NULL){
        contact = createContact(strToken,"null");
        strToken = strtok(NULL,sep);
        addArrayListContact(contact,reader->contacts);
    }
}

char ** parsingCities(Reader * reader){
    ///_________________Variables
    unsigned capacity = 30;
    unsigned size = 0; 
    char ** towns = (char **)calloc(sizeof(char *),capacity);
    readLine(reader);
    char * sep = "[], ";
    char * strToken = strtok(reader->buffer,sep);

    ///_________________Lectures 
    while(strToken != NULL){
        if(size == capacity){
            capacity += 30;
            char ** townstmp = (char **)calloc(sizeof(char *),capacity);
            if(!townstmp){
                ERROR_EXIT("Problème de parsing pour les villes ( realloc )",EXIT_FAILURE);
            }
        }
        towns[size++] = strToken;
        strToken = strtok(NULL, sep);
    }
    return towns;
}

char * parsingSubscriptions(Reader * reader){
    ///_________________Variables
    char * sep = ":";
    char * strToken = strtok(reader->buffer,sep);
    char * content = (char *)calloc(sizeof(char),200);
    char * contentlast = (char *)calloc(sizeof(char),200);
    Contact * c = getContact(reader->contacts,strToken,"null");
    Contact * c2 = NULL;
    
    ///_________________Splitting
    strToken = strtok(NULL,sep);
    reader->buffer = strToken;
    sep = "[]{},";
    strToken = strtok(reader->buffer,sep);
    content = strToken;

    ///_________________Lectures
    while(content != NULL){
        if(content)c2 = getContact(reader->contacts,content,"null");
        if(c2 != NULL)addSubscriber(c2,c);
        if(content)strcpy(contentlast,content);
        content=strtok(NULL, sep);
    }
    free(content);
    return contentlast;
}

void parsingDistances(char ** list, Reader * reader){
    ///_________________Variables    
    char * sep = ",() ";
    char * strToken = (char *)calloc(sizeof(char),100);
    char * strToken2 = (char *)calloc(sizeof(char),100);
    char * strToken3 = (char *)calloc(sizeof(char),100);
    float value = 0.0;
    ///_________________Lectures
    strToken = strtok(reader->buffer,sep);
    strToken2 = strtok(NULL,sep);
    strToken3 = strtok(NULL,sep);
    value = (float)atoi(strToken2); 
    addCities(strToken,strToken3,value,reader->cities);
}