#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"


int main() {
    printf("Ouverture des fichiers...\n"); // on ouvre les deux fichiers filtrés générés par le shell
    FILE *file = fopen("tmp/temp_station.txt", "r"); // on effectue la lecture du fichier avec la commande "r"
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier temp_station.txt");
        return EXIT_FAILURE;
    }
    NoeudAVL *racine = NULL;
    NoeudAVL *temp = lire_fichier_station(file);
    while (temp != NULL) {
        racine = inserer(racine, temp);
        temp = lire_fichier_station(file);
    }
    fclose(file);

    FILE *file2 = fopen("tmp/temp_usager.txt", "r"); // On effectue la lecture du fichier avec la commande "r"
    if (file2 == NULL) {
        perror("Erreur lors de l'ouverture du fichier temp_usager.txt");
        return EXIT_FAILURE; // Robustesse du programme assuré par cette commande
    }
    lire_fichier_consommateur(file2, racine);
    fclose(file2);
    
    
    // Choisir le nom du fichier pour l'exportation
    char nom_fichier[256];
    char nom_fichier2[256];
    choisirNomFichier(nom_fichier,nom_fichier2);

    printf("Écriture des résultats dans %s\n",nom_fichier);
    FILE *fichier_csv = fopen(nom_fichier, "w");
    if (fichier_csv == NULL) {
        perror("Erreur lors de la création du fichier resultat.csv");
        return EXIT_FAILURE; // Robustesse du programme assuré par cette commande
    }
    fprintf(fichier_csv, "%s:Capacité:Consommation\n",nom_fichier2); // On affiche l'entête du fichier de sortie csv
    fclose(fichier_csv); // Robustesse assuré par la fermeture du fichier_csv
    collectAndSortByCapacity(racine, nom_fichier); // on collecte et trie par capacités croissantes l'Arbre AVL de stations
    

    printf("Résultats écrits dans %s\n",nom_fichier); // On affiche dans le terminal le succès de la génération du fichier csv 
    

    // CAS SPECIAL : traitement de la commande lv all
    if (strcmp(nom_fichier,"lv_all.csv") == 0) {
        printf("Génération des informations LV...\n");
        processTopAndBottomLV(racine, "lv_all_minmax.csv");
        printf("Fichier 'lv_all_minmax.csv' généré.\n"); // On affiche dans le terminal le succès de la génération du fichier "lv_all_minmax.csv"
    }
    
    liberer_arbre(racine); // Robustesse du programme assuré par une libération complète de tout les noeuds de l'AVL racine
    return EXIT_SUCCESS; 

}
