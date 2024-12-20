**README - Projet C-Wire**

Description

Ce projet analyse les données d'un système de distribution d'électricité en France pour déterminer les situations de surproduction ou sous-production d'énergie dans les différentes stations (HV-B, HV-A, LV) et les proportions de consommation par les entreprises et les particuliers. Il inclut un script Shell (« c-wire.sh ») pour le filtrage des données et un programme C pour les calculs avec un arbre AVL.

Structure des fichiers

Voici l'organisation des fichiers dans le projet :
/ (racine du projet)
|-- c-wire.sh             (script Shell principal)
|-- input/                (dossier contenant le fichier de données d'entrée)
|   |-- c-wire_v25.dat    (fichier de données CSV)
|-- codeC/                (dossier contenant le programme C et le Makefile)
|   |-- main.c            (code source du programme en C)
|   |-- Makefile          (pour la compilation du programme)
|-- tmp/                  (dossier pour les fichiers intermédiaires)
|-- tests/                (dossier pour les résultats précédents)
|-- graphs/               (dossier pour les graphiques générés)
