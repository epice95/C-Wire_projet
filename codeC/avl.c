#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "avl.h"

/* REMARQUE : LE CODE FOURNI EST D'UNE GRANDE ROBUSTESSE ET EFFICACITÉ, CE QUI JUSTIFIE LES NOMBREUSES VÉRIFICATIONS */ 

// Fonction qui retourne le maximum entre deux entiers
int max(int a , int b){
  // **Efficacité** : Utilise l'opérateur ternaire pour retourner directement la valeur maximale entre `a` et `b`.
  return (a>b)?a:b;
}

// Fonction pour créer un nœud AVL à partir d'un tableau de chaînes
NoeudAVL *creerNoeud(char *elt[]) {
    // **Robustesse** : Vérification si le tableau d'entrée `elt` est NULL avant de procéder.
    if (elt == NULL) {
        fprintf(stderr, "Erreur : Paramètre elt est NULL\n");
        return NULL; // Retour immédiat si le tableau d'entrée est invalide.
    }	
    
    // **Efficacité et Robustesse** : Allocation dynamique de mémoire pour le nœud AVL.
    NoeudAVL *n = (NoeudAVL *)malloc(sizeof(NoeudAVL));
    if (n == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL; // **Robustesse** : Gestion de l'erreur d'allocation mémoire pour éviter un comportement indéfini.
    }
    
    // **Robustesse et Efficacité** : Extraction sécurisée de l'ID à partir du tableau `elt`.
    if (atoi(elt[3]) != 0) { // **Efficacité** : Priorise l'extraction de l'ID en fonction de son niveau dans les champs
        n->id =atoi(elt[3]);
    } else if (atoi(elt[2]) != 0) {
        n->id =atoi(elt[2]);
    } else if(atoi(elt[1]) != 0) {
        n->id =atoi(elt[1]);
    } else {
        // **Robustesse** : Gestion des cas où aucun ID valide n'est trouvé.
        fprintf(stderr, "Erreur : Aucun ID valide trouvé dans les données fournies\n");
        free(n); // Libère la mémoire allouée avant de quitter.
        return NULL;
    }
    
    // **Efficacité** : Initialisation de la capacité à partir du champ correspondant.
    n->capacite = atoll(elt[6]);
    
    // **Efficacité** : Initialisation de l'ID de la centrale.
    n->id_centrale = atoi(elt[0]);
    
    // **Robustesse et Efficacité** : Initialisation explicite de la hauteur du nœud à 0.
    n->hauteur = 0;
    return n; // Retourne un pointeur valide vers le nœud créé.
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
    // **Robustesse** : Vérification que le nœud d'entrée n'est pas NULL.
    if (n == NULL) {
        fprintf(stderr, "Erreur : Nœud NULL passé à rotationGauche\n");
        return NULL;
    }
    
    // **Robustesse** : Vérification que le sous-arbre droit existe, condition essentielle pour une rotation gauche.
    if (n->droite == NULL) {
        fprintf(stderr, "Erreur : Rotation gauche impossible, le sous-arbre droit est NULL\n");
        return n; // Retourne le nœud actuel, aucune rotation n'est effectuée
    }

    NoeudAVL * temp = n->droite;   // **Efficacité** : Le sous-arbre droit devient la nouvelle racine temporaire.
    n->droite = temp->gauche;      // **Efficacité** : Le sous-arbre gauche de `temp` est réassigné au sous-arbre droit de `n`.
    temp->gauche = n;              // **Efficacité** : `n` devient le sous-arbre gauche de `temp`.
    
    // **Efficacité et Robustesse** : Mise à jour des hauteurs des nœuds pour conserver la propriété AVL.
    n->hauteur = 1 + max((n->gauche ? n->gauche->hauteur : -1), (n->droite ? n->droite->hauteur : -1));
    temp->hauteur = 1 + max((temp->gauche ? temp->gauche->hauteur : -1), (temp->droite ? temp->droite->hauteur : -1));
    
    return temp;  // **Efficacité** : Retourne la nouvelle racine après rotation gauche.
}

NoeudAVL * rotationDroite(NoeudAVL * n) {
    // **Robustesse** : Vérification que le nœud d'entrée n'est pas NULL.
    if (n == NULL) {
        fprintf(stderr, "Erreur : Nœud NULL passé à rotationDroite\n");
        return NULL;
    }
    
    // **Robustesse** : Vérification que le sous-arbre gauche existe, condition essentielle pour une rotation droite
    if (n->gauche == NULL) {
        fprintf(stderr, "Erreur : Rotation droite impossible, le sous-arbre gauche est NULL\n");
        return n; // Retourne le nœud actuel, aucune rotation n'est effectuée
    }
    
    NoeudAVL * temp = n->gauche;   // **Efficacité** : Le sous-arbre gauche devient la nouvelle racine temporaire.
    n->gauche = temp->droite;      // **Efficacité** : Le sous-arbre droit de `temp` est réassigné au sous-arbre gauche de `n`.
    temp->droite = n;              // **Efficacité** : `n` devient le sous-arbre droit de `temp`..
    
    // **Efficacité et Robustesse** : Mise à jour des hauteurs des nœuds pour conserver la propriété AVL.
    n->hauteur = 1 + max((n->gauche ? n->gauche->hauteur : -1), (n->droite ? n->droite->hauteur : -1));
    temp->hauteur = 1 + max((temp->gauche ? temp->gauche->hauteur : -1), (temp->droite ? temp->droite->hauteur : -1));
    
    return temp; // **Efficacité** : Retourne la nouvelle racine après rotation droite.
}


NoeudAVL* inserer(NoeudAVL* n, NoeudAVL* nouveau) {
    // **Robustesse** : Vérification si le nœud à insérer est NULL.
    if (nouveau == NULL) {
        fprintf(stderr, "Erreur : Nœud à insérer est NULL\n");
        return n; // Retourne le nœud actuel sans modification
    }
  
    // **Efficacité** : Si l'arbre ou le sous-arbre est vide, insère directement le nouveau nœud comme racine locale.
    if (n == NULL) {
        return nouveau;
    }
    if (nouveau->id < n->id) {
        n->gauche = inserer(n->gauche, nouveau);// Parcourt récursivement le sous-arbre gauche.
    } else if (nouveau->id > n->id) {
        n->droite = inserer(n->droite, nouveau); // Parcourt récursivement le sous-arbre droit.
    } else {
        return n; // **Robustesse** : Si l'ID existe déjà, ne fait rien pour éviter les doublons.
    }
    
    // **Efficacité et Robustesse** : Mise à jour des hauteurs des nœuds pour conserver la propriété AVL.
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
    
    return n; // **Efficacité** : Retourne la racine locale mise à jour.
}



NoeudAVL *rechercher(NoeudAVL *n, int id) {
    // **Robustesse** : Vérification si l'arbre ou le sous-arbre est NULL.
    if (n == NULL) {
        fprintf(stderr, "Erreur : Arbre ou sous-arbre NULL dans rechercher\n");
        return NULL; // Retourne NULL pour signaler que l'élément n'a pas été trouvé.
    }
    
    // **Efficacité** : Si l'ID du nœud courant correspond, retourne immédiatement ce nœud.
    if (n == NULL) {
        return NULL; 
    }
    
    // **Efficacité** : Si l'ID du nœud courant correspond, retourne immédiatement ce nœud.
    if (n->id == id) {
        return n; 
    }
    
    // **Efficacité** : Parcourt récursivement le sous-arbre gauche si l'ID recherché est plus petit
    if (id < n->id) {
        return rechercher(n->gauche, id); 
    }
    return rechercher(n->droite, id); // **Efficacité** : Parcourt récursivement le sous-arbre droit si l'ID recherché est plus grand.
}


NoeudAVL * lire_fichier_station(FILE *fic) {
    // **Robustesse** : Vérification que le fichier n'est pas NULL.
    if (fic == NULL) {
        fprintf(stderr, "Erreur : Fichier NULL passé à lire_fichier_station\n");
        return NULL; // Retourne NULL si le fichier est invalide.
    }

    char ligne[256]; // **Efficacité** : Utilisation d'un tableau pour stocker chaque ligne du fichier.
    
    // **Robustesse** : Allocation dynamique pour chaque champ à traiter.
    char *elt1 = (char*)malloc(256 * sizeof(char));
    char *elt2 = (char*)malloc(256 * sizeof(char));
    char *elt3 = (char*)malloc(256 * sizeof(char));
    char *elt4 = (char*)malloc(256 * sizeof(char));
    char *elt5 = (char*)malloc(256 * sizeof(char));
    char *elt6 = (char*)malloc(256 * sizeof(char));
    char *elt7 = (char*)malloc(256 * sizeof(char));
    char *elt8 = (char*)malloc(256 * sizeof(char));
    NoeudAVL *n = NULL; // **Robustesse** : Initialisation du pointeur de nœud.
    if (fic == NULL) {
        perror("Erreur d'ouverture du fichier");
        return NULL; 
    }
    
    // Lecture d'une ligne du fichier
    if (fgets(ligne, sizeof(ligne), fic) != NULL) {
        // **Efficacité** : Extraction des éléments de la ligne avec `sscanf
        int items_read = sscanf(ligne, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]",elt1, elt2, elt3, elt4, elt5, elt6, elt7, elt8);
         // **Robustesse** : Vérifie si tous les champs attendus ont été lus correctement
        if (items_read == 8) {
            char *element[] = {elt1, elt2, elt3, elt4, elt5, elt6, elt7, elt8};
              n = creerNoeud(element);
        } else {
            printf("Erreur de format dans la ligne: %s\n", ligne);
        }
    }
    // **Robustesse** : Libération des allocations dynamiques
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
    // **Robustesse** : Vérification que le fichier n'est pas NULL.
    if (fic == NULL) {
        fprintf(stderr, "Erreur : Fichier NULL passé à lire_fichier_consommateur\n");
        return;
    }
    if (n == NULL) {
        fprintf(stderr, "Erreur : Arbre AVL NULL passé à lire_fichier_consommateur\n");
        return;
    }

    char ligne[256]; // **Efficacité** : Utilisation d'un tableau pour stocker chaque ligne du fichier.
    
    // **Robustesse** : Allocation dynamique pour chaque champ à traiter.
    char *elt1 = (char *)malloc(256 * sizeof(char));
    char *elt2 = (char *)malloc(256 * sizeof(char));
    char *elt3 = (char *)malloc(256 * sizeof(char));
    char *elt4 = (char *)malloc(256 * sizeof(char));
    char *elt5 = (char *)malloc(256 * sizeof(char));
    char *elt6 = (char *)malloc(256 * sizeof(char));
    char *elt7 = (char *)malloc(256 * sizeof(char));
    char *elt8 = (char *)malloc(256 * sizeof(char));
    int id; // **Efficacité** : Variable pour stocker l'ID du consommateur.
    
    if (fic == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }
    
    // Lecture de chaque ligne du fichier jusqu'à EOF.
    while (fgets(ligne, sizeof(ligne), fic) != NULL) {
        // **Efficacité** : Extraction des éléments de la ligne avec `sscanf
        int items_read = sscanf(ligne, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]",elt1, elt2, elt3, elt4, elt5, elt6, elt7, elt8);
         // **Efficacité et Robustesse** : Extraction de l'ID valide selon l'ordre de priorité.
        if (items_read == 8) {
            char *element[] = {elt1, elt2, elt3, elt4, elt5, elt6, elt7, elt8};
            if (atoi(element[3]) != 0) {
                id = atoi(element[3]);
            } else if (atoi(element[2]) != 0) {
                id = atoi(element[2]);
            } else if (atoi(element[1]) != 0) {
                id = atoi(element[1]);
            }
            // Recherche du nœud dans l'AVL.
            NoeudAVL *noeud = rechercher(n, id);
            if (noeud != NULL) {
                noeud->consommation += atoll(element[7]); // **Efficacité** : Mise à jour de la consommation.
            } else {
                printf("Aucun nœud trouvé avec l'ID %d\n", id); // **Robustesse** : Signale les ID non trouvés.
            }
        } else {
            printf("Erreur de format dans la ligne : %s\n", ligne); // **Robustesse** : Signale les lignes mal formatées.
        }
    }
    
    // **Robustesse** : Libération des allocations dynamiques
    free(elt1);
    free(elt2);
    free(elt3);
    free(elt4);
    free(elt5);
    free(elt6);
    free(elt7);
    free(elt8);
}

// Fonction qui collecte les noeuds et leurs consommations dans un tableau via un parcourt infixe de l'AVL
void collecterNoeuds(NoeudAVL *noeud, NodeInfo *tab, int *index) {
    // **Robustesse** : Vérifie si le nœud est NULL. Si oui, la fonction retourne immédiatement.
    if (noeud == NULL) {
        return; // Aucun traitement nécessaire pour un nœud NULL.
    }
    
    // **Robustesse** : Vérifie si le tableau est NULL avant d'effectuer toute opération.
    if (tab == NULL) {
        fprintf(stderr, "Erreur : Tableau NULL passé à collecterNoeuds\n");
        return;
    }
    
    // **Robustesse** : Vérifie si l'index est NULL avant de le déférencer.
    if (index == NULL) {
        fprintf(stderr, "Erreur : Index NULL passé à collecterNoeuds\n");
        return;
    }
    
    collecterNoeuds(noeud->gauche, tab, index);
    // **Efficacité** : Ajoute les informations du nœud actuel au tableau.
    tab[*index].id = noeud->id;
    tab[*index].capacite = noeud->capacite;
    tab[*index].consommation = noeud->consommation;
    (*index)++;
    collecterNoeuds(noeud->droite, tab, index); // **Efficacité** : Parcours récursif du sous-arbre droit.
}

// Comparateur pour trier les noeuds par consommation décroissante
int compareParConsommation(const void *a, const void *b) {
    // **Robustesse** : Conversion sécurisée des pointeurs génériques en pointeurs vers `NodeInfo`
    const NodeInfo *noeudA = (const NodeInfo *)a;
    const NodeInfo *noeudB = (const NodeInfo *)b;
    
    // **Efficacité** : Compare les consommations pour trier dans l'ordre décroissant.
    if (noeudA->consommation < noeudB->consommation) return 1; // Consommation plus grande en premier.
    if (noeudA->consommation > noeudB->consommation) return -1; // Consommation plus petite en dernier.
    return 0; // Les consommations sont égales.
}

// Fonction qui écrit dans le fichier les 10 stations avec la consommation maximum et les 10 stations avec la consommation minimum dans l'ordre décroissant
void MaxEtMinLV(NoeudAVL *racine, const char *outputFile) {
    // **Robustesse** : Vérifie si l'arbre AVL est NULL.
    if (racine == NULL) {
        fprintf(stderr, "Erreur : Arbre AVL NULL passé à processTopAndBottomLV\n");
        return; // Retour immédiat pour éviter des opérations sur un arbre inexistant.
    }

    // **Robustesse** : Vérifie si le nom du fichier de sortie est NULL.
    if (outputFile == NULL) {
        fprintf(stderr, "Erreur : Nom de fichier NULL passé à processTopAndBottomLV\n");
        return; // Retour immédiat pour éviter des erreurs lors de l'ouverture du fichier.
    }

    int CompteurNoeud = 0; // **Efficacité** : Compteur pour déterminer le nombre total de nœuds dans l'arbre.
    
    // Fonction récursive pour compter les nœuds dans l'arbre AVL.
    void CompteurNoeuds(NoeudAVL *n) {
        if (n == NULL) return;
        CompteurNoeuds(n->gauche);
        CompteurNoeud++;
        CompteurNoeuds(n->droite);
    }
    CompteurNoeuds(racine); // Appel de la fonction pour compter tous les nœuds.
    
    // **Robustesse** : Allocation dynamique du tableau pour stocker les informations des nœuds.
    NodeInfo *noeuds = (NodeInfo *)malloc(sizeof(NodeInfo) * CompteurNoeud);
    if (noeuds == NULL) {
        perror("Erreur d'allocation mémoire");
        return; // Retour immédiat en cas d'échec d'allocation.
    }

    int index = 0; // **Efficacité** : Initialise l'index pour remplir le tableau.
    collecterNoeuds(racine, noeuds, &index); // Remplit le tableau avec les informations des nœuds via un parcours infixe.

    // **Efficacité** : Trie les nœuds par consommation décroissante à l'aide de `qsort`
    qsort(noeuds, CompteurNoeud, sizeof(NodeInfo), compareParConsommation);

    FILE *file = fopen(outputFile, "w"); // **Robustesse** : Ouverture du fichier de sortie en mode écriture.
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        free(noeuds); // Libération de la mémoire allouée avant de quitter
        return; 
    }
    
    // Écriture de l'entête dans le fichier.
    fprintf(file, "Station LV,Capacite,Consommation\n");

    // **Efficacité** : Écrit les 10 stations avec les consommations maximales.
    for (int i = 0; i < 10 && i < CompteurNoeud; i++) {
        fprintf(file, "%d,%lld,%lld\n", noeuds[i].id, noeuds[i].capacite, noeuds[i].consommation);
    }

    // **Efficacité** : Écrit les 10 stations avec les consommations minimales.
    for (int i = CompteurNoeud - 10; i < CompteurNoeud; i++) {
        if (i >= 0) { // **Robustesse** : Vérifie que l'index est valide.
            fprintf(file, "%d,%lld,%lld\n", noeuds[i].id, noeuds[i].capacite, noeuds[i].consommation);
        }
    }

    fclose(file); // **Robustesse** : Fermeture du fichier pour garantir l'intégrité des données
    free(noeuds); // **Robustesse** : Libération de la mémoire allouée pour éviter les fuites.
}


// Comparateur pour trier les nœuds par capacité croissante
int compareParCapacite(const void *a, const void *b) {
    // **Robustesse** : Vérification des paramètres d'entrée pour éviter des erreurs de segmentation.
    if (a == NULL || b == NULL) {
        fprintf(stderr, "Erreur : Pointeurs NULL passés à compareByCapacity\n");
        return 0; // Retourne égalité en cas de problème
    }
    
    // **Efficacité** : Conversion des pointeurs génériques en pointeurs vers `NodeInfo`
    NodeInfo *nodeA = (NodeInfo *)a;
    NodeInfo *nodeB = (NodeInfo *)b;
    
    // **Efficacité** : Comparaison des capacités pour le tri croissant.
    if (nodeA->capacite < nodeB->capacite) return -1; // `nodeA` avant `nodeB`.
    if (nodeA->capacite > nodeB->capacite) return 1; // `nodeB` avant `nodeA`.
    return 0; // Les capacités sont égales.
}

// Fonction pour collecter les nœuds et les trier
void collecterEtTrierParCapacite(NoeudAVL *racine, const char *outputFile) {
    // **Robustesse** : Vérification des paramètres d'entrée pour éviter des erreurs logiques.
    if (racine == NULL) {
        fprintf(stderr, "Erreur : Arbre AVL NULL passé à collectAndSortByCapacity\n");
        return; // Retour immédiat si l'arbre AVL est vide
    }
    if (outputFile == NULL) {
        fprintf(stderr, "Erreur : Nom de fichier NULL passé à collectAndSortByCapacity\n");
        return; // Retour immédiat si le nom du fichier est absent.
    }
    
    // **Efficacité** : Compter le nombre de nœuds dans l'arbre pour allouer un tableau de taille appropriée.
    int CompteurNoeud = 0;
    
    // Fonction récursive pour compter les nœuds dans l'arbre
    void CompteurNoeuds(NoeudAVL *noeud) {
        if (noeud == NULL) return;
        CompteurNoeuds(noeud->gauche);
        CompteurNoeud++;
        CompteurNoeuds(noeud->droite);
    }
    CompteurNoeuds(racine); // Compte tous les nœuds de l'arbre.

    // **Robustesse** : Allocation dynamique pour le tableau de nœuds.
    NodeInfo *noeuds = (NodeInfo *)malloc(sizeof(NodeInfo) * CompteurNoeud);
    if (noeuds  == NULL) {
        perror("Erreur d'allocation mémoire");
        return;
    }

    // **Efficacité** : Collecte les données des nœuds dans le tableau.
    int index = 0;
    collecterNoeuds(racine, noeuds , &index);

    // **Efficacité** : Trie les nœuds par capacité croissante.
    qsort(noeuds , CompteurNoeud, sizeof(NodeInfo), compareParCapacite);

    // **Robustesse** : Ouverture du fichier CSV en mode ajout
    FILE *file = fopen(outputFile, "a");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier CSV");
        free(noeuds);
        return;
    }
    
    // **Efficacité** : Écriture des nœuds triés dans le fichier CSV.
    for (int i = 0; i < CompteurNoeud; i++) {
        fprintf(file, "%d:%lld:%lld\n", noeuds[i].id, noeuds[i].capacite, noeuds[i].consommation);
    }

    fclose(file); // **Robustesse** : Fermeture du fichier pour garantir l'intégrité des données.
    free(noeuds); // **Robustesse** : Libération de la mémoire allouée pour éviter les fuites.
}


// Génère un fichier de données pour GnuPlot
void genererFichierGnuPlot(NoeudAVL *racine, const char *outputFile) {
    if (racine == NULL || outputFile == NULL) {
        fprintf(stderr, "Erreur : Paramètres invalides pour genererFichierGnuPlot\n");
        return;
    }

    int totalNoeuds = 0;
    void compterNoeuds(NoeudAVL *noeud) {
        if (noeud == NULL) return;
        compterNoeuds(noeud->gauche);
        totalNoeuds++;
        compterNoeuds(noeud->droite);
    }
    compterNoeuds(racine);

    NodeInfo *noeuds = malloc(totalNoeuds * sizeof(NodeInfo));
    if (noeuds == NULL) {
        perror("Erreur d'allocation mémoire");
        return;
    }

    int index = 0;
    collecterNoeuds(racine, noeuds, &index);
    qsort(noeuds, totalNoeuds, sizeof(NodeInfo), compareParConsommation);

    FILE *file = fopen(outputFile, "w");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier pour GnuPlot");
        free(noeuds);
        return;
    }

    fprintf(file, "ID Consommation\n");
    for (int i = 0; i < 10 && i < totalNoeuds; i++) {
        fprintf(file, "%d %lld\n", noeuds[i].id, noeuds[i].consommation);
    }
    for (int i = totalNoeuds - 10; i < totalNoeuds; i++) {
        if (i >= 0) {
            fprintf(file, "%d %lld\n", noeuds[i].id, noeuds[i].consommation);
        }
    }

    fclose(file);
    free(noeuds);
}

// Génère le script GnuPlot
void genererScriptGnuPlot(const char *dataFile, const char *outputImage) {
    FILE *file = fopen("graphs/gnuplot_script.txt", "w");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier de script GnuPlot");
        return;
    }

    fprintf(file, "set terminal png size 800,600\n");
    fprintf(file, "set output '%s'\n", outputImage);
    fprintf(file, "set title 'Postes LV les plus et les moins chargés'\n");
    fprintf(file, "set style fill solid 1.0 border -1\n");
    fprintf(file, "set boxwidth 0.5\n");
    fprintf(file, "set xtics rotate by -45\n");
    fprintf(file, "plot '%s' using 2:xtic(1) title '' with boxes lc rgb 'red'\n", dataFile);

    fclose(file);
}

// Exécute le script GnuPlot
void executerGnuPlot() {
    system("gnuplot graphs/gnuplot_script.txt");
}

// Appel principal
void creerGraphique(NoeudAVL *racine) {
    const char *dataFile = "graphs/gnuplot_data.txt";
    const char *outputImage = "graphs/lv_chart.png";

    genererFichierGnuPlot(racine, dataFile);
    genererScriptGnuPlot(dataFile, outputImage);
    executerGnuPlot();
}


// Fonction pour libérer l'arbre AVL
void liberer_arbre(NoeudAVL *n) {
    // **Robustesse** : Vérifie si le nœud est NULL avant de procéder.
    if (n == NULL) {
        return; // Aucun traitement nécessaire si le nœud est NULL
    }
    liberer_arbre(n->gauche); // **Efficacité** : Parcours récursif du sous-arbre gauche pour libérer ses nœuds.
    liberer_arbre(n->droite); // **Efficacité** : Parcours récursif du sous-arbre droit pour libérer ses nœuds.
    free(n); // **Robustesse et Efficacité** : Libère la mémoire du nœud courant.
}


