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


#si le premier paramètre est bien un fichier
if [ ! -f $1 ];then
	echo "ERREUR : Le fichier spécifié n'existe pas ou n'en est pas un"
	exit 1
fi
	
#si le premier paramètre est un bien un fichier CSV	
if [ "$1" != *".csv" ];then
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


#verification executable C

#verification de la présence du dossier temp et images 




#fonction pour pour identifier l'argument 
verification_parametre() {
	case "$1" in
		'-d1'|'-d2'|'-l'|'-t'|'-s') return 0;;
		
		*) return 1;;
	esac
}

#fonction lancement des traitements

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
	
done
