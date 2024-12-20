**C-Wire Project**
**Description**

Ce projet analyse les données de distribution d’électricité en France, en identifiant les cas de surproduction ou de sous-production d’énergie des différentes stations (HV-B, HV-A, LV). Il fournit également des statistiques sur la consommation énergétique des entreprises et des particuliers.

Le programme traite un fichier CSV volumineux (**c-wire_v25.dat**) à l’aide d’un script **Shell** pour le filtrage et d’un **programme en C** pour le calcul des consommations à l’aide d’un arbre AVL.
**____________________________________________________________________________________________________________________________**

**Structure du projet**

    c-wire.sh : Script Shell principal pour le filtrage et l’exécution.
    codeC/ : Contient le programme en C, le Makefile pour la compilation.
    input/ : Dossier contenant le fichier de données c-wire_v25.dat.
    tmp/ : Dossier temporaire pour les fichiers intermédiaires.
    graphs/ : Dossier pour stocker les graphiques générés.
    tests/ : Contient les fichiers résultats des traitements.
**____________________________________________________________________________________________________________________________**

**Prérequis**

    Un système Unix/Linux avec Bash installé.
    Accès à un compilateur C (ex. : gcc).
    GnuPlot pour les graphiques (facultatif).
**____________________________________________________________________________________________________________________________**

**Étapes d'installation et d'exécution**

**1 - Cloner le dépôt Git**

```git clone https://github.com/epice95/C-Wire_projet.git```


```cd C-Wire_projet```


```ls``` (pour vérifier que tout c'est bien passé)


**2 - Donner les droits d’exécution au script**

```chmod +x c-wire.sh```

**3 - Placer le fichier de données**

Assurez-vous que le fichier c-wire_v25.dat est dans le dossier input/.

**____________________________________________________________________________________________________________________________**

**Exécution du script**

Le script s'exécute avec les paramètres suivants :

``` ./c-wire.sh input/c-wire_v25 <type_station> <type_consommateur> [<id_centrale>] [-h]```  (les crochets veulent dire que c'est facultatif)

**Paramètres**

chemin_du_fichier : Chemin vers le fichier c-wire_v25.dat (obligatoire).


type_station : Type de station à analyser (obligatoire) :

    hvb : Stations HV-B

    
    hva : Stations HV-A

    
    lv : Postes LV

    

type_consommateur : Type de consommateur à analyser (obligatoire) :

    comp : Entreprises

    
    indiv : Particuliers

    
    all : Tous les consommateurs

    

id_centrale : ID de la centrale à filtrer (optionnel).


-h : Affiche l’aide.

**Exemples**

**1. Pour analyser les stations HV-A et les entreprises :**


```./c-wire.sh input/c-wire_v25.dat hva comp```


**2. Pour analyser les postes LV et tous les consommateurs  :**


```./c-wire.sh input/c-wire_v25.dat lv all```


**3. Pour afficher l’aide :**


```./c-wire.sh -h```


**____________________________________________________________________________________________________________________________**


**Détails du script**

    Vérifie les options fournies.

    
    Compile le programme C avec make si nécessaire.

    
    Nettoie ou crée les dossiers tmp/ et graphs/.

    
    Filtre les données dans le fichier .dat selon les options.

    
    Exécute le programme C pour effectuer les calculs.

    
    Génère les fichiers de résultats dans le dossier tests/.

**____________________________________________________________________________________________________________________________**


**Résultats**

Les résultats sont stockés dans des fichiers CSV au format suivant :

    <type_station>_<type_consommateur>.csv (ex. : hva_comp.csv).
    lv_all_minmax.csv : Pour les postes LV avec le plus et le moins de consommation.

**BONUS** le cas bonus à été traité, dans le cas de la commande lv all, aller dans le dossier graphs et vous verrez le graphique ainsi que le script gnuplot

Chaque ligne des fichiers contient :

    Identifiant de la station.
    Capacité en kWh.
    Consommation en kWh.


**____________________________________________________________________________________________________________________________**


**Limitations et améliorations possibles**


Les performances peuvent être limitées par la taille des données.


Les graphiques sont générés uniquement pour le cas lv_all.


**____________________________________________________________________________________________________________________________**

**Auteurs**

- ZAAROUR Chady
- BOUIDEGHAGHEN Aline
- BOUABDALLAH-JAQUET Leo

Contactez-nous sur Teams si vous avez un problème, nous sommes tout le temps disponible.



