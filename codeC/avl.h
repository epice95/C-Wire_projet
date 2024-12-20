#ifndef AVL_H
#define AVL_H

#include <stdio.h>


// Déclaration de la structure NoeudAVL qui définit une station de type NoeudAVL
typedef struct node{
  int id;
  int id_centrale;
  long long capacite;
  long long consommation;
  int hauteur;
  struct node * droite;
  struct node *gauche;
}NoeudAVL;

// Déclaration de la structure NodeInfo qui permet de gerer les postes LV dans le cas lv all
typedef struct {
    int id;
    long long capacite;
    long long consommation;
} NodeInfo;

// Déclaration des fonctions
int max(int a, int b); // Retourne le maximum entre deux entiers 
NoeudAVL *creerNoeud(char *elt[]); // Créé un noeud de type NoeudAVL
void choisirNomFichier(char *nom_fichier,char *nom_fichier2); // Choisi automatiquement le nom du fichier de sortie en fonction de la station
NoeudAVL *rotationGauche(NoeudAVL *n); // Retourne l'AVL après une rotation simple gauche
NoeudAVL *rotationDroite(NoeudAVL *n); // Retourne l'AVL après une rotation simple droite
NoeudAVL *inserer(NoeudAVL *n, NoeudAVL *nouveau); // Retourne l'AVL après l'insertion d'un noeud en fonction de l'ID dans l'AVL
NoeudAVL *rechercher(NoeudAVL *n, int id); // Retourne un noeud de l'AVL en fonction de son ID à travers une recherche
NoeudAVL *lire_fichier_station(FILE *fic); // Récupère les données du fichier station généré par le script Shell
void lire_fichier_consommateur(FILE *fic, NoeudAVL *n); // Récupère les données du fichier usagers généré par le script Shell
void collecterNoeuds(NoeudAVL *node, NodeInfo *array, int *index); // Collecte les postes LV (id,capacite et consommation) dans un tableau de type NodeInfo
int compareParConsommation(const void *a, const void *b); // trie les postes LV collectés par la consommation décroissante
void MaxEtMinLV(NoeudAVL *root, const char *outputFile); // Genere le fichier de sortie csv "lv_all_minmac.csv" selon l'ordre des consommations décroissantes
int compareParCapacite(const void *a, const void *b); // Compare par capacité deux élements de type const void 
void collecterEtTrierParCapacite(NoeudAVL *root, const char *outputFile); // Extrait les stations, les tries par capacités et les affiches par ordre de capacité croissante
void genererFichierGnuPlot(NoeudAVL *racine, const char *outputFile);
void genererScriptGnuPlot(const char *dataFile, const char *outputImage);
void executerGnuPlot();
void creerGraphique(NoeudAVL *racine);
void liberer_arbre(NoeudAVL *n); // libère chaque noeuds de l'AVL pour contrecarrer les fuites mémoires 

#endif
