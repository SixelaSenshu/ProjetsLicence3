#!/bin/bash

## Variables ---------------------------------------------------------------------------------------------------------------
equal=-1
option=$3
res=0

# Gestion des nombres de paramètres : Si + ou - que 3 : problème de paramètres : Fin la comparaison
if [ $# -ne 3 ]; 
    then echo "Not enough parameters : (file1) (file2) -(opt1) ... -(optN)"
    return
fi
## Functions ---------------------------------------------------------------------------------------------------------------
###
# @brief Compare un fichier en fonction de son type et renvoie son ordre de priorité.
# @param $1 : Fichier que l'on compare.
function compareType () {
    file=$1
    # On test chaque type de fichier et on retourn une valeur qui correspond à l'ordre des types
    if test -d "$file"; # directory
        then res=0
    elif test -L "$file"; # link
        then res=2
    elif test -f "$file"; # file
        then res=1
    elif test -b "$file"; # block
        then res=3
    elif test -c "$file"; # special character
        then res=4
    elif test -p "$file"; # named pipe
        then res=5
    elif test -S "$file"; # socket 
        then res=6
    fi
}

###
# @brief Calcule la taille d'un fichier ou la somme des fichiers d'un répertoire 
# @param $1 : Fichier / Dossier que l'on examine
function getSizeRepertory(){
    somme=0
    if test -d "$1" 
        then 
        # On regarde si il y a un fichier dans le dossier courant
        vide=$(find "$1" -maxdepth 0 -empty -exec echo 0 \;)
        if test "$vide" = "0"
            then echo 0
            return
        fi
        for fichier in "$1"/*
        
        do 
            if test -d "$fichier";
                then res="$(echo $(getSizeRepertory "$fichier"))"
            else   
                res=$(stat -c %s "$fichier")
            fi 
            somme=$((somme+res))
        done
        echo "$somme"  

    else
        echo $(stat -c %s "$1")
    fi
}

###
# @brief Calcule le nombre de ligne d'un fichier ou la somme des fichiers d'un répertoire 
# @param $1 : Fichier / Dossier que l'on examine
function getlinesRepertory(){
    somme=0
    if test -f "$1"
        then echo $(wc -l "$1" |cut -d' ' -f1)
    elif test -d "$1"
        then 
        for fichier in "$1"/*
        do 
            if test -d "$fichier";
                then res="$(echo $(getlinesRepertory "$fichier")) "
            elif test -f "$fichier"
                then res=$(wc -l "$fichier" |cut -d' ' -f1)
            else
                res=0
            fi 
            if [ "$res" -gt 0 ]; then
                somme=$((somme+res))
            fi
        done
        echo "$somme" 
    else echo 0 
    fi
}

###
# @brief Compare deux fichiers en fonction d'un paramètre.
# @param $1 : Fichier que l'on comparer.
# @param $2 : Fichier qui permet la comparaison.
# @return Une valeur entre 0 et 1 selon la comparaison.
function compare(){
    #Variable locales
    local file1=$1 
    local file2=$2
    local value1
    local value2
    local type_result # contiendra le type de resultat 
    # en effet , pour certaines comparaisons, on attend une expression avec uniquement des entiers ou des chaînes de caractères

    #Analyse le paramètre actuelle et lance une comparaison en fonction de celui-ci.
    case $current_option in
        "n") # Permet de comparer les noms des fichiers ( hors extensions)
            value1=$(echo "$file1" | cut -d'.' -f1) # on cut à partir du premier point et on recupère la partie gauche ( pour l'extension , sa sera l'inverse )
            value2=$(echo "$file2" | cut -d'.' -f1)
            type_result="txt"     
        ;;
        "s") # Permet de calculer la taille du fichier
            value1=$(getSizeRepertory "$file1") # avec la command stats on peut ainsi récuperer plusieurs valeurs à propos du fichier
            value2=$(getSizeRepertory "$file2")
            type_result="nb"
        ;;
        "m") # Permet de calculer la difference de temps entre les dernière modification de fichier
            value1=$(stat -c %y "$file1")
            value2=$(stat -c %y "$file2")
            type_result="txt"
        ;;
        "l") # permet de comparer le nombre de ligne 
            # Dans le cas ou ce n'est pas un fichier, on met à zéro
            value1=$(getlinesRepertory "$file1") # avec la command stats on peut ainsi récuperer plusieurs valeurs à propos du fichier
            value2=$(getlinesRepertory "$file2")
            type_result="nb"
        ;;
        "e")
            # Permet de comparer les extensions
            if test -f "$file1";
                then value1=$(echo "$file1" | cut -d'.' -f2)
                # Si ce n'est pas un fichier, il n'y a pas d'extension 
            else
                value1="0"
            fi

            if test -f "$file2";
                then value2=$(echo "$file2" | cut -d'.' -f2) 
            else
                value2="0"
            fi
            type_result="txt"
        ;;
        "t") # Permet de comparer les types de fichier ( pas les extensions )
            compareType "$file1"
            value1="$res"
            compareType "$file2"
            value2="$res"
            type_result="nb"
        ;;
        "p") #Permet de récuperer le nom du propriétaire du fichier
            value1=$(stat -c %U "$file1")
            value2=$(stat -c %U "$file2")
            type_result="txt"
        ;;
        "g") #Permet de récuperer le nom du groupe du propriétaire du fichier
            value1=$(stat -c %G "$file1")
            value2=$(stat -c %G "$file2")
            type_result="txt"
        ;;
        *) #### Le caractère n'est pas reconnue donc on ne fait rien
        ;;
    esac


    ## si la comparaison est entre deux chaînes de caractères
    if test "$type_result" = "txt"; then 
        size_value1=${#value1}
        size_value2=${#value2}
        # On regarde la valeur la plus grande parmi les caractères
        local size
        if [ "$size_value1" -gt "$size_value2" ]
        then
            size=$size_value1
        else
            size=$size_value2
        fi
        # On met les valeurs en minuscules pour pas avoir de problème dans les comparaisons de lettre
        value1=$(echo "$value1" | tr [A-Z] [a-z])
        value2=$(echo "$value2" | tr [A-Z] [a-z])
        for((j=0;j<"$size";j++)); do
            current_caract=${value1:j:1}
            current_caract2=${value2:j:1}
            # Si ce n'est pas les mêmes caractères, on retourne l'ordre de comparaisons
            if [ "$current_caract" \> "$current_caract2" ];
                then echo "1"
                equal=1
                exit 
            elif [ "$current_caract" \< "$current_caract2" ];
                then echo "0"
                equal=0
                exit
            fi
        done
        return
    ## si la comparaison est entre deux entiers
    elif test "$type_result" = "nb"; then
        if [ "$value1" -gt "$value2" ]; then
            echo "1"
            equal=1
            exit
        elif [ "$value2" -gt "$value1" ]; then
            echo "0"
            equal=0
            exit
        fi
    fi
}

### 
# @brief Compare deux fichiers
# @param $1: Fichier que l'on compare
# @param $2: Fichier avec lequel on fait la comparaison
# @return la valeur de la comparaison.
function compareFile(){

    ## Conditions ---------------------------------------------------------------------------------------------------------------
    #echo "size = $#"
    local current_option=
    local size_option=
    local f1=
    local f2=
    ### ------------------------------------- Initialisation des conditions pour la comparaison -------------------------------------
    # * On récupère les fichiers et on enlèves les ./ ( qui seront dans le tri.sh)
    # * On récuère la taille des options pour les comparaisons
    f1=$(echo "$1" | sed 's/\.\///g')
    f2=$(echo "$2" | sed 's/\.\///g')
    size_option=${#option}
   
    ### ------------------------------------- Lancement de la comparaison -------------------------------------
    #*On récupère chaque paramètre
    #*On effectue la comparaison en mettant à jour l'option
    #*On analyse les résultats
    local i=1
    while (("$i"<"$size_option"))
    do
        current_option=${option:i:1}
        compare "$f1" "$f2"
        i=$((i+1))
    done

    # Si il y a une égalité on ne fait rien
    if [ "$equal" -eq -1 ];
        then echo "0"
    fi
}

compareFile "$1" "$2" "$option"