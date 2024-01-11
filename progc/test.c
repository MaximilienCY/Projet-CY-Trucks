#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_VILLE 35
#define MAX_VILLES 10


// Structure pour stocker les informations sur une ville
typedef struct ApparitionVILLE {
    char nom[TAILLE_VILLE];
    int totalVisites;
    struct ApparitionVILLE *gauche;
    struct ApparitionVILLE *droite;
    int hauteur;
} ApparitionVILLE;


// Fonction utilitaire pour retourner le maximum entre deux entiers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fonction pour calculer la hauteur d'un nœud dans l'arbre
int hauteur(ApparitionVILLE *N) {
    if (N == NULL)
        return 0;
    return N->hauteur;
}

// Crée un nouveau nœud pour une ville avec son nom et initialise les données
ApparitionVILLE *nouvelleVille(char *nom) {
    ApparitionVILLE *ville = (ApparitionVILLE *)malloc(sizeof(ApparitionVILLE));
    strcpy(ville->nom, nom);
    ville->totalVisites = 1;
    ville->gauche = NULL;
    ville->droite = NULL;
    ville->hauteur = 1;
    return ville;
}

// Fonctions pour effectuer les rotations dans l'arbre AVL
ApparitionVILLE *rotationDroite(ApparitionVILLE *y) {
    ApparitionVILLE *x = y->gauche;
    ApparitionVILLE *T2 = x->droite;

    x->droite = y;
    y->gauche = T2;

    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;

    return x;
}

ApparitionVILLE *rotationGauche(ApparitionVILLE *x) {
    ApparitionVILLE *y = x->droite;
    ApparitionVILLE *T2 = y->gauche;

    y->gauche = x;
    x->droite = T2;

    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;

    return y;
}

// Fonction pour obtenir l'équilibre d'un nœud dans l'arbre
int calculEQUILIBRE(ApparitionVILLE *N) {
    if (N == NULL)
        return 0;
    return hauteur(N->gauche) - hauteur(N->droite);
}

// Fonction pour insérer une ville dans l'arbre
ApparitionVILLE *insererVille(ApparitionVILLE *noeud, char *nom) {
    if (noeud == NULL)
        return nouvelleVille(nom);

    if (strcmp(nom, noeud->nom) < 0)
        noeud->gauche = insererVille(noeud->gauche, nom);
    else if (strcmp(nom, noeud->nom) > 0)
        noeud->droite = insererVille(noeud->droite, nom);
    else {
        (noeud->totalVisites)++;
        return noeud;
    }
	
    // Mettre à jour la hauteur du nœud
    noeud->hauteur = 1 + max(hauteur(noeud->gauche), hauteur(noeud-		>droite));
    // Vérifier l'équilibre du nœud et effectuer des rotations si nécessaire
    int equilibre = calculEQUILIBRE(noeud);

    if (equilibre > 1 && strcmp(nom, noeud->gauche->nom) < 0)
        return rotationDroite(noeud);

    if (equilibre < -1 && strcmp(nom, noeud->droite->nom) > 0)
        return rotationGauche(noeud);

    if (equilibre > 1 && strcmp(nom, noeud->gauche->nom) > 0) {
        noeud->gauche = rotationGauche(noeud->gauche);
        return rotationDroite(noeud);
    }

    if (equilibre < -1 && strcmp(nom, noeud->droite->nom) < 0) {
        noeud->droite = rotationDroite(noeud->droite);
        return rotationGauche(noeud);
    }

    return noeud;
}

// Parcours l'arbre pour récupérer les 10 villes avec le plus de visites
void parcoursPrefixe(ApparitionVILLE *racine, ApparitionVILLE *topVilles[], int *nbVilles) {
    if (racine != NULL) {
        parcoursPrefixe(racine->gauche, topVilles, nbVilles);

        if (*nbVilles < MAX_VILLES) {
            topVilles[(*nbVilles)++] = racine;
        } else {
            int minVisites = topVilles[0]->totalVisites;
            int minIndex = 0;

            for (int i = 1; i < MAX_VILLES; i++) {
                if (topVilles[i]->totalVisites < minVisites) {
                    minVisites = topVilles[i]->totalVisites;
                    minIndex = i;
                }
            }

            if (racine->totalVisites > minVisites) {
                topVilles[minIndex] = racine;
            }
        }

        parcoursPrefixe(racine->droite, topVilles, nbVilles);
    }
}

int comparerVisites(const void *a, const void *b) {
    const ApparitionVILLE *villeA = *(const ApparitionVILLE **)a;
    const ApparitionVILLE *villeB = *(const ApparitionVILLE **)b;
    return (villeB->totalVisites - villeA->totalVisites);
}

int main() {
    FILE *fichier;
    char ligne[1024];

    ApparitionVILLE *racine = NULL;

    fichier = fopen("data.csv", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    while (fgets(ligne, sizeof(ligne), fichier)) {
        char *token;
        char *colonnes[5];

        int colonne = 0;
        token = strtok(ligne, ";");
        while (token != NULL && colonne < 5) {
            colonnes[colonne++] = token;
            token = strtok(NULL, ";");
        }

        if (colonne >= 4) {
            char *ville = colonnes[3];

            if (strlen(ville) < TAILLE_VILLE) {
                racine = insererVille(racine, ville);
            }
        }

        if (colonne >= 2 && atoi(colonnes[1]) == 1 && colonne >= 3) {
            char *villeDepart = colonnes[2];

            if (strlen(villeDepart) < TAILLE_VILLE) {
                racine = insererVille(racine, villeDepart);
            }
        }
    }

    fclose(fichier);

    ApparitionVILLE *topVilles[MAX_VILLES];
    int nbVillesTop = 0;

    parcoursPrefixe(racine, topVilles, &nbVillesTop);

    qsort(topVilles, nbVillesTop, sizeof(ApparitionVILLE *), comparerVisites);

    printf("Les 10 villes avec le plus de visites :\n");
    for (int i = 0; i < nbVillesTop && i < MAX_VILLES; i++) {
        printf("%s : Total Visites - %d\n", topVilles[i]->nom, topVilles[i]->totalVisites);
    }

    return EXIT_SUCCESS;
}

