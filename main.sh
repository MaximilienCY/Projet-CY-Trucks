#!/bin/bash

#######################################################################
#
# Description : Projet démarré le 24 Décembre
#
# Auteur      : Maximilien HAMPE et Timothée FRANCOIS
#
# Version     : 1.0
#
#######################################################################


echo "
Options : 
	- chemin du fichier data.csv : premier paramètre obligatoire 
	- deuxième paramètre obligatoire, voici la liste des paramètres possibles : 
	
		 -h : paramètre d'aide (tout les autres arguments seront ignorés)
	
	 
	 
"



if [ ! -f $1 ];then #si le premier paramètre est bien un fichier
	echo "ERREUR : Le fichier spécifié n'existe pas ou n'en est pas un"
	exit 1
	
elif [ "$1" != *".csv" ];then #si le premier paramètre est un bien un fichier CSV	
	echo "ERREUR : le fichier spécifié doit être un fichier CSV "
	exit 1
	
fi

	


#stockage des données dans une variable data
data=$1
#ignore le premier paremètre
shift

#verification de l'existance d'un deuxième paramètre
if [ "$1" == "" ];then
		echo "ERREUR : Il est obligatoire de passé un deuxième paramètre"
		exit 1
fi




#copie le fichier .csv dans le dossier data. ### VERIFICATION ###
if [ ! -d "data" ];then
	mkdir "data"
	cp $data data
else
	cp $data data	
fi

if [ ! -d "images" ];then #verifiction de la présence du répertoire images
	mkdir "images" 
fi

if [ ! -d "temp" ];then #verifiction de la présence du répertoire temp
	mkdir "temp"
fi



#fonction pour pour identifier l'argument 
verification_parametre() {
	case "$1" in
		'-d1'|'-d2'|'-l'|'-t'|'-s') return 0;;
		
		*) return 1;;
	esac
}

#fonction lancement des traitements
traitement(){
	case "$1" in 
	'-d1')  
		start=$(date +%s)
		#Extraction des prénoms et comptage des occurrences
		cut -d ',' -f $NOM_COLONNE data.csv | sort | uniq -c | sort -rn > counts.txt
		#Affichage des 10 prénoms les plus fréquents
		echo "Les 10 prénoms les plus fréquents :"
		head -n 10 counts.txt

		end=$(date +%s)
		echo "Temps d'exécution : $((end-start)) secondes"
		return 1;;
	'-d2') ;;
	'-l') ;;
	'-t') ;;
	'-s') ;;
	*) return 0;;
	esac
}
#Parcours tout les paramètres 
for par in $*;do
	if [ $par == "-h" ];then
		echo "Vous venez de selectionner le paramètre d'aide"
		exit 1
	fi
	#verification de la validée du parametre par la fonction verification_parametre
	if  ! verification_parametre "$par" ;then
		echo "ERREUR : un des paramètres n'existe pas"
		exit 1
	fi
	#verification executable C
	executable="./m"
	
	#fonction traitement 
	traitement "$par"
done























