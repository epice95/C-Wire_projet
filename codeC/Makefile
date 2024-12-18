# Variables
CC = gcc
CFLAGS = -Wall -g -std=c11
OBJ = main.o avl.o
EXEC = cwire

# Règle par défaut : compilera et liera le programme
all: $(EXEC)

# Lier les objets pour créer l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Compiler le fichier main.c en main.o
main.o: main.c avl.h
	$(CC) $(CFLAGS) -c main.c

# Compiler le fichier avl.c en avl.o
avl.o: avl.c avl.h
	$(CC) $(CFLAGS) -c avl.c

# Nettoyage des fichiers générés (objets, exécutables)
clean:
	rm -f $(OBJ) $(EXEC)

# Cible pour exécuter le programme
run: $(EXEC)
	./$(EXEC)

# Cible pour vérifier la syntaxe et faire des tests si nécessaire
debug: CFLAGS += -DDEBUG
debug: $(EXEC)

