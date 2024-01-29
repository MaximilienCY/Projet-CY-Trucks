
#ifndef AVL_H
#define AVL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


int height(Trajet* N);

int getBalance(Trajet* N);

Trajet* rotateLeft(Trajet* a);

Trajet* rotateLeft(Trajet* a);

Trajet* doubleRotationGauche(Trajet* a);

Trajet* doubleRotationDroite(Trajet* a);

Trajet* equilibrerAVL(Trajet* a);

Trajet* recherche(Trajet* a, int e);

void modificationNoeud(Trajet* trajet, int km);

void afficherAVL(Trajet* root);

Trajet* suppressionAVL(Trajet* a, int e, int* h);

Trajet* suppMinAVL(Trajet* a, int* h, int* pe);

Trajet* insertionAVL(Arbre* a, int e, int* h)

#endif
