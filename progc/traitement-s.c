#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
typedef struct Trajet{
	int trajet;
	float distance_maxi;
	float distance_mini;
	int nombre_etape;
	float moyenne;
	float valeur;
	struct Trajet * droit;
	struct Trajet * gauche;
	int hauteur;
}Trajet;





int main(){
	Trajet* Arbre;
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
        	
        	Trajet * noeud_trajet = recherche(arbre , trajet_id);
        	
        	
        	if(noeud_trajet!= NULL){
        		a = noeud_trajet->valeur;
        		modificationNoeud(noeud_trajet, km);
        		b = noeud_trajet->valeur;
        		if (a!=b){
        		
        			suppressionAVL(arbre,noeud_trajet->valeur, noeud_trajet->hauteur);
        			
        			insertionAVL(arbre, noeud_trajet);
        			
        		} 
        			
        	}else{
        		insertionAVL(arbre,CreationNoeud(km));
        	}
        	
    	}
    	
	afficherAVL(arbre);
    	fclose(fichier); // Fermer le fichier
    	return 0; // Succès
}
