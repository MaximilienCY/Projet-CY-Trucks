#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#define TOP_N 50

int main(){
	
	Trajet* arbre = NULL;
	FILE * fichier = fopen("../data.csv", "r");
    	if (fichier == NULL) {
        	perror("Erreur lors de l'ouverture du fichier");
        	return 1;
    	}
    	
	char ligne[500]; 
	
	if (fgets(ligne, 500, fichier) == NULL) {
        	printf("Le fichier est vide ou une erreur de lecture s'est produite.\n");
        	fclose(fichier);
        	return 1;    
    	}
    		
    	int trajet_id;
    	int etape;
    	float km;
    	char *valeurs[6];
    	
    	while (fgets(ligne, 500, fichier) != NULL) {
    		
        	char *token = strtok(ligne, ";");
        	int colonne = 0;
        	
        	while (token != NULL && colonne < 6){
			valeurs[colonne] = token;
			token = strtok(NULL, ";");
			colonne++;
        	}
        	 
		// Stocker les valeurs des colonnes 1, 2 et 5
        	if (colonne >= 5) {
            		trajet_id = atoi(valeurs[0]);
            		etape = atoi(valeurs[1]);
            		km = atof(valeurs[4]);
        	}
        	
        	int h = 0;
        	Trajet * NouveauTrajet = creerNoeud(trajet_id,km);
        	arbre = insertionAVL(arbre, NouveauTrajet,km , &h);
        	
    	}
    	
	// Initialisation du tableau pour stocker les 50 plus grandes valeurs et du compteur
    	Trajet* topTrajets[TOP_N] = {0};
    	int nombreTrajets = 0;

    	// Parcourir l'arbre et collecter les 50 plus grandes valeurs
    	parcourirEtCollecterTop50(arbre, topTrajets, &nombreTrajets);

    	// Afficher les 50 plus grandes valeurs
    	afficherTop50(topTrajets, nombreTrajets);
    	
    	fclose(fichier); // Fermer le fichier
    	return 0; // Succès
}
