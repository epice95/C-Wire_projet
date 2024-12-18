#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "avl.h"


int max(int a , int b){
  return (a>b)?a:b;
}

NoeudAVL *creerNoeud(char *elt[]) {
    // Robustesse : Vérification des paramètres d'entrée
    if (elt == NULL) {
        fprintf(stderr, "Erreur : Paramètre elt est NULL\n");
        return NULL;
    }	
    // Allocation mémoire pour le nœud
    NoeudAVL *n = (NoeudAVL *)malloc(sizeof(NoeudAVL));
    if (n == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL; // Robustesse : Gestion de l'erreur d'allocation
    }
    // Extraction des id et du type en fonction des conditions
    if (atoi(elt[3]) != 0) {
        n->id =atoi(elt[3]);
    } else if (atoi(elt[2]) != 0) {
        n->id =atoi(elt[2]);
    } else if(atoi(elt[1]) != 0) {
        n->id =atoi(elt[1]);
    }
    // Initialisation de la capacité
    n->capacite = atoll(elt[6]);
    n->id_centrale = atoi(elt[0]);
    // Initialisation de la hauteur
    n->hauteur = 0;
    return n;
}


// Fonction pour choisir le nom du fichier selon l'entrée de l'utilisateur
void choisirNomFichier(char *nom_fichier, char *nom_fichier2) {
    FILE *fic = fopen("tmp/temp_usager.txt", "r");  // Ouvre le fichier consommateur
    if (fic == NULL) {
        perror("Erreur lors de l'ouverture du fichier temp_usager.txt");
        return;
    }

    char ligne[256];
    char *elt1 = (char *)malloc(256 * sizeof(char));
    char *elt2 = (char *)malloc(256 * sizeof(char));
    char *elt3 = (char *)malloc(256 * sizeof(char));
    char *elt4 = (char *)malloc(256 * sizeof(char));
    char *elt5 = (char *)malloc(256 * sizeof(char));
    char *elt6 = (char *)malloc(256 * sizeof(char));
    char *elt7 = (char *)malloc(256 * sizeof(char));
    char *elt8 = (char *)malloc(256 * sizeof(char));

    // Lire la première ligne pour analyser le format
    if (fgets(ligne, sizeof(ligne), fic) != NULL) {
        int items_read = sscanf(ligne, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]",
                                 elt1, elt2, elt3, elt4, elt5, elt6, elt7, elt8);
        if (items_read == 8) {
            // Vérifier quel type de fichier c'est et choisir le nom
            if (atoi(elt3) != 0) {  // Par exemple, si elt3 a une valeur non nulle
                strcpy(nom_fichier, "hva_comp.csv");
                strcpy(nom_fichier2, "Station HV-A");
            } else if (atoi(elt2) != 0) {  // Si elt2 a une valeur
                strcpy(nom_fichier, "hvb_comp.csv");
                strcpy(nom_fichier2, "Station HV-B");
            } else if (atoi(elt5) != 0) {  // Vérifier un autre champ pour "lv_all"
                strcpy(nom_fichier, "lv_all.csv");
                strcpy(nom_fichier2, "Station LV");
            } else {
                strcpy(nom_fichier, "lv_indiv.csv");
                strcpy(nom_fichier2, "Station LV");
            }
        } else {
            printf("Erreur de format dans la ligne: %s\n", ligne);
        }
    }
    // Libérer la mémoire
    free(elt1);
    free(elt2);
    free(elt3);
    free(elt4);
    free(elt5);
    free(elt6);
    free(elt7);
    free(elt8);
    
    fclose(fic);  // Fermer le fichier
    
}

NoeudAVL * rotationGauche(NoeudAVL * n) {
    // Robustesse : Vérification du paramètre d'entrée
    if (n == NULL) {
        fprintf(stderr, "Erreur : Nœud NULL passé à rotationGauche\n");
        return NULL;
    }

    if (n->droite == NULL) {
        fprintf(stderr, "Erreur : Rotation gauche impossible, le sous-arbre droit est NULL\n");
        return n; // Retourne le nœud actuel, aucune rotation n'est effectuée
    }

    NoeudAVL * temp = n->droite;   // Le sous-arbre droit devient la nouvelle racine.
    n->droite = temp->gauche;      // Le sous-arbre gauche de temp devient le sous-arbre droit de n.
    temp->gauche = n;              // n devient le sous-arbre gauche de temp.
    
    // Mise à jour des hauteurs
    n->hauteur = 1 + max((n->gauche ? n->gauche->hauteur : -1), (n->droite ? n->droite->hauteur : -1));
    temp->hauteur = 1 + max((temp->gauche ? temp->gauche->hauteur : -1), (temp->droite ? temp->droite->hauteur : -1));
    
    return temp;  // Retourne la nouvelle racine (temp).
}

NoeudAVL * rotationDroite(NoeudAVL * n) {
    //Robustesse : Vérification du paramètre d'entrée
    if (n == NULL) {
        fprintf(stderr, "Erreur : Nœud NULL passé à rotationDroite\n");
        return NULL;
    }

    if (n->gauche == NULL) {
        fprintf(stderr, "Erreur : Rotation droite impossible, le sous-arbre gauche est NULL\n");
        return n; // Retourne le nœud actuel, aucune rotation n'est effectuée
    }
    
    NoeudAVL * temp = n->gauche;   // Le sous-arbre gauche devient la nouvelle racine.
    n->gauche = temp->droite;      // Le sous-arbre droit de temp devient le sous-arbre gauche de n.
    temp->droite = n;              // n devient le sous-arbre droit de temp.
    
    // Mise à jour des hauteurs
    n->hauteur = 1 + max((n->gauche ? n->gauche->hauteur : -1), (n->droite ? n->droite->hauteur : -1));
    temp->hauteur = 1 + max((temp->gauche ? temp->gauche->hauteur : -1), (temp->droite ? temp->droite->hauteur : -1));
    
    return temp;
}


NoeudAVL* inserer(NoeudAVL* n, NoeudAVL* nouveau) {
    // Robustesse : Vérification des paramètres d'entrée
    if (nouveau == NULL) {
        fprintf(stderr, "Erreur : Nœud à insérer est NULL\n");
        return n; // Retourne le nœud actuel sans modification
    }
  
    
    if (n == NULL) {
        return nouveau;
    }
    if (nouveau->id < n->id) {
        n->gauche = inserer(n->gauche, nouveau);
    } else if (nouveau->id > n->id) {
        n->droite = inserer(n->droite, nouveau);
    } else {
        return n;
    }
    // Mise à jour de la hauteur du nœud courant.
    n->hauteur = 1 + max((n->gauche ? n->gauche->hauteur : -1),(n->droite ? n->droite->hauteur : -1));
    // Calcul du facteur d'équilibre (droite - gauche)
    int balance = (n->droite ? n->droite->hauteur : -1) - (n->gauche ? n->gauche->hauteur : -1);
    // Cas 1 : Déséquilibre à droite.
    if (balance > 1 && nouveau->id > n->droite->id) {
        return rotationGauche(n);
    }
    // Cas 2 : Déséquilibre à gauche.
    if (balance < -1 && nouveau->id < n->gauche->id) {
        return rotationDroite(n);
    }
    // Cas 3 : Déséquilibre droite-gauche.
    if (balance > 1 && nouveau->id < n->droite->id) {
        n->droite = rotationDroite(n->droite);
        return rotationGauche(n);
    }
    // Cas 4 : Déséquilibre gauche-droite.
    if (balance < -1 && nouveau->id > n->gauche->id) {
        n->gauche = rotationGauche(n->gauche);
        return rotationDroite(n);
    }
    return n; // Retourne la nouvelle racine locale.
}



NoeudAVL *rechercher(NoeudAVL *n, int id) {
    // Robustesse : Vérification du paramètre d'entrée
    if (n == NULL) {
        fprintf(stderr, "Erreur : Arbre ou sous-arbre NULL dans rechercher\n");
        return NULL;
    }

    if (n == NULL) {
        return NULL; // retourne n car n est NULL
    }
    if (n->id == id) {
        return n; // si l'id est trouvé, alors on retourne le noeud
    }
    if (id < n->id) {
        return rechercher(n->gauche, id); // si l'id est plus petit, on effectue le parcourt à gauche de l'AVL
    }
    return rechercher(n->droite, id); // si l'id est plus grand, on effectue le parcourt à droite de l'AVL
}


NoeudAVL * lire_fichier_station(FILE *fic) {
    // Robustesse : Vérification de l'entrée
    if (fic == NULL) {
        fprintf(stderr, "Erreur : Fichier NULL passé à lire_fichier_station\n");
        return NULL;
    }

    char ligne[256];
    // Déclaration des tableaux de chaînes pour chaque élément
    char *elt1 = (char*)malloc(256 * sizeof(char));
    char *elt2 = (char*)malloc(256 * sizeof(char));
    char *elt3 = (char*)malloc(256 * sizeof(char));
    char *elt4 = (char*)malloc(256 * sizeof(char));
    char *elt5 = (char*)malloc(256 * sizeof(char));
    char *elt6 = (char*)malloc(256 * sizeof(char));
    char *elt7 = (char*)malloc(256 * sizeof(char));
    char *elt8 = (char*)malloc(256 * sizeof(char));
    NoeudAVL *n = NULL;
    if (fic == NULL) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }
    // Lire une ligne du fichier
    if (fgets(ligne, sizeof(ligne), fic) != NULL) {
        // Extraction des éléments de la ligne en utilisant sscanf
        int items_read = sscanf(ligne, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]",elt1, elt2, elt3, elt4, elt5, elt6, elt7, elt8);
        // Vérification si tous les champs ont été lus correctement
        if (items_read == 8) {
            char *element[] = {elt1, elt2, elt3, elt4, elt5, elt6, elt7, elt8};
              n = creerNoeud(element);
        } else {
            printf("Erreur de format dans la ligne: %s\n", ligne);
        }
    }
    // Libération de la mémoire allouée pour les éléments
    free(elt1);
    free(elt2);
    free(elt3);
    free(elt4);
    free(elt5);
    free(elt6);
    free(elt7);
    free(elt8);
    return n;
}


void lire_fichier_consommateur(FILE *fic, NoeudAVL *n) {
    // Robustesse : Vérification des paramètres d'entrée
    if (fic == NULL) {
        fprintf(stderr, "Erreur : Fichier NULL passé à lire_fichier_consommateur\n");
        return;
    }
    if (n == NULL) {
        fprintf(stderr, "Erreur : Arbre AVL NULL passé à lire_fichier_consommateur\n");
        return;
    }

    char ligne[256];
    char *elt1 = (char *)malloc(256 * sizeof(char));
    char *elt2 = (char *)malloc(256 * sizeof(char));
    char *elt3 = (char *)malloc(256 * sizeof(char));
    char *elt4 = (char *)malloc(256 * sizeof(char));
    char *elt5 = (char *)malloc(256 * sizeof(char));
    char *elt6 = (char *)malloc(256 * sizeof(char));
    char *elt7 = (char *)malloc(256 * sizeof(char));
    char *elt8 = (char *)malloc(256 * sizeof(char));
    int id;
    if (fic == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }
    // Lire chaque ligne du fichier jusqu'à EOF
    while (fgets(ligne, sizeof(ligne), fic) != NULL) {
        // Extraction des éléments de la ligne en utilisant sscanf
        int items_read = sscanf(ligne, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]",elt1, elt2, elt3, elt4, elt5, elt6, elt7, elt8);
        if (items_read == 8) {
            char *element[] = {elt1, elt2, elt3, elt4, elt5, elt6, elt7, elt8};
            if (atoi(element[3]) != 0) {
                id = atoi(element[3]);
            } else if (atoi(element[2]) != 0) {
                id = atoi(element[2]);
            } else if (atoi(element[1]) != 0) {
                id = atoi(element[1]);
            }
            // Rechercher le nœud correspondant dans l'AVL et mettre à jour sa consommation
            NoeudAVL *noeud = rechercher(n, id);
            if (noeud != NULL) {
                noeud->consommation += atoll(element[7]);
            } else {
                printf("Aucun nœud trouvé avec l'ID %d\n", id);
            }
        } else {
            printf("Erreur de format dans la ligne : %s\n", ligne);
        }
    }
    // Libération de la mémoire allouée pour les éléments
    free(elt1);
    free(elt2);
    free(elt3);
    free(elt4);
    free(elt5);
    free(elt6);
    free(elt7);
    free(elt8);
}

// Fonction pour effectuer un parcours infixe et écrire dans un fichier CSV
void ecrire_parcours_inf(NoeudAVL *n, FILE *fichier) {
    if (n == NULL) {
        return;
    }
    ecrire_parcours_inf(n->gauche, fichier);
    fprintf(fichier, "%d:%lld:%lld\n", n->id, n->capacite, n->consommation);
    ecrire_parcours_inf(n->droite, fichier);
}

// Fonction qui collecte les noeuds et leurs consommations dans un tableau via un parcourt infixe de l'AVL
void collectNodes(NoeudAVL *node, NodeInfo *array, int *index) {
    if (node == NULL) return;
    collectNodes(node->gauche, array, index);
    array[*index].id = node->id;
    array[*index].capacite = node->capacite;
    array[*index].consommation = node->consommation;
    (*index)++;
    collectNodes(node->droite, array, index);
}

// Tri bulle par consommation (decroissant)
void bubbleSort(NodeInfo *array, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j].consommation < array[j + 1].consommation) {
                NodeInfo temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

// Fonction qui écrit dans le fichier les 10 stations avec la consommation maximum et les 10 station avec la consommation minimum dans l'ordre décroissant
void processTopAndBottomLV(NoeudAVL *root, const char *outputFile) {
    // Compte le nombre de noeuds pour l'allocation dynamique qui va suivre 
    int nodeCount = 0;
    void countNodes(NoeudAVL *n) {
        if (n == NULL) return;
        countNodes(n->gauche);
        nodeCount++;
        countNodes(n->droite);
    }
    countNodes(root);

    // Tableau dynamique de type NodeInfo à travers une allocation dynamique
    NodeInfo *nodes = (NodeInfo *)malloc(sizeof(NodeInfo) * nodeCount);
    if (nodes == NULL) {
        perror("Allocation error");
        return;
    }

    // Populate the array
    int index = 0;
    collectNodes(root, nodes, &index);

    // Tri bulle effectuant le tri par ordre décroissant de consommation
    bubbleSort(nodes, nodeCount);

    // Ouvre le fichier outputFile
    FILE *file = fopen(outputFile, "w");
    if (file == NULL) {
        perror("Error opening output file");
        free(nodes);
        return;
    }

    // Ecrit la ligne d'entête
    fprintf(file, "Station LV,Capacite,Consommation(max,min)\n");

    // Ecrit le top 10 des noeuds avec les consommations les plus fortes
    for (int i = 0; i < 10 && i < nodeCount; i++) {
        fprintf(file, "%d,%lld,%lld\n", nodes[i].id, nodes[i].capacite, nodes[i].consommation);
    }

    // Ecrit le top 10 des noeuds avec les consommations les plus faibles
    for (int i = nodeCount - 10; i < nodeCount; i++) {
        if (i >= 0) {
            fprintf(file, "%d,%lld,%lld\n", nodes[i].id, nodes[i].capacite, nodes[i].consommation);
        }
    }

    fclose(file);
    free(nodes);
}


// Comparateur pour trier les nœuds par capacité croissante
int compareByCapacity(const void *a, const void *b) {
    NodeInfo *nodeA = (NodeInfo *)a;
    NodeInfo *nodeB = (NodeInfo *)b;
    if (nodeA->capacite < nodeB->capacite) return -1;
    if (nodeA->capacite > nodeB->capacite) return 1;
    return 0;
}

// Fonction pour collecter les nœuds et les trier
void collectAndSortByCapacity(NoeudAVL *root, const char *outputFile) {
    // Compter les nœuds pour allouer un tableau
    int nodeCount = 0;

    void countNodes(NoeudAVL *node) {
        if (node == NULL) return;
        countNodes(node->gauche);
        nodeCount++;
        countNodes(node->droite);
    }
    countNodes(root);

    // Allouer un tableau pour les nœuds
    NodeInfo *nodes = (NodeInfo *)malloc(sizeof(NodeInfo) * nodeCount);
    if (nodes == NULL) {
        perror("Erreur d'allocation mémoire");
        return;
    }

    // Collecter les nœuds dans le tableau
    int index = 0;
    collectNodes(root, nodes, &index);

    // Trier le tableau par capacité
    qsort(nodes, nodeCount, sizeof(NodeInfo), compareByCapacity);

    // Écrire les données triées dans le fichier CSV
    FILE *file = fopen(outputFile, "a");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier CSV");
        free(nodes);
        return;
    }

    for (int i = 0; i < nodeCount; i++) {
        fprintf(file, "%d:%lld:%lld\n", nodes[i].id, nodes[i].capacite, nodes[i].consommation);
    }

    fclose(file);
    free(nodes);
}

// Fonction pour libérer l'arbre AVL
void liberer_arbre(NoeudAVL *n) {
    if (n == NULL) return;
    liberer_arbre(n->gauche);
    liberer_arbre(n->droite);
    free(n);
}
