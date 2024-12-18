#ifndef AVL_H
#define AVL_H

#include <stdio.h>

typedef struct node{
  int id;
  int id_centrale;
  long long capacite;
  long long consommation;
  int hauteur;
  struct node * droite;
  struct node *gauche;
}NoeudAVL;

typedef struct {
    int id;
    long long capacite;
    long long consommation;
} NodeInfo;

// Déclaration des fonctions
int max(int a, int b);
NoeudAVL *creerNoeud(char *elt[]);
void choisirNomFichier(char *nom_fichier,char *nom_fichier2);
NoeudAVL *rotationGauche(NoeudAVL *n);
NoeudAVL *rotationDroite(NoeudAVL *n);
NoeudAVL *inserer(NoeudAVL *n, NoeudAVL *nouveau);
NoeudAVL *rechercher(NoeudAVL *n, int id);
NoeudAVL *lire_fichier_station(FILE *fic);
void lire_fichier_consommateur(FILE *fic, NoeudAVL *n);
void ecrire_parcours_inf(NoeudAVL *n, FILE *fichier);
void collectNodes(NoeudAVL *node, NodeInfo *array, int *index);
void bubbleSort(NodeInfo *array, int size);
void processTopAndBottomLV(NoeudAVL *root, const char *outputFile);
void liberer_arbre(NoeudAVL *n);

#endif
