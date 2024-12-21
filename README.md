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
    Accès à un compilateur C.
    GnuPlot pour les graphiques.
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

**AVERTISSEMENT** _Nous avons déjà mis des tests d'executions dans le dossier ```tests```, vous pouvez les mettre à la corbeille et executer le programme pour les faire apparaître de vous-même si vous le souhaitez, mais ca revient au même car après execution ils sont remplacés_


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

    
    Filtre les données dans le fichier .dat selon les options en deux fichiers dans le dossier de fichier temporaire ```tmp```

    
    Exécute le programme C pour effectuer les calculs.

    
    Génère les fichiers de résultats dans le dossier tests/.

**____________________________________________________________________________________________________________________________**


**Résultats**


Dans le dossier ```tmp```vous verrez les deux fichier contenant les stations et consommateurs filtré par le script shell

Les résultats sont stockés dans des fichiers CSV, dans le dossier test, au format suivant :

    <type_station>_<type_consommateur>.csv (ex. : hva_comp.csv).
    lv_all_minmax.csv : Pour les postes LV avec le plus et le moins de consommation.

**BONUS** le cas bonus à été traité, dans le cas de la commande lv all, aller dans le dossier graphs et vous verrez le graphique ainsi que le script gnuplot

Chaque ligne des fichiers contient :

    Identifiant de la station.
    Capacité en kWh. (trie croissant)
    Consommation en kWh. (trie décroissant dans le cas de lv_all_minmax.csv)


**____________________________________________________________________________________________________________________________**


**Graphique**


Les graphiques en couleurs sont générés uniquement pour le cas lv_all en couleur (bonus)


**____________________________________________________________________________________________________________________________**

**Auteurs**

- ZAAROUR Chady
- BOUIDEGHAGHEN Aline
- BOUABDALLAH-JAQUET Leo

Contactez-nous sur Teams si vous avez un problème, nous sommes tout le temps disponible.



