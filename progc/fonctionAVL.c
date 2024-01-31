#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"



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



void modificationNoeud(Trajet* trajet, int km){
	if(km > trajet->distance_maxi){
		trajet->distance_maxi = km;
	}
	if(km < trajet->distance_mini){
		trajet->distance_mini = km;
	}
	trajet->valeur = (trajet->distance_maxi - trajet->distance_mini);
	trajet->moyenne = ((trajet->moyenne)*(trajet->nombre_etape) + km) / (trajet->nombre_etape + 1);
	trajet->nombre_etape+=1;
	
}

void afficherAVL(Trajet* root) {
    if (root != NULL) {
        afficherAVL(root->gauche);       // Visiter le sous-arbre gauche
        printf("%f ", root->valeur);    // Afficher la valeur du nœud
        afficherAVL(root->droit);      // Visiter le sous-arbre droit
    }
}



//FONCTION SuppressionAVL
Trajet* suppressionAVL(Trajet* a, float e, int* h) {
    	Trajet* tmp;
    	if (a == NULL) {
        	*h = 1;
        	return a;
    	}
    	if (e > a->valeur) {
    		
        	a->droit = suppressionAVL(a->droit, e, h);
    	} else if (e < a->valeur) {
    		
        	a->gauche = suppressionAVL(a->gauche, e, h);
        	*h = -*h;
    	} else {
    		
        	if (a->droit != NULL) {
            		a->droit = suppMinAVL(a->droit, h, &(a->valeur));
        	} else {
        		
            		tmp = a;
            		a = a->gauche;
            		free(tmp);
            		*h = -1;
      
        	}
    	}
	
    	if (*h != 0 && a != NULL) {
    		
        	a->hauteur = 1 + max(height(a->gauche), height(a->droit));
        	
        	a = equilibrerAVL(a);
        	*h = (getBalance(a) == 0) ? 0 : 1;
    	}
    	
    	return a;
}

//FONCTION suppMinAVL
Trajet* suppMinAVL(Trajet* a, int* h, float* pe) {
    	Trajet* tmp;
    	if (a->gauche == NULL) {
    		
        	*pe = a->valeur;
        	*h = -1;
        	tmp = a;
        	a = a->droit;
        	free(tmp);
        	return a;
    	} else {
        	a->gauche = suppMinAVL(a->gauche, h, pe);
        	*h = -*h;
    	}

    	if (*h != 0) {
    		
        	a->hauteur = 1 + max(height(a->gauche), height(a->droit));
        	a = equilibrerAVL(a);
        	*h = (getBalance(a) == 0) ? -1 : 0;
    	}
    	return a;
}


Trajet* insertionAVL(Trajet* a, Trajet* noeud, int* h) {
    	if (a == NULL) {	
      	   	*h = 1; // Arbre a grandi en hauteur
        	return noeud;
    	}
	
    	if (noeud->valeur < a->valeur) {
    		printf("b"); 		
        	a->gauche = insertionAVL(a->gauche, noeud, h);
    	} else if (noeud->valeur > a->valeur) {
    		printf("a"); 	
        	a->droit = insertionAVL(a->droit, noeud, h);
    	} else {
    		printf("c"); 
        	*h = 0; // Valeur déjà présente, pas d'insertion
        	return a;
    	}

    	a->hauteur = 1 + max(height(a->gauche), height(a->droit));

    	if (*h != 0) {
        	a = equilibrerAVL(a);
        	*h = (getBalance(a) == 0) ? 0 : 1;
    	}
    	return a;
}

//FONCTION RECHERCHE
Trajet* recherche(Trajet* a, int e) {

    	if (a == NULL) {
    		
     	   	return NULL; // Cas de base : nœud est NULL
    	}

    	if (a->trajet == e) {
    	    	return a; // Nœud trouvé
    	}

    	// D'abord, recherche dans le sous-arbre gauche
    	Trajet* resultGauche = recherche(a->gauche, e);
    	if (resultGauche != NULL) {
    	    	return resultGauche; // Nœud trouvé dans le sous-arbre gauche
   	 }

    	// Ensuite, recherche dans le sous-arbre droit
    	return recherche(a->droit, e);
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



void parcoursOrdreInverseLimite(Trajet* racine, int* compteur) {
    	if (racine == NULL || *compteur >= 50) {	
        	return; // Arrêter si l'arbre est vide ou si 50 nœuds ont été visités
    	}

    	// Parcourir d'abord le sous-arbre droit
    	parcoursOrdreInverseLimite(racine->droit, compteur);

    	if (*compteur < 50) {

        	// Afficher la valeur du nœud actuel
        	printf("%d ", racine->trajet); // ou toute autre valeur que vous voulez afficher
        	(*compteur)++; // Incrémenter le compteur
    	}

    	// Parcourir ensuite le sous-arbre gauche
    	parcoursOrdreInverseLimite(racine->gauche, compteur);
}

// Fonction pour initier le parcours
void afficherTop50(Trajet* racine) {
    	int compteur = 0;
    	parcoursOrdreInverseLimite(racine, &compteur);
}


