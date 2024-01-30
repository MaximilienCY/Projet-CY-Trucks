#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"


//Fonction OBTENIR LA HAUTEUR d'un noead
int height(Arbre* N){
	if(N==NULL){
		return 0;
	}
	return N->hauteur;
}

//FONCTION OBTENIR LE FACTEUR d'equilibrage
int getBalance(Arbre* N) {
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

//FONCTION RECHERCHE
Trajet* recherche(Trajet* a, int e) {
    if (a == NULL) {
        // L'élément n'est pas trouvé
        return NULL;
    } else if (a->trajet == e) {
        // L'élément est trouvé
        return a;
    } else if (e < a->trajet) {
        // Chercher dans le sous-arbre gauche
        return recherche(a->gauche, e);
    } else {
        // Chercher dans le sous-arbre droit
        return recherche(a->droit, e);
    }
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
Arbre* suppressionAVL(Arbre* a, int e, int* h) {
    Node* tmp;
    if (a == NULL) {
        *h = 1;
        return a;
    }

    if (e > a->value) {
        a->right = suppressionAVL(a->right, e, h);
    } else if (e < a->value) {
        a->left = suppressionAVL(a->left, e, h);
        *h = -*h;
    } else {
        if (a->right != NULL) {
            a->right = suppMinAVL(a->right, h, &(a->value));
        } else {
            tmp = a;
            a = a->left;
            free(tmp);
            *h = -1;
        }
    }

    if (*h != 0) {
        a->height = 1 + max(height(a->left), height(a->right));
        a = equilibrerAVL(a);
        *h = (getBalance(a) == 0) ? 0 : 1;
    }
    return a;
}

//FONCTION suppMinAVL
Node* suppMinAVL(Node* a, int* h, int* pe) {
    Node* tmp;
    if (a->left == NULL) {
        *pe = a->value;
        *h = -1;
        tmp = a;
        a = a->right;
        free(tmp);
        return a;
    } else {
        a->left = suppMinAVL(a->left, h, pe);
        *h = -*h;
    }

    if (*h != 0) {
        a->height = 1 + max(height(a->left), height(a->right));
        a = equilibrerAVL(a);
        *h = (getBalance(a) == 0) ? -1 : 0;
    }
    return a;
}


//FONCTION d'insertion dans l'arbre AVL
Trajet* insertionAVL(Trajet* a, int e, int* h) {
    if (e < a->value) {
        a->left = insertionAVL(a->gauche, e, h);
        *h = -*h;
    } else if (e > a->value) {
        a->right = insertionAVL(a->droit, e, h);
    } else {
        *h = 0;
        return a;
    }

    a->height = 1 + max(height(a->left), height(a->right));

    if (*h != 0) {
        a = equilibrageAVL(a);
        *h = (getBalance(a) == 0) ? 0 : 1;
    }

    return a;
}


//FONCTION RECHERCHE
Trajet* recherche(Trajet* a, int e) {
    if (a != NULL){
    	if (a->trajet == e){
    		return a;
    	}
    }
    recherche(a->left, e);
    recherche(a->right, e);
}

