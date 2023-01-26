#include "../headers/stack.h"

StackContact *  createStackContact(){
    StackContact * stack = (StackContact *)malloc(sizeof(StackContact));
    if(!stack){
        ERROR_EXIT("Erreur d'allocation pour la pile de contact", EXIT_FAILURE);
    }
    stack->capacity = 30;
    stack->size =0;
    stack->contact = (Contact **)malloc(sizeof(Contact *) * stack->capacity);
    if(!stack->contact){
        ERROR_EXIT("Erreur d'allocation pour la pile de contact", EXIT_FAILURE);
    }
    stack->ranks = (unsigned int *)malloc(sizeof(unsigned int) * stack->capacity);
    if(!stack->ranks){
        ERROR_EXIT("Erreur d'allocation pour la pile de contact", EXIT_FAILURE);
    }
    return stack;
}

void destroyStackContact(StackContact * stack){
    free(stack->ranks);
    free(stack->contact);
    free(stack);
}

void toStringStackContact(StackContact * stack){
    for(int i = 0; i < stack->size; i++){
        printf("Name : %s , Rank : %d \n",stack->contact[i]->name,stack->ranks[i]);
    }
}

void push(StackContact * stack, Contact * contact, unsigned int rank){
    if(stack->size == stack->capacity){
        Contact ** contacttmp = (Contact **)realloc(stack->contact, stack->capacity + 30 * sizeof(Contact*));
        if(!contacttmp){
            ERROR_EXIT("Erreur de réallocation pour la pile de contact", EXIT_FAILURE);
        }
        stack->contact = contacttmp;
        unsigned int * rankstmp = (unsigned int * )realloc(stack->ranks,sizeof(unsigned int) * stack->capacity + 30); 
        if(!rankstmp){
            ERROR_EXIT("Erreur de réallocation pour la pile de contact", EXIT_FAILURE);
        }
        stack->ranks=rankstmp;
    }
    stack->contact[stack->size]= contact;
    stack->ranks[stack->size]= rank;
    stack->size++;
}

unsigned isEmptyStackContact(StackContact * stack){
    return stack->size <= 0 ? 0 : 1;
}

Contact * pop(StackContact * stack){
    if(stack->size > 0){
        stack->size--;
        return stack->contact[stack->size];
    }else{
        return NULL;
    }
}

Contact * top(StackContact * stack){
    if(stack->size > 0){
        return stack->contact[stack->size - 1];
    }else{
        return NULL;
    }
}

unsigned int topRank(StackContact * stack){
    if(stack->size > 0){
        return stack->ranks[stack->size - 1];
    }else{
        return 0;
    }
}
