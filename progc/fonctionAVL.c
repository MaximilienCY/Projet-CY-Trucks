#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#define TOP_N 50



//Fonction OBTENIR LA HAUTEUR d'un noead
int height(Trajet* N){
	if(N == NULL){
		return 0;
	}
	return N->hauteur;
}

//FONCTION OBTENIR LE FACTEUR d'equilibrage
int getBalance(Trajet* N) {
    	if (N == NULL) return 0;
    	return height(N->droit) - height(N->gauche);
}



// Fonction de rotation à gauche
Trajet* rotateLeft(Trajet *a) {

    	Trajet *pivot = a->droit;
    	a->droit = pivot->gauche;
    	pivot->gauche = a;

    	// Mise à jour de la hauteur
    	a->hauteur = 1 + max(height(a->gauche), height(a->droit));
    	pivot->hauteur = 1 + max(height(pivot->gauche), height(pivot->droit));
    	
	return pivot;
}


// Fonction de rotation à droite
Trajet* rotateRight(Trajet *a) {
    	Trajet *pivot = a->gauche;
   	 a->gauche = pivot->droit;
   	 pivot->droit = a;

   	 // Mise à jour de la hauteur
   	 a->hauteur = 1 + max(height(a->gauche), height(a->droit));
   	 pivot->hauteur = 1 + max(height(pivot->gauche), height(pivot->droit));

    	return pivot;
}

// Fonction de double rotation gauche
Trajet* doubleRotationGauche(Trajet *a) {
    	a->droit = rotateRight(a->droit);
    	return rotateLeft(a);
}

// Fonction de double rotation droite
Trajet* doubleRotationDroite(Trajet *a) {
   	 a->gauche = rotateLeft(a->gauche);
    	return rotateRight(a);
}


// Fonction pour rééquilibrer un noeud AVL
Trajet* equilibrerAVL(Trajet *a) {
    	int balance = getBalance(a);

    	// Sous-arbre droit plus profond
    	if (balance >= 2) {
       		if (getBalance(a->droit) >= 0) {
        		return rotateLeft(a);
        	} else {
            		return doubleRotationGauche(a);
        	}
    	}
    	// Sous-arbre gauche plus profond
    	else if (balance <= -2) {
        	if (getBalance(a->gauche) <= 0) {
            		return rotateRight(a);
        	} else {
            		return doubleRotationDroite(a);
        	}
    	}

    	return a;
}



Trajet * modificationNoeud(Trajet* trajet, float km){
	
	if(km > trajet->distance_maxi){
		trajet->distance_maxi = km;
	}
	else if(km < trajet->distance_mini){
		trajet->distance_mini = km;
	}
	trajet->valeur = (trajet->distance_maxi) - (trajet->distance_mini);
	trajet->moyenne = ((trajet->moyenne)*(trajet->nombre_etape) + km) / (trajet->nombre_etape + 1);
	trajet->nombre_etape+=1;
	
	return trajet;
}

void afficherAVL(Trajet* root) {
    if (root != NULL) {
        afficherAVL(root->gauche);       // Visiter le sous-arbre gauche
        printf("%f ", root->valeur);    // Afficher la valeur du nœud
        afficherAVL(root->droit);      // Visiter le sous-arbre droit
    }
}




Trajet* insertionAVL(Trajet* a, Trajet* noeud, float km, int* h) {
    	if (a == NULL) {	
    		
      	   	*h = 1; // Arbre a grandi en hauteur
        	return noeud;
    	}
	
    	if (noeud->trajet < a->trajet) {	
        	a->gauche = insertionAVL(a->gauche, noeud,km, h);
        	
    	} else if (noeud->trajet > a->trajet) {
        	a->droit = insertionAVL(a->droit, noeud,km, h);
        	
    	} else if (noeud->trajet == a->trajet) { 		
    		a = modificationNoeud(a, km);
    	}	
    	a->hauteur = 1 + max(height(a->gauche), height(a->droit));

    	if (*h != 0) {
        	a = equilibrerAVL(a);
        	*h = (getBalance(a) == 0) ? 0 : 1;
    	}
    	
    	return a;
}




Trajet* creerNoeud(int trajet_id,float kilometrage) {
    	Trajet* nouveau = (Trajet*)malloc(sizeof(Trajet)); // Allocation de mémoire
	
    	if (nouveau != NULL) { // Vérifier si l'allocation a réussi
    		nouveau->trajet = trajet_id; // Initialiser trajet (ou avec une autre valeur appropriée)
        	nouveau->distance_maxi = kilometrage; // Utiliser la valeur de kilométrage fournie
        	nouveau->distance_mini = kilometrage; // Initialiser à zéro ou une autre valeur appropriée
        	nouveau->nombre_etape = 1; // Initialiser à zéro ou une autre valeur appropriée
        	nouveau->moyenne = kilometrage; // Initialiser à zéro ou une autre valeur appropriée
        	nouveau->valeur = 0; // Initialiser à zéro ou une autre valeur appropriée
        	nouveau->droit = NULL; // Aucun enfant droit au début
        	nouveau->gauche = NULL; // Aucun enfant gauche au début
        	nouveau->hauteur = 1; // Hauteur initiale pour un nœud feuille
    	}

    	return nouveau; // Retourner le nouveau nœud
}


int max(int a, int b) {
    	if (a > b) {
        	return a;
    	} else {
        	return b;
    	}
}

int min(int a, int b) {
    	if (a < b) {
        	return a;
    	} else {
        	return b;
    	}
}



void insererDansTop50(Trajet* topTrajets[TOP_N], Trajet* nouveauTrajet, int* nombreTrajets) {
    	if (*nombreTrajets < TOP_N) {
        	topTrajets[(*nombreTrajets)++] = nouveauTrajet;
    	} else {
        	if (nouveauTrajet->valeur <= topTrajets[TOP_N - 1]->valeur) {
            		return; // La nouvelle valeur n'est pas assez grande pour entrer dans le top 50
        	}
        	topTrajets[TOP_N - 1] = nouveauTrajet; // Remplacer le dernier élément
    }

    	// Tri naïf pour maintenir les plus grandes valeurs (cette partie peut être optimisée)
    	for (int i = 0; i < *nombreTrajets - 1; i++) {
        	for (int j = i + 1; j < *nombreTrajets; j++) {
            		if (topTrajets[i]->valeur < topTrajets[j]->valeur) {
                		Trajet* temp = topTrajets[i];
                		topTrajets[i] = topTrajets[j];
                		topTrajets[j] = temp;
            		}
        	}
    	}
}

void parcourirEtCollecterTop50(Trajet* racine, Trajet* topTrajets[TOP_N], int* nombreTrajets) {
    	if (racine == NULL) {
        	return;
    	}

    	// Parcourir l'arbre de manière récursive
    	parcourirEtCollecterTop50(racine->gauche, topTrajets, nombreTrajets);
    	insererDansTop50(topTrajets, racine, nombreTrajets);
    	parcourirEtCollecterTop50(racine->droit, topTrajets, nombreTrajets);
}


void afficherTop50(Trajet* topTrajets[TOP_N], int nombreTrajets) {
    	FILE *file = fopen("../temp/traitement-s.txt", "w"); // Open the file for writing
    	if (file == NULL) {
        	printf("Error opening file!\n");
        	return;
    	}

    	for (int i = 0; i < nombreTrajets && i < TOP_N; i++) {
        	fprintf(file, "%d;%f;%f;%f;%f\n",
                	topTrajets[i]->trajet, topTrajets[i]->distance_mini, topTrajets[i]->moyenne,
                	topTrajets[i]->distance_maxi, topTrajets[i]->valeur);
    	}

    	fclose(file); // Close the file
}




