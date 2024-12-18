#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"


int main() {
    printf("Ouverture des fichiers...\n");
    FILE *file = fopen("tmp/temp_station.txt", "r");
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

    FILE *file2 = fopen("tmp/temp_usager.txt", "r");
    if (file2 == NULL) {
        perror("Erreur lors de l'ouverture du fichier temp_usager.txt");
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }
    fprintf(fichier_csv, "%s:Capacité:Consommation\n",nom_fichier2);
    ecrire_parcours_inf(racine, fichier_csv);
    fclose(fichier_csv);

    printf("Résultats écrits dans %s\n",nom_fichier);
    

    
    if (strcmp(nom_fichier,"lv_all.csv") == 0) {
        printf("Génération des informations LV...\n");
        processTopAndBottomLV(racine, "lv_all_minmax.csv");
        printf("Fichier 'lv_all_minmax.csv' généré.\n");
    }
    
    liberer_arbre(racine);
    return EXIT_SUCCESS;

}
