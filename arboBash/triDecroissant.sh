#!/bin/bash
# But de ce fichier: echo une string de type : {0}rep {1}fichier1.txt {1}fichier2.txt
# Dans le cas d'un tri de l'arborescence : {0}rep {1}fichier.txt {1}fichier2.txt {1}dossier {2}fichier3.sh {2}fichier4.c {1}fichier5.sh
# Cette chaine est utilisée dans la fonction d'affichage, utilisant les degrés entre accolade pour déterminer le degré de profondeur

# parcours la partie gauche et droite simultanément et compare les fichiers 2 à 2 pour trier
function partition () {
    local ind1=2 #indice de départ de la partie gauche (le $1 est le nombre de paramètre de la partie gauche, donc démarrage à 2)
    local ind2=$(($1+2)) #indice de départ de la partie droite (nbParamGauche + 2)
    local res="" #string finale

    local degre
    degre=$(echo "${!ind1}" | cut -d'{' -f2 | cut -d'}' -f1) #degré du premier fichier à gauche
    # local degre2
    # degre2=$(echo "${!ind2}" | cut -c2- | cut -c-1) #degré du premier fichier à droite (plus utilisé)

    local tmp1="" #voir ligne 42/43
    local tmp2=""

    while test "$ind1" -le "$(($1+1))" && test "$ind2" -le $# #tant que nous n'avons pas atteint la fin d'une des parties (gauche et droite)
    do

        #ces 2 if servent dans le cas d'un tri de l'arborescence, car les paramètres peuvent contenir des fichiers de différentes profondeur
        #si le fichier rencontré possède un degré de profondeur suppérieur au premier fichier (qui possède le degré minimum)
        #alors ajouter ce fichier directement à res, incrémenter l'indice, et passer à l'itération suivante (il est possible de passer plusieurs itérations d'affilée dans ces if)
        #impossible de passer dans les 2 if à la fois car nous n'y passerons qu'après avoir mis un dossier dans res, donc une seule partie est affectée
        if [ "$(echo "${!ind1}" | cut -d'{' -f2 | cut -d'}' -f1)" -gt "$degre" ]
        then
            res="$res ${!ind1}"
            ind1=$(("$ind1"+1))
            continue
        fi
        if [ "$(echo "${!ind2}" | cut -d'{' -f2 | cut -d'}' -f1)" -gt "$degre" ]
        then
            res="$res ${!ind2}"
            ind2=$(("$ind2"+1))
            continue
        fi

        #ne pas prendre les accolades et profondeurs pour comparer les fichiers (pas utile, simple mesure de précaution)
        tmp1=$(echo "${!ind1}" | cut -d'}' -f2)
        tmp2=$(echo "${!ind2}" | cut -d'}' -f2)

        local comp
        comp=$(./compare.sh "$tmp1" "$tmp2" "$options")  #comparer les fichiers tmp1 et tmp2, comp = 0 si les fichiers sont dans le bon ordre, 1 sinon
        if test "$comp" -eq 1 #si comp == 1 ajouter le fichier de la partie gauche à res
        then
            res="$res ${!ind1}"
            ind1=$(("$ind1"+1))
        else #sinon ajouter celui de droite
            res="$res ${!ind2}"
            ind2=$(("$ind2"+1))
        fi
    done
    #après ce while, il reste une des parties où il reste des fichiers (obligatoirement)
    #ajouter ces fichiers à res sans changement d'ordre

    while test "$ind1" -le "$(($1+1))" #partie gauche
    do
        res="$res ${!ind1}"
        ind1=$(("$ind1"+1))
    done

    while test "$ind2" -le $# #partie droite
    do
        res="$res ${!ind2}"
        ind2=$(("$ind2"+1))
    done
    echo "$res" #echo resultat
}

# fusion en partie gauche et droite des paramètres (fichiers la plupart du temps) et appel à partition qui trie
function fusion () {
    if test $# -eq 1 #cas où un dossier est vide, et appelle donc fusion avec seulement le degré en paramètre 
    then
        return
    fi
    if test $# -eq 2 #si il y a 2 paramètres : degré de profondeur et fichier/dossier, cas spéciaux
    then
        if test "$1" -eq 0 #si on est au degré 0: il faut explorer les fichiers du répertoire
        then
            echo "{$1}$2" #echo du répertoire rentré avec son degré (0)
            vide=$(find "$2" -maxdepth 0 -empty -exec echo 0 \;)
            if test "$vide" = "0"
            then
                return
            fi
            local files="" #string qui contiendra les fichiers du repertoire exploré
            for j in "$2"/* #boucle sur les fichiers du répertoire
            do
                files="$files $j" #concaténation une a une des fichiers a la variable
            done
            fusion $(($1+1)) $files #appel de fusion sur les fichiers avec un degré incrémenté (1 maintenant)
        elif [ "$arborescence" -eq 1 ] #si nous ne somme pas au degré 0 et qu'il faut explorer l'arborescence
        then
            echo "{$1}$2" #d'abord echo le nom du fichier/dossier avec son degré (1 ou plus)
            local files="" 
            if [ -d "$2" ] #si ce paramètre est un dossier
            then
                vide=$(find "$2" -maxdepth 0 -empty -exec echo 0 \;)
                if test "$vide" = "0"
                then
                    return
                fi
                for j in "$2"/* #ajouter les fichiers de ce dossier à une string
                do
                    files="$files $j" #concaténation
                done
                fusion $(($1+1)) $files #appel de fusion sur ces fichiers avec un degré incrémenté
            fi
            exit 1
        else #s'il ne faut pas explorer l'arborescence, juste echo le nom du fichier avec son degré
            echo "{$1}$2"
            exit 1
        fi
    fi

    local cpt=0 #compteur d'itération de la boucle

    local gauche="" #string de la partie gauche
    local droite="" #string de la partie droite

    for i in "$@" #boucle sur les paramètres
    do
        if [ $cpt -eq 0 ] #si cpt == 0, donc $i == $1, ne rien faire car c'est le degré et non un fichier/dossier 
        then
            cpt=$((cpt+1))
            continue
        fi
        cpt=$((cpt+1))
        if test "$cpt" -le "$((($#/2)+1))" #si nous sommes dans la première moitié des paramètre (hors le premier) concaténer le fichier à la chaine de gauche
        then
            gauche="$gauche $i"
        else #sinon concaténer à droite
            droite="$droite $i"
        fi
    done

    gauche=$(fusion "$1" $gauche) #appeler fusion sur la partie gauche
    droite=$(fusion "$1" $droite) #appeler fusion sur la partie droite
    #les 2 parties sont triées ici
    local nbParamGauche
    nbParamGauche=$(echo "$gauche" | wc -w) #nombre de fichiers de la partie gauche (utile quand les fichiers des sous-dossiers sont rajouté à la chaine)

    partition $nbParamGauche $gauche $droite #appel de partition avec les nombre de paramètres à gauche et des deux partie (non protégées surtout, pour split chaque fichier en paramètre pour partition)
    exit
}

# Petit main pour démarrer le programme
function main () {
    arborescence=$1 #0 pour non, ou 1 pour oui, selon si on veut trier en profondeur et non le fichier entré en paramètre uniquement
    options=$2 #les options de tri (ex: -nsl)
    fusion 0 "$3" #appel du tri à fusion, le 0 est une variable pour connaître le degré de profondeur, $3 est le répertoire entré
}

main "$1" "$2" "$3"