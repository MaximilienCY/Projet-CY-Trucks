
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

Trajet* rotateRight(Trajet *a);

Trajet* doubleRotationGauche(Trajet* a);

Trajet* doubleRotationDroite(Trajet* a);

Trajet* equilibrerAVL(Trajet* a);

void modificationNoeud(Trajet* trajet, int km);

void afficherAVL(Trajet* root);

Trajet* suppressionAVL(Trajet* a, float e, int* h);

Trajet* suppMinAVL(Trajet* a, int* h, float* pe);

Trajet* insertionAVL(Trajet* a, Trajet* noeud, int* h);

Trajet* recherche(Trajet* a, int e);

Trajet* creerNoeud(int trajet_id,float kilometrage);

void parcoursOrdreInverseLimite(Trajet* racine, int* compteur);

void afficherTop50(Trajet* racine);

int max(int a, int b);

int min(int a, int b);

#endif
