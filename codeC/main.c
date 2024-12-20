#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"


int main() {
    printf("Ouverture des fichiers...\n"); // **Efficacité** : Affichage pour indiquer l'état d'avancement, permettant de suivre l'exécution du programme.
    FILE *file = fopen("tmp/temp_station.txt", "r"); // **Robustesse** : Ouverture du fichier contenant les informations de station en mode lecture. Vérifie que le fichier existe et est accessible.
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier temp_station.txt");
        // **Robustesse** : Gestion des erreurs d'ouverture de fichier pour éviter un comportement indéfini si le fichier est manquant.
        return EXIT_FAILURE;
    }
    
    // **Robustesse** : Initialisation explicite de la racine à NULL pour garantir un état contrôlé en cas d'erreur.
    NoeudAVL *racine = NULL;
    NoeudAVL *temp = lire_fichier_station(file); // **Efficacité** : Lecture d'un nœud de station depuis le fichier. Cette fonction permet un traitement ligne par ligne.
    while (temp != NULL) {
        racine = inserer(racine, temp); // **Efficacité** : Insertion du nœud dans l'arbre AVL pour maintenir un ordre structuré et permettre des recherches rapides.
        temp = lire_fichier_station(file); // **Efficacité** : Lecture de la ligne suivante pour continuer à remplir l'arbre AVL.
    }
    
    // **Robustesse** : Fermeture du fichier pour libérer les ressources système associées.
    fclose(file);
    
    if (racine == NULL) {
        fprintf(stderr, "Erreur : Aucun nœud inséré dans l'arbre AVL à partir de temp_station.txt\n");
        // **Robustesse** : Vérification si l'arbre est vide, ce qui signale un problème avec le fichier ou son traitement.
        return EXIT_FAILURE;
    }

    FILE *file2 = fopen("tmp/temp_usager.txt", "r"); // **Robustesse** : Ouverture du fichier contenant les informations des usagers en mode lecture.
    if (file2 == NULL) {
        perror("Erreur lors de l'ouverture du fichier temp_usager.txt"); // **Robustesse** : Gestion des erreurs d'ouverture de fichier pour éviter des opérations inutiles sur un fichier absent.
        return EXIT_FAILURE; 
    }
    
    lire_fichier_consommateur(file2, racine); // **Efficacité** : Lecture des consommations et mise à jour de l'arbre AVL pour intégrer ces informations.
    fclose(file2); // **Robustesse** : Fermeture du fichier pour libérer les ressources associées.
    
    
    // **Efficacité** : Préparation des noms de fichiers pour structurer les résultats
    char nom_fichier[256]; 
    char nom_fichier2[256];
    choisirNomFichier(nom_fichier,nom_fichier2);

    printf("Écriture des résultats dans %s\n",nom_fichier); // **Efficacité** : Indique l'emplacement du fichier de sortie pour le suivi de l'exécution.
    FILE *fichier_csv = fopen(nom_fichier, "w"); // **Robustesse** : Création du fichier CSV en mode écriture.
    if (fichier_csv == NULL) {
        perror("Erreur lors de la création du fichier resultat.csv"); 
        // **Robustesse** : Gestion des erreurs de création de fichier pour éviter un crash en cas d'échec.
        return EXIT_FAILURE; 
    }
    
    fprintf(fichier_csv, "%s:Capacité:Consommation\n",nom_fichier2); // **Efficacité** : Écriture de l'entête du fichier CSV pour structurer les données exportées.
    fclose(fichier_csv); // **Robustesse** : Fermeture du fichier pour garantir l'intégrité des données et libérer les ressources.
    collecterEtTrierParCapacite(racine, nom_fichier); // **Efficacité** : Collecte des nœuds AVL, tri par capacité croissante et écriture dans le fichier CSV.
    

    printf("Résultats écrits dans %s\n",nom_fichier); // **Efficacité** : Retour visuel confirmant le succès de l'exportation des résultats.
    

    // CAS SPECIAL : traitement de la commande lv all
    if (strcmp(nom_fichier,"lv_all.csv") == 0) { 
        // **Efficacité** : Indique l'exécution d'une tâche spécifique pour les fichiers `lv_all.csv`
        printf("Génération des informations LV...\n");
        // **Efficacité** : Génère un fichier distinct contenant les 10 stations les plus et les moins chargées
        MaxEtMinLV(racine, "lv_all_minmax.csv");
        // **Efficacité** : Confirmation visuelle que le fichier spécial a été créé avec succès.
        printf("Fichier 'lv_all_minmax.csv' généré.\n"); 
    }
    
    liberer_arbre(racine); // **Robustesse** : Programme assuré par une libération complète de tout les noeuds de l'AVL racine
    return EXIT_SUCCESS; 

}
