#!/bin/bash

#######################################################################
#
# Description : 
#
# Auteur      : Maximilien HAMPE et Timothée FRANCOIS
#
# Version     : 1.0
#
#######################################################################


echo "
Options : 
	- chemin du fichier data.csv : premier paramètre obligatoire 
	- -h : paramètre d'aide (tout les autres arguments seront ignorés)
	 
	 
	 
"


#si le premier paramètre est bien un fichier
if [ ! -f $1 ];then
	echo "Le fichier spécifié n'existe pas ou n'en est pas un"

#si le premier paramètre est un bien un fichier CSV	
elif [ "$1" != "*.csv" ];then
	echo "ERREUR : le fichier spécifié doit être un fichier CSV "
	
fi

#stockage des données dans une variable data
data=$1
#ignore le premier paremètre
shift

#fonction pour pour identifier l'argument 
recherche() {
	case $1 in
		'-h') return 0;
		'-d1') return 1;
		'-d2') return 2;
		'-l') return 3;
		'-t') return 4;
		'-s') return 5;

}

#Regarde tout les paramètres restant
for par in $*;do
	echo $par
done



