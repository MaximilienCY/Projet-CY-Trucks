
#ifndef AVL_H
#define AVL_H
#define TOP_N 50
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

Trajet* modificationNoeud(Trajet* trajet, float km);

void afficherAVL(Trajet* root);

Trajet* insertionAVL(Trajet* a, Trajet* noeud,float km, int* h);

Trajet* creerNoeud(int trajet_id,float kilometrage);

void afficherTop50(Trajet* topTrajets[TOP_N], int nombreTrajets);

void parcourirEtCollecterTop50(Trajet* racine, Trajet* topTrajets[TOP_N], int* nombreTrajets);

void insererDansTop50(Trajet* topTrajets[TOP_N], Trajet* nouveauTrajet, int* nombreTrajets);

int max(int a, int b);

int min(int a, int b);

#endif
