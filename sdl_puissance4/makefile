#---------------------------------------------------------------------- Makefile ----------------------------------------------------------------------#
.DEFAULT: mrproper err
.PHONY: clean mrproper doc zip manage_repository infoComp clean-local
.DELETE_ON_ERROR : mrproper

.INIT: mrproper
.VARIABLE: CC SOURCES_DIR OBJECTS_DIR DOC OBJS CFLAGS LPATH LLIB LDFLAGS DEPS TARGET DOCFILE


#________________________________________ Architectures du project
CC = gcc
SOURCES_DIR = sources
OBJECTS_DIR = objects
DOC = doc
#________________________________________ Options pour la compilation
CFLAGS ?=
LPATH := -L/home/alexisbernard/Lib/SDL-main/lib
LLIB := -l:libSDL2.so -lm
LDFLAGS = $(LPATH) $(LLIB)

#________________________________________ Dépendances et récuperation des données du  project
OBJS = $(shell find .)
OBJS := $(shell find $(SOURCES_DIR) -name "*.c" |sed 's/.c$$/.o/g' |sed 's/$(SOURCES_DIR)/$(OBJECTS_DIR)/g')
DEPS := $(OBJS : .o = .d) 
TARGET ?= exec
DOCFILE ?= Doxyfile

all: manage_repository $(TARGET) 

#________________________________________ Compilation des fichiers

#________________ Information sur la compilation
err:
	@echo Problème de compilation de fichier

infoComp:
	@echo "---------------- Compilation --------------------"
	@echo "[WAITING]Début de la compilation des .o"

#________________ Commandes pour la compilation

$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJECTS_DIR)/Vue/%.o: $(SOURCES_DIR)/Vue/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS_DIR)/Jeu/%.o: $(SOURCES_DIR)/Jeu/%.c	
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): infoComp $(OBJS)
	@echo "[CHECK]Fin de la compilation des .o"
	@echo "---------------- Assemblage ---------------------"
	@echo "[WAITING]Generation de l'executable pour le programme..."
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo "[CHECK]Generation de l'executable pour le programme validé."
#________________________________________ Nettoyage 

manage_repository:
	@echo "---------------- Création des dossiers ----------"
	@echo "[WAITING]Creation des dossiers..."
	@mkdir -p $(OBJECTS_DIR)
	@mkdir -p $(OBJECTS_DIR)/Vue
	@mkdir -p $(OBJECTS_DIR)/Jeu
	@echo "[CHECK]Creation des dossiers validé."

clean: 
	@echo "---------------- Nettoyage ----------------------"
	@echo "[WAITING]Nettoyage des objects..."
	rm -rf $(OBJECTS_DIR)
	@echo "[CHECK]Nettoyage des objects terminé"

mrproper:
	@echo "---------------- Nettoyage ----------------------"
	@echo "[WAITING]Nettoyage des objects et de l'executable..."
	rm -rf $(OBJECTS_DIR)
	rm -f $(TARGET)
	@echo "[CHECK]Nettoyage des objects et de l'executable terminé."

#________________________________________ Gestion du project
doc:
	@echo "[WAITING]Création des documentations..."	
	rm -rf $(DOC)
	@mkdir -p $(DOC)
	@doxygen $(DOCFILE)
	@echo "[CHECK]Création des documentations valides."

zip:
	@echo "[WAITING]Compression des fichiers..."
	@zip -r zipper.zip *
	@echo "[CHECK]Compression des fichiers valides."

-include $(DEPS)
