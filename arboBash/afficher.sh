#!/bin/bash

#fonction qui sert à réduire les noms des fichiers passés dans afficher (enlever le chemin)
#$1 string
function reduire() {
    local i=0;
    #indice du dernier "/"
    local ind=-1;
    local res="$1";
    #longueur de la chaîne entrée en paramètre
    local length=${#res};
    #longueur du début de chaîne jusqu'à ind
    local length_tmp;
    local char_tmp;
    while ((i < length))
    do
        #caractère à l'indice i
        char_tmp=${res:i:1}
        #si on trouve un "/" on mémorise son indice dans la variable ind
        if [ "$char_tmp" == "/" ]
        then
            ind=$((i))
        fi
        i=$[$i+1]
    done
    #si un "/" a été trouvé
    if [ $ind -ne -1 ]
    then
        ind=$[ind+1]
        length_tmp=$[$length-$ind]
        #res va stocker tout ce qui se trouve après le dernier "/"
        res=${res:ind:length_tmp}
    fi
    echo "$res"
}

#fonction servant à calculer le nombre de chiffres entre les deux accolades au début de la chaîne
#$1 chaîne de caractère
function nb_chiffres() {
    local i=1;
    #indice de la première accolade fermante
    local ind_accolade=-1;
    local res=0;
    local ch;
    while ((i<${#1})) && ((ind_accolade==-1))
    do
        ch=${1:i:1}
        if [ $ch == "}" ]
        then
            ind_accolade=$i
        else
            res=$[res+1]
        fi
        i=$[i+1]
    done
    echo "$res"
}

#foncion qui sert à afficher les fichiers entrés en paramètre
#$1 maniere (arborescence ou affichage linéaire)
#$ ... noms des fichiers
function afficher() {
    local i;
    #chaîne du dossier à l'indice i
    local ch;
    #chaîne du dossier à l'indice i réduite et affichable
    local ch_tmp;
    #longueur de la chaîne
    local length;
    #longueur de la chaîne sans le {?}
    local length_tmp;

    #nombre de chiffres entre les accolades
    local _nb_chiffres
    #nombre de chiffres entre les accolades sous format int
    local nb_chiffres_int
    #nombre de chiffres entre les accolades + les 2 accolades
    local nb_chiffres_int_acc

    local j;
    #chaîne qui contiendra le nombre d'espaces avec "|"
    local space;
    #nombre d'espaces à mettre avant la chaîne
    local nb_space;
    #nombre d'espaces converti en int
    local nb_space_int;
    #Si on souhaite afficher l'arborescence
    if [ "$1" == "-R" ]
    then
        #Pour le répertoire courant car on n'affiche pas "| -"
        ch="$2"
        length=${#ch}
        length_tmp=$[$length-3]
        #on réduit à partir de l'indice 3 car on enlève {?} qui se trouve au début
        ch_tmp=${ch:3:length_tmp}
        ch_tmp=$(reduire "$ch_tmp")
        echo "$ch_tmp"

        i=3
        while ((i<=$#))
        do
            #initialisation des paramètres
            ch="${!i}"
            _nb_chiffres=$(nb_chiffres "$ch")
            nb_chiffres_int=$((_nb_chiffres))
            nb_chiffres_int_acc=$[nb_chiffres_int+2]
            length=${#ch}
            length_tmp=$[$length-$nb_chiffres_int_acc]
            ch_tmp=${ch:nb_chiffres_int_acc:length_tmp}
            ch_tmp=$(reduire "$ch_tmp")
            nb_space=${ch:1:nb_chiffres_int}
            nb_space_int=$((nb_space))
            #Si c'est un fichier du répertoire courant, on ne met pas d'espace
            if [ $nb_space_int -eq 1 ]
            then
                nb_space_int=0
            fi
            nb_space_int=$[nb_space_int]
            space=""
            j=1
            while ((j<$nb_space_int))
            do
                space="$space|   "
                j=$[$j+1]
            done
            space="$space| -"
            #on concatène les espaces avec le nom du dossier réduit
            ch_tmp="$space $ch_tmp"
            echo "$ch_tmp"
            i=$[$i+1]
            done
    #Si on souhaite les afficher de façon linéaire
    else
        i=2
        while ((i<=$#))
        do
            ch="${!i}"
            _nb_chiffres=$(nb_chiffres "$ch")
            nb_chiffres_int=$((_nb_chiffres))
            nb_chiffres_int_acc=$[nb_chiffres_int+2]
            length=${#ch}
            length_tmp=$[$length-$nb_chiffres_int_acc]
            ch_tmp=${ch:nb_chiffres_int_acc:length_tmp}
            ch_tmp=$(reduire "$ch_tmp")
            echo "$ch_tmp"
            i=$[$i+1]
        done
    fi
}

afficher $@