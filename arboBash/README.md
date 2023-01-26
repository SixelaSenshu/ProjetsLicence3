# <center> Project Bash : Tri de repertoire avec options </center>
<hr>

## Sommaire 
- I.Utilisation
- III.Structure du dossier
- IV.Fonctionnement
- V.Fonctionnalités

<hr>

## Utilisation

Tout d'abord, autoriser les lectures de fichier via chmod u+x <file>
- Pour lancer l'assistance : ```./help.sh```
- Pour lancer le programme : ```./main.sh <arborescence> <ordre> <options>``` <dossier>
    - note : l'ordre des options sont facultatifs (Sauf le dernier)
- Exemple1 : Sans options
    * ./main.sh . -> ./main.sh -R -c -n . (Options par défauts)
- Exemple2 : Avec certaines options
    * ./main.sh -d -R . -> ./main.sh -R -d -n . (-n Par défaut)
- Exemple3 : Avec options
    * ./main.sh -r -nsm file -> ./main.sh -r -c -nsm file 

<hr>

## Structure du dossier

1. main.sh
    * Programme principal qui lit les paramètres et fait appel aux autres fonctions/fichiers.
    * Il renvoie une erreur si la commande est invalide.
2. afficher.sh
    * Affiche le résultat du tri avec ou sans l'arborescence. (Par défaut : -R)
3. tri.sh
    * Tri dans l'ordre croissant selon les paramètres.
    * Le tri est de type fusion 0(log(n)).
4. triDécroissant.sh
    * Tri dans l'ordre décroissant selon les paramètres. 
    * Le tri est de type fusion 0(log(n)).
5. compare.sh    
    * Compare selon les paramètres saisie
    * Tant que les deux fichiers sont égaux selon le paramètre actuel, il passe au paramètre suivant (Par défaut : -n)

<hr>

## Fonctionnement

Pour commencer, le fichier main.sh va dans un premier temps analyser les paramètres et remplacer ceux absents par les paramètres par défaut. Il va ensuite faire appel au tri selon -d ou -c (c.f fonctionnalités) avec l'ensemble des options de tri et de parcours. Il va stocker dans une variable le résultat du tri qui va être envoyé à afficher.sh.

Tri.sh ou TriDecroissant.sh va s'occuper de trier l'ensemble des fichiers reçus, en faisant les comparaisons via compare.sh. Il va retourner l'ensemble des fichiers triés sous la forme : {PROFONDEUR}CHEMIN/FICHIER. (ex : {0}dossier_courant {1}fichier1 {1}dossier1 {2}fichier2)

Compare.sh se charge de comparer un à un les fichiers entre eux, en lisant à la suite chaque paramètre de tri tant qu'il y a une égalité entre ceux-ci selon le paramètre courant.

Afficher.sh a pour rôle d'afficher l'arborescence complète à l'aide de la profondeur indiquée pour chaque fichier s'il y a la présence de -R. Sinon il affichera uniquement les fichiers linéairement.

<hr>

## Fonctionnalités

1. Type de parcours du dossier
    1. -R (Par défaut) : Parcourt l'arborescence du dossier.
    2. -r : Parcourt le dossier en surface.
2. Ordre de tri
    1. -c (Par défaut) : Tri dans l'ordre croissant.
    2. -d : Tri dans l'ordre décroissant.
3. Paramètre de tri
    1. -n : (Par défaut) : Tri par nom (Hors extension du fichier '^[A-Za-z]+\.')
    2. -s : Tri par taille des fichiers/dossiers (Somme des sous-fichiers et sous-dossiers)
    3. -m : Tri selon la dernière date de modification.
    4. -l : Tri selon le nombre de ligne des fichiers/dossiers (Somme des sous-fichiers et sous-dossiers)
    5. -e : Tri par extension du fichier (Hors nom du fichier '\.[A-Za-z]+$')
        * note : Tri par nom des extensions pour les fichiers régulier (Ordre croissant)
        * Tri par type pour les autres.
    6. -t : Tri par type de fichier.
    7. -p : Tri selon le nom du propriétaire.
    8. -g : Tri selon le nom du groupe du propriétaire.
4. Autres
    1. Dans le cas ou un dossier est vide, on aura dans l'affichage '*' 