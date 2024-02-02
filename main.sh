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
  		gnuplot <<-EOF
   		set terminal png size 600,1500
   		set output 'images/Traitement(d1).png'
   		set datafile separator ";"
   		set label "Option -d1: Nb of routes = f(Driver)" at -1.5,95 rotate by 90
   		set ylabel "Number of Routes" offset 64
   		set xlabel "Driver Names" rotate by 180
   		set style data histograms
   		set style fill solid border -1
   		set yrange [0:250]
   		set boxwidth 0.8
   		set xtic rotate 90
   		set ytic rotate 90 offset 61
   		set ytic 50

   		plot 'temp/d1.txt' using 2:xtic(1) notitle
   
		EOF
	
		convert -rotate 90 images/histogram_d1.png images/histogram_d1.png


		end=$(date +%s)
		echo "Temps d'exécution : $((end-start)) secondes"
		return 1;;
	'-d2') 
		start=$(date +%s)

		awk -F';' '
		{ 
			trajet = $6
			distances[trajet] += $5
		} 
		END { 
			for (conducteur in distances) {
				printf "%s;%.3f\n", conducteur, distances[conducteur]
			}
		}' data.csv | sort -t';' -k2,2nr | head -n 10 > temp/d2temp.txt

	
		
		gnuplot <<-EOF
   		set terminal png size 600,1500
   		set output 'images/Traitement(d2).png'
   		set datafile separator ";"
   		set label "Option -d2 : Distance = f(Driver)" at -1.5,95 rotate by 90
   		set ylabel "DISTANCE (Km)" offset 64
   		set xlabel "Driver Names" rotate by 180
   		set style data histograms
   		set style fill solid border -1
   		set yrange [0:160000]
   		set boxwidth 0.8
   		set xtic rotate 90
   		set ytic rotate 90 offset 61
   		set ytic 20000

   		plot 'temp/d2temp.txt' using 2:xtic(1) notitle
   
		EOF
	
		convert -rotate 90 images/histogram_d2.png images/histogram_d2.png
		
		
		
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

	
		
		gnuplot -persist <<- EOF
		set terminal pngcairo enhanced font "arial,12" size 800,600
		set output 'images/Traitement(-l).png'
		set title 'Option -l : Distance = f(Route)'
		set style data histograms
		set style fill solid border -1
		set ylabel 'DISTANCE (Km)'
		set xlabel 'ROUTE ID'
		set yrange [0:3000]
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
	'-s') 
		start=$(date +%s)
		
		# Définir le nom de l'exécutable
		EXEC_NAME="traitement_s"
		
		# Définir le chemin vers le sous-répertoire contenant l'exécutable
		BUILD_DIR="progc"
		cd "$BUILD_DIR"
		cd exe
		
		# Vérifier si l'exécutable existe
		if [ -f "$EXEC_NAME" ]; then
    			echo "L'exécutable existe déjà. Exécution..."
    			./"$EXEC_NAME"
		else
    			echo "L'exécutable n'existe pas. Compilation..."
    			cd ..
		    	# Exécuter la commande make pour compiler le programme
		    	make CY_Truck_s
		    	cd exe
		    	# Vérifier si la compilation a réussi
		    	if [ -f "$EXEC_NAME" ]; then
				echo "Compilation réussie. Exécution..."
				
				./"$EXEC_NAME"
		    	else
				echo "La compilation a échoué."
				exit 1
		    	fi	
		fi
		cd ..
		cd ..
		gnuplot -persist <<- EOF
		
		set terminal png size 1600,1000 enhanced font "arial,12"
		set output 'images/Traitement -S.png'
		
		set title "Distances des trajets (min, moyenne, max) pour les 50 premières valeurs"
		set xlabel "Identifiants des trajets"
		set ylabel "Distances (km)"
		
		set xtics autofreq nomirror rotate by 60 right
		
		set style line 100 lc rgb "black" lw 0.5
		
		set datafile separator ';'
		
		plot 'temp/traitement-s.txt' using (2*\$0+1):2:4 with filledcurves lc rgb "#CEA3FF" fs transparent solid 0.5 title 'Min/Max', \
    		'' using (2*\$0+1):3:xticlabel(1) with lines lt -1 lw 2 title 'Moyenne'
		
		EOF
		end=$(date +%s)	
		echo "Temps d'exécution : $((end-start)) secondes"
		return 1;;
	
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
	
	#fonction traitement 
	traitement "$par"
done
