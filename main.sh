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
		awk -F';' '
		{ 
			trajet = $1 ";" $6
			if (!existence[trajet]++) {
				conducteurs[$6]++
			}	
		} 
		END { 
			for (conducteur in conducteurs) {
				print conducteur";"conducteurs[conducteur]
			}
		}'  data.csv | sort -t';' -k2,2nr | head -n 10 > temp/d1.txt
  		gnuplot -persist <<- EOF 
		set terminal png size 800,600 enhanced font "arial,10"
set output 'horizontal_histogram.png'

set style fill solid 1.0
set boxwidth 0.5

# Définir les étiquettes de l'axe des y pour être les noms
set ytics nomirror rotate by -90
set format y ""

set xlabel "Valeurs"
set ylabel "Noms"

# Pour que les étiquettes de l'axe des Y apparaissent à gauche de chaque barre
set datafile separator ";"
set yrange [-1:*] reverse
unset key

	plot 'temp/d2.txt' using 2:xtic(1) with boxes lc rgb "blue" notitle



		EOF
  
		end=$(date +%s)
		echo "Temps d'exécution : $((end-start)) secondes"
		return 1;;
	'-d2') 
		start=$(date +%s)

		LC_NUMERIC=C awk -F';' 'BEGIN { OFS=";" } { gsub(",", "", $5); distances[$6] += $5 
		} 
		END { 
    		for (conducteur in distances) printf "%s %.3f\n", conducteur, distances[conducteur] 
		}' data.csv > temp/d2temp.csv

		LC_NUMERIC=C sort -t ' ' -k3,3nr temp/d2temp.csv | head -n 10 > d2.csv
		end=$(date +%s)
		echo "Temps d'exécution : $((end-start)) secondes"
		return 1;;
		
	'-l')
		start=$(date +%s)
		LC_NUMERIC=C awk -F';' 'BEGIN { OFS=";" } { gsub(",", "", $5); distances[$1] += $5 
		} 
		END { 
    			for (conducteur in distances) printf "%s %.3f\n", conducteur, distances[conducteur] 
		}' data.csv > temp/ltemp.csv 
		LC_NUMERIC=C sort -t ' ' -k2,2nr temp/ltemp.csv | head -n 10 | sort -n -t ' ' -k1 > l.csv

		output_folder="images"  # Spécifiez le dossier de sortie
		output_file="${output_folder}/Traitement -l.png"
		
		gnuplot -persist <<- EOF
		set terminal pngcairo enhanced font "arial,12" size 800,600
		set output '$output_file'
		set title 'Les 10 trajets les plus longs'
		set style data histograms
		set style fill solid border -1
		set ylabel 'Distance (km)'
		set xlabel 'Identifiant du trajet'
		set xtics rotate by -45
		set datafile separator " "
		plot 'l.csv' using 2:xtic(1) title ''
		EOF

		end=$(date +%s)	
		echo "Temps d'exécution : $((end-start)) secondes"
		return 1;;
	'-t') 
		start=$(date +%s)
		
		

		
		gnuplot -persist <<- EOF
		set terminal png size 800,500 enhanced font "arial,12"
		set output 'images/Traitement -T.png'
		
		red = "#FF0000"; blue = "#0000FF"; 
 		set yrange [0:4000]
		set style data histogram
 		set style histogram cluster gap 1
 		set style fill solid
 		set boxwidth 0.9
		set xtics format ""
		set bmargin 10
		set ylabel "NB ROUTES"
		set xlabel "TOWN NAMES"
		set xtics rotate by -90
		set grid ytics
		set datafile separator ";"

		set title "Option -t Nb routes = f(Towns)"
		plot "data_t.dat" using 2:xtic(1) title "Total routes" linecolor rgb red,   \
     		"data_t.dat" using 3 title "First town" linecolor rgb blue
		EOF
		
		
		
		end=$(date +%s)	
		echo "Temps d'exécution : $((end-start)) secondes"
		return 1;;
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



