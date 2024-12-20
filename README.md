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

**2 - Donner les droits d’exécution au script**
chmod +x c-wire.sh

**3 - Placer le fichier de données**
Assurez-vous que le fichier c-wire_v25.dat est dans le dossier input/.
