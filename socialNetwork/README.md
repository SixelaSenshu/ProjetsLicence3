# TP3 

## Introduction

- Ce projet contient plusieurs programme permettant de faire un étude de graphes sur un ensemble de personne et d'observer en détail tous les informations

- Toutes les fonctionnalités demandé sont tester directement dans le main, 
ou indirectement dans le code des fonctions dans le main

## Fonctionnement

- trois commandes : 
    * compiler : make
    * nettoyer : make clean ou mrproper
    * run :  ./exec pathfile (i.e ./data/txt1.txt (voir txt2.txt pour un meilleur exemple à propos des composantes connexes))

- principe du main:
    1. parse le fichier puis génère la liste de contact , les abonnements , les villes ainsi que la matrice de distance 
    (c.f listContact, linkedList, cities, contact, parsing)
    2. à partir de tout ceci, on symetrise le graphes et gére les abonnées de chaque personne. puis on créer la liste de connection de distance à partir des abonnements uniquement (c.f hashmapConnection)
    3. une fois tout ceci réaliser, on fais un parcours de tous les connections et on créer les composantes connexes simple et forte avec la methode classique mais aussi via l'algorithme de Tarjan
    ( c.f à nouveau listContact, stack)

- macro.h donne une template pour les gestions d'erreurs 


##  Architecture

* headers / sources : 
    contient les fichiers sources et les en-têtes de chaque fichier

* data : contient les documents qui doivent être parser

* bin : contient les fichiers de compilation (par défaut déja fournie mais make permet de les refaire si voulu)
