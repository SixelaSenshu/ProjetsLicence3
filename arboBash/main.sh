#!/bin/bash

# William Dubois 33%
# Maxime De Sainte Maresville 34%
# Alexis Bernard 33%

#_________________________________________________________________________ Variables
# - On met par défaut les paramètres
size_args=$# # Taille des arguments
args_option="-n" # Options de tri
args_arbro=1 # Arborescence
args_order="-c" # Ordre de tri
args_repertory=${!#} #Le chemin courant
res= # Variable pour le résultat

#_________________________________________________________________________ Recherche de type
# - On récupère le caractère un par un 
# - On regarde si il doit est validé ou non
# - On retourne la valeur
function typeParameter(){
    local param=$1
    local size_param=${#1}
    local current_param
    for((y=0; "$y" < "$size_param";y++)); do
       current_param=${param:$y:1}
       case "$current_param" in
        '-')
        ;;
        'r'|'R')
            res="2"
        ;;
        'd'|'c')
            res="1"
        ;;
        'n'|'s'|'m'|'l'|'e'|'t'|'p'|'g')
            res="3"
        ;;
        *)
            echo "Erreur de saisie de paramètre : Paramètre erroné $current_param."
            exit 1
        ;;
       esac
    done
}

#_________________________________________________________________________ Vérification de la saise
# - On check si le dernier paramètre est un dossier (obligatoire)
if ! test -d $args_repertory;
    then echo "it's not a directory"
    exit 0
fi

#_________________________________________________________________________ Recherche d'arguments
# - On vérifie qu'il y a bien un nombre d'arguments correct 
# - On fait une boucle et on stocke chaque type d'option saisie
# - On met à jour le paramètre (sinon on laisse par défaut)

if test "$size_args" -gt 0 && test "$size_args" -lt 5;
    then for((i=1;i<"$size_args";i++)); do
        var=${*:i:${#i}}
        typeParameter "$var"
        
        if [ "$res" == "1" ]; 
           then args_order="$var"
        elif [ "$res" == "2" ]; then
            if test "$var" = "-R"
                then args_arbro=1
            else
                args_arbro=0
            fi
        elif [ "$res" == "3" ];
            then args_option="$var"
        fi
        
    done
else #Si il a trop ou pas assez d'option on ne lance pas la commande
    exit 
fi

#_________________________________________________________________________ Lancement des fonctions
#_____________Fusion_____________
# Selon l'ordre, on fait croissant et décroissant.
if [ "$args_order" == "-d" ];
    then res=$(./triDecroissant.sh "$args_arbro" "$args_option" "$args_repertory")
else [ "$args_order" == "-c" ];
    res=$(./tri.sh "$args_arbro" "$args_option" "$args_repertory")
fi

#_____________Affichage_____________
#On regarde si on fouille ou non les dossiers et on met à jour l'option
if test "$args_arbro" -eq 1; 
    then args_arbro="-R"

elif test "$args_arbro" -eq 0;
    then args_arbro="-r"

fi

./afficher.sh "$args_arbro" $res
