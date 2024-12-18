#!/bin/bash

# Fonction d'aide
aide() {
    echo "Usage: $0 [OPTIONS][FICHIER][STATION][CONSOMMATEUR][ID CENTRALE]"
    echo "Options :"
    echo " -h Affiche ce message d'aide et arrête l'exécution"
    echo "Fichier :"
    echo " Fichier .dat existant requis"
    echo "Station :"
    echo -e " Indiquer le type de station a étudier :\n hvb : Station de haut voltage B\n hva : Station de haut voltage A\n lv : Station de faible voltage"
    echo "Consommateur :"
    echo -e " Indiquer le type de consommateur a étudier :\n comp : Entreprises\n indiv : Particuliers\n all : Particuliers et Entreprises\n A noter que les stations de haut voltage ne sont pas connecté aux particuliers."
    echo "Id Centrale :"
    echo -e " Indiquer un identifiant de centrale, l'option est optionnelle et doit être un numéro x compris entre 0<x<n où n est le nombre de centrale."
    
    exit 1

}

# Vérification des arguments pour l'option -h
for arg in "$@"; do
    if [[ "$arg" == "-h" ]]; then
        aide
    fi
done

# Fonction pour compiler cwire si nécessaire
compiler_cwire() {
    # Vérifier si l'exécutable existe
    if [ ! -f "codeC/cwire" ]; then
        echo "Compilation du programme C (cwire) en cours..."
        # Aller dans le répertoire codeC et lancer make
        cd codeC || { echo "Erreur : Répertoire codeC non trouvé."; exit 1; }
        make || { echo "Erreur : La compilation a échoué."; exit 1; }
        cd ..
    else
        echo "L'exécutable 'cwire' est déjà présent."
    fi
}


# Vérification des paramètres d'entré 
verif(){

 # Vérification que les trois options obligatoires sont présentes.
 if [ $# -lt 3 ]; then
  echo "Erreur : Vous devez passer au minimum 3 arguments en paramètre"
  return -1
 fi
	
 # Vérification que le fichier existe et son extension.
 if [[ -f "$arg1" && "$arg1" == *.dat ]]; then
     :
 else
     echo "Erreur : '$arg1' n'est pas un fichier .dat valide."
     return -1
 fi

 # Vérification du second argument concernant les stations a traiter.
 if [[ "$arg2" != "hvb" && "$arg2" != "hva" && "$arg2" != "lv" ]]; then
  echo "Erreur : les stations a traiter sont : hvb ; hva ; lv "
  return -1
 fi


 # Vérification du troisième argument concernant les usagers a traiter.
 if [[ "$arg3" != "comp" && "$arg3" != "indiv" && "$arg3" != "all" ]]; then
  echo "Erreur : les consommateurs a traiter sont : comp ; indiv ; all "
  return -1
 fi
	
 ligne_fin=$(tac "$arg1" | grep -v '^[[:space:]]*$' | head -n 1 | awk -F';' '{print $1}')
 # Vérification du quatrième argument 
 if [[ -n "$arg4" && ! "$arg4" =~ ^[0-9]+$ || -n "$arg4" && "$ligne_fin" -lt "$arg4" ]];then
  echo "Erreur : Mauvais identifiant de central"
  return -1
 fi


 arg_fus="$arg2 $arg3"

 # Vérification de la compatibilité des arguments 2 et 3.
 case "$arg_fus" in 
  "hvb all")
  echo "Erreur : les stations HV-B ne sont connecté qu'a des entreprises"
  return -1
  ;;
  "hvb indiv")
  echo "Erreur : les stations HV-B ne sont connecté qu'a des entreprises"
  return -1return -1
  ;;
  "hva all")
  echo "Erreur : les stations HV-A ne sont connecté qu'a des entreprises"
  return -1
  ;;
  "hva indiv")
  echo "Erreur : les stations HV-A ne sont connecté qu'a des entreprises"
  return -1
  ;;
 esac
}

# Création des dossiers utile au programme
crea_doss(){
 doss="tmp"
 if [ ! -d "$doss" ] ; then
  mkdir "$doss"
 else
  rm -rf $doss/{*,.*} 2>/dev/null
 fi
 doss="graphs"
 if [ ! -d "$doss" ] ; then
  mkdir "$doss"
 fi
}
# Récupération des donnés sur la centrale et ses usagers.
recup_donne(){
	
 case "$arg_fus" in
  "hvb comp")
  if [ -z "$arg4" ];then
   station=$(grep -P "^(\d+);(\d+);-;-;-;-;(\d+);-" $arg1)
   usagers=$(grep -P "^(\d+);(\d+);-;-;(\d+);-;-;(\d+)" $arg1)
  
  else
   station=$(grep -P "^($arg4);(\d+);-;-;-;-;(\d+);-" $arg1)
   usagers=$(grep -P "^($arg4);(\d+);-;-;(\d+);-;-;(\d+)" $arg1)
  fi
  ;;
  "hva comp")
  if [ -z "$arg4" ];then
   station=$(grep -P "^(\d+);(\d+);(\d+);-;-;-;(\d+);-" $arg1)
   usagers=$(grep -P "^(\d+);-;(\d+);-;(\d+);-;-;(\d+)" $arg1)
  else
   station=$(grep -P "^($arg4);(\d+);(\d+);-;-;-;(\d+);-" $arg1)
   usagers=$(grep -P "^($arg4);-;(\d+);-;(\d+);-;-;(\d+)" $arg1)
  
  fi
  ;;
  "lv comp")
  if [ -z "$arg4" ];then
   station=$(grep -P "^(\d+);-;(\d+);(\d+);-;-;(\d+);-" $arg1)
   usagers=$(grep -P "^(\d+);-;-;(\d+);(\d+);-;-;(\d+)" $arg1)
  else
   station=$(grep -P "^($arg4);-;(\d+);(\d+);-;-;(\d+);-" $arg1)
   usagers=$(grep -P "^($arg4);-;-;(\d+);(\d+);-;-;(\d+)" $arg1)
  
  fi
  ;;
  "lv indiv")
  if [ -z "$arg4" ];then
   station=$(grep -P "^(\d+);-;(\d+);(\d+);-;-;(\d+);-" $arg1)
   usagers=$(grep -P "^(\d+);-;-;(\d+);-;(\d+);-;(\d+)" $arg1)
  else 
   station=$(grep -P "^($arg4);-;(\d+);(\d+);-;-;(\d+);-" $arg1)
   usagers=$(grep -P "^($arg4);-;-;(\d+);-;(\d+);-;(\d+)" $arg1)
  
  fi
  ;;
  "lv all")
  if [ -z "$arg4" ];then
   station=$(grep -P "^(\d+);-;(\d+);(\d+);-;-;(\d+);-" $arg1)
   usagers_1=$(grep -P "^(\d+);-;-;(\d+);(\d+);-;-;(\d+)" $arg1)
   usagers_2=$(grep -P "^(\d+);-;-;(\d+);-;(\d+);-;(\d+)" $arg1)
  else
   station=$(grep -P "^($arg4);-;(\d+);(\d+);-;-;(\d+);-" $arg1)
   usagers_1=$(grep -P "^($arg4);-;-;(\d+);(\d+);-;-;(\d+)" $arg1)
   usagers_2=$(grep -P "^($arg4);-;-;(\d+);-;(\d+);-;(\d+)" $arg1)
  fi
  usagers="$usagers_1\n$usagers_2"
  ;;
 esac
}

# Fonction pour exécuter cwire
executer_cwire() {
    # Lancer l'exécution de cwire
    ./codeC/cwire
}


# Calcul le temps d'exécution d'une fonction passé en paramètre
timer(){
 local Nom="$1"
 local Fonction="$2"
	
 echo "Traitement de $Nom"
	
 start_timer=$(date +%s)
	
 $Fonction "$arg1" "$arg2" "$arg3" "$arg4"
 local statut=$?
	
 end_timer=$(date +%s)
	
 if [ $statut -eq 0 ]; then
  duree=$(echo "$end_timer - $start_timer " | bc)
  echo "Durée de la fonction $Nom : $duree secondes"
 else
  duree=0
  echo "Erreur dans le traitement de la fonction $Fonction"
  echo "Durée de la fonction $Nom : $duree secondes"
  aide
  exit 1
  
 fi
	
  
}


ecriture(){
    # Créer les fichiers temporaires pour les stations et les usagers
    touch tmp/temp_station.txt tmp/temp_usager.txt

    # Écrire les stations dans temp_station.txt
    echo -e "$station" > tmp/temp_station.txt

    # Écrire les usagers dans temp_usager.txt
    echo -e "$usagers" > tmp/temp_usager.txt

}

arg1="$1"
arg2="$2"
arg3="$3"
arg4="$4"

timer "Vérification" "verif"

# Compilation de cwire si nécessaire
compiler_cwire

timer "Création des dossiers" "crea_doss"
timer "Récuperation des données" "recup_donne"
timer "Écriture des données dans le fichier temporaire" "ecriture"
# Exécution du programme cwire
executer_cwire

