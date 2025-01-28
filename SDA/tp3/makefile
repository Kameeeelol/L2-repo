# Nom de l'exécutable
EXEC = main_expr

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Fichiers sources et en-têtes
SRC = binary_tree.c main_expr.c queue_link.c stack_link.c
HEADERS = item.h BT.h binary_tree.h queue_link.h stack_link.h

# Cibles
OBJ = $(SRC:.c=.o)

# Règle par défaut
all: $(EXEC)

# Création de l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation des fichiers objets
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJ) $(EXEC)

# Règle pour tout reconstruire
rebuild: clean all
