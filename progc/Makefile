# Définir le compilateur utilisé
CC = gcc

# Définir le nom de l'exécutable
EXEC_NAME = traitement

# Compile tout
all: CY_Truck_s
	
# Compile le script pour l'option s
CY_Truck_s:
	$(CC) traitement-s.c fonctionAVL.c -o $(EXEC_NAME)_s

# Efface les fichiers compilés
clean:
	# linux os ci dessous #
	rm -f $(EXEC_NAME)_*
