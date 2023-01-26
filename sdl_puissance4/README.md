# README.md
Fichier explicant brievement le déroulement du jeu ainsi que les commandes du programme.
Pour plus d'info voir le rapport sous format -.pdf

------------------------------------------------------------------------------------------------------------------
## 0.Sommaire
    - 1.Utilisations pour make
        -1.1 Jeu
        -1.2 Génération 
    - 2.Utilisations & Fonctionnalité du puissance4
        - 2.1 Interface dans l'invite de commande
        - 2.2 Interface via la SDL
------------------------------------------------------------------------------------------------------------------
## 1.Utilisations pour make && exec
### 1.1 Jeu
* Les commandes avec l’exécutable :
    * ./exec -c → Permet de lancer le jeu en mode textuelle
	* ./exec -s → Permet de lancer le jeu en mode graphique
!!! Attention, l’option est obligatoire. Dans le cas ou il n’y a pas d’option, le programme ne lance pas le jeu et se ferme par la suite.

### 1.2 Génération 
* Les commandes avec le makefile : 
	* make → Permet le générer les exécutables de chaque fichier en -.c avec les fichiers en -.h et de les relier entre afin de créer un fichier en byte code exécutable qui permet de lancer le jeu.
	* make mrproper → Permet d’effacer l’exécutable actuelle du dossier si il y en a 1.
	* make clean → Permet d’effacer tous les fichiers exécutables du projet dans le dossiers Object ainsi que l’exécutable principale lui même.
	* make zip → Permet de créer un dossier compressé en .zip 
	* make doc → Permet de générer la documentation des fichiers actuelles.

------------------------------------------------------------------------------------------------------------------
## 2.Utilisations & Fonctionnalité du puissance4

### 2.1 Interface dans l'invite de commande

* On a le choix entre 3 modes de jeux, et 3 modes de difficulté pour l'ia.
- 1 pour JvsJ , 2 pour JvsIA et 3 pour IAvsIA
- Les difficultés sont FACILE / NORMALE / DIFFICILE. On doit les écrires de cette manière.
- On a le joueur actuelle à chaque tour (1 ou 2 sont les cases respectives des joueur.
- On doit choisir un nombre entre 0 et 6 sinon le programme continuera à redemander.

### 2.2 Interface via la SDL

* On a un plateau, un indice pour connaître la personne qui doit jouer, des boutons pour choisir les difficultés, les modes de jeu ainsi que les boutons pour démarrer un nouveau mode ou redemarrer l'ancien.

* Quand on choisie une mode avec une ia, les boutons du haut (ou il y a les modes) changent et montrent les niveaux de difficultés. Il y a deux fois le choix si le mode est IAvsIA. Dans le cas ou on ne choisie pas, on aura la difficulté normale

* le bouton start permet de lancer le mode avec des nouvelles options choisi: On peut lancer avec la touche S

* le bouton restart permet de relancer l'ancien mode. (On peut le lancer avec la touche R)
Si on chosit des modes et que l'on relance, les nouveaux choix seront effacés et il faudra alors rechoisir.

* les touches 1, 2 et 3 permettent différents lancement de jeu:
    * 1 : On lance le mode JvsJ avec les difficultés USER & USER
    * 2 : On lance le mode JvsIA avec les difficultés USER & IA
    * 3 : On lance le mode IAvsIA avec deux difficultés en Normal

* On peut quitter le jeu via la croix ou avec la touche Q
* 
* On peut agrandir et reduire la fenêtre. Le rendu peut en être affectée donc il faut faire attention.
  

