#!/bin/bash


#################################################################
#								#
#    	  ______   __  _____ ____  _   _  ____ _  __		#
#   	 / ___\ \ / / |_   _|  _ \| | | |/ ___| |/ /		#
#  	| |    \ V /    | | | |_) | | | | |   | ' / 		#
#	| |___  | |     | | |  _ <| |_| | |___| . \ 		#
#  	\____| |_|     |_| |_| \_\\___/ \____|_|\_\		#
#                                            			#
#								#
#################################################################




##################################################################
#								 #
# 	Description : Projet démarré le 24 Décembre		 #		
#								 #
# 	Auteur      : Maximilien HAMPE et Timothée FRANCOIS      #
#								 #
# 	Version     : 1.0					 #
#								 #
##################################################################



echo "			   ___        _   _                     "
echo "			  / _ \ _ __ | |_(_) ___  _ __  ___     "
echo "			 | | | | '_ \| __| |/ _ \| '_ \/ __|    "
echo "			 | |_| | |_) | |_| | (_) | | | \__ \    "
echo "			  \___/| .__/ \__|_|\___/|_| |_|___/    "
echo "			       |_|                              "

echo "	-> chemin du fichier data.csv : premier paramètre obligatoire 
	-> Traitement voulu , voici la liste des paramètres possibles : 
		
		-d1 : Affiche un graphique des 10 conducteurs avec le plus de trajets
		-d2 : Affiche un graphique des 10 conducteurs ayant parcourus la plus longue distance 
		-l  : Affiche un graphique des 10 trajets les plus longs
		-t  : Affiche un graphique des 10 villes les plus traversées
		-s  : Affiche un graphique de statistiques sur les étapes
		 -h : paramètre d'aide (tout les autres arguments seront ignorés) 
"

#########################################################################

if [ ! -f $1 ];then # Si le premier paramètre est bien un fichier
	echo "ERREUR : Le fichier spécifié n'existe pas ou n'en est pas un"
	exit 1
	
elif [ "$1" != *".csv" ];then  # Si le premier paramètre est un bien un fichier CSV	
	echo "ERREUR : le fichier spécifié doit être un fichier CSV "
	exit 1
	
fi

	


# Stockage des données dans une variable data
data=$1
# Ignore le premier paremètre
shift

# Vérification de l'existence d'un deuxième paramètre
if [ "$1" == "" ];then
		echo "ERREUR : Il est obligatoire de passé un deuxième paramètre"
		exit 1
fi




# Copie le fichier .csv dans le dossier data. ### VERIFICATION ###

if [ ! -d "data" ];then
	mkdir "data"
	cp $data data
else
	cp $data data	
fi

if [ ! -d "images" ];then # Vérifiction de la présence du répertoire images
	mkdir "images" 
fi

if [ ! -d "temp" ];then # Vérifiction de la présence du répertoire temp
	mkdir "temp"
fi



# Fonction pour pour identifier l'argument 

verification_parametre() {
	case "$1" in
		'-d1'|'-d2'|'-l'|'-t'|'-s') return 0;;
		
		*) return 1;;
	esac
}

####################################### [Fonction de lancement des traitements] ###################################

traitement(){

	case "$1" in 
	
	#Traitement -d1
	
	'-d1')  
		
		echo -e "Vous venez de sélectionner le traitement -d1, patientez..."
		
		#Démarrage du compteur de temps
		
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
		}'  data.csv | sort -t';' -k2,2nr | head -n 10 > temp/d1temp.txt

################################# [Création du graphique avec GNUPLOT] #####################################		
		
  		# Ce script génère un histogramme en utilisant Gnuplot et fait pivoter l'image résultante.
# Il configure le terminal en PNG, spécifie le fichier de sortie et ajuste divers paramètres de tracé.


		gnuplot <<-EOF
		
		# Défini "blue" comme une couleur en hexadécimal
		blue = "#6984a3";
		
    		# Défini le terminal en PNG avec une taille spécifiée
    		set terminal png size 600,1500

    		# Défini le fichier de sortie
    		set output 'images/traitement_d1.png'

    		# Spécifie le séparateur de fichier de données comme point-virgule
    		set datafile separator ";"

    		# Ajoute les différents titres 
    		set label "Option -d1: Nb de routes = f(Conducteur)" at -1.5,95 rotate by 90
    		set ylabel "Nombre de Routes" offset 64
    		set xlabel "Noms des Conducteurs" rotate by 180

   	 	# Défini le style de données comme des histogrammes
    		set style data histogram
    		set style fill solid border -1

    		# Défini la plage de l'axe des y et configure la largeur de la barre
    		set yrange [0:250]
    		set boxwidth 1.2

    		# Fait pivoter les repères de l'axe des x et configure les repères de l'axe des y
    		set xtic rotate 90
   	 	set ytic rotate 90 offset 61
    		set ytic 50
    		
    		# Augmente la marge en bas de l'axe des ordonées
    		set bmargin 15
    		

    		# Tracer les données à partir du fichier spécifié en utilisant la deuxième colonne pour les valeurs 		de l'axe des y en spécifiant la couleur des barres 
    		plot 'temp/d1temp.txt' using 2:xtic(1) notitle linecolor rgb blue 
		EOF

		# Faire pivoter l'image générée au format paysage en utilisant la commande convert d'ImageMagick
		convert -rotate 90 images/traitement_d1.png images/traitement_d1.png
		
		
#############################################################################################################

		# Affiche le graphique dans une fenêtre
		xdg-open "images/traitement_d1.png"

		# Arrêt du compteur de temps

		end=$(date +%s)
		echo " Traitement reussi !"
		echo "Temps d'exécution : $((end-start)) secondes"
		
		return 1;;
		
	#Traitement -d2	
		
	'-d2') 
	
		echo -e "Vous venez de sélectionner le traitement -d2, patientez..."
	
		#Démarrage du compteur de temps
		
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

################################# [Création du graphique avec GNUPLOT] #####################################	
		
		gnuplot <<-EOF
		
		# Défini "blue" comme une couleur en hexadécimal
		blue = "#6984a3";
		
		# Défini le terminal en PNG avec une taille spécifiée
   		set terminal pngcairo enhanced font "arial,12" size 600,1500
   		
   		# Défini le fichier de sortie
   		set output 'images/traitement_d2.png'
   		
   		# Spécifie le séparateur de fichier de données comme point-virgule
   		set datafile separator ";"
   		
   		# Ajoute les différents titres 
   		set label "Option -d2 : Distance = f(Driver)" at -1.5,60000 rotate by 90
   		set ylabel "DISTANCE (Km)" offset 64
   		set xlabel "Driver Names" rotate by 180
   		
   		# Défini le style de données comme des histogrammes
   		set style data histograms
   		set style fill solid border -1
   		
   		# Défini la plage de l'axe des y et configure la largeur de la barre
   		set yrange [0:160000]
   		set boxwidth 1.2
   		
   		# Fait pivoter les repères de l'axe des x et configure les repères de l'axe des y
   		set xtic rotate 90
   		set ytic rotate 90 offset 61
   		set ytic 20000
   		
    	
		# Tracer les données à partir du fichier spécifié en utilisant la deuxième colonne pour les valeurs 		de l'axe des y en spécifiant la couleur des barres 
   		plot 'temp/d2temp.txt' using 2:xtic(1) notitle linecolor rgb blue
   
		EOF
		
		#Permet de faire une rotation de l'image pour qu'elle soit au format paysage
	
		convert -rotate 90 images/traitement_d2.png images/traitement_d2.png
		
		
#############################################################################################################		
		
		# Affiche le graphique dans une fenêtre
		xdg-open "images/traitement_d2.png"
		
		# Arrêt du compteur de temps
		
		end=$(date +%s)
		echo "Temps d'exécution : $((end-start)) secondes"
		
		return 1;;
		
	#Traitement -l
		
	'-l')
		
		echo -e "Vous venez de sélectionner le traitement -l, patientez..."
	
		#Démarrage du compteur de temps
		
		start=$(date +%s)
		LC_NUMERIC=C awk -F';' 'BEGIN { OFS=";" } { gsub(",", "", $5); distances[$1] += $5 
		} 
		END { 
    			for (conducteur in distances) printf "%s %.3f\n", conducteur, distances[conducteur] 
		}' data.csv > temp/l_temp.csv
		LC_NUMERIC=C sort -t ' ' -k2,2nr temp/l_temp.csv | head -n 10 | sort -n -t ' ' -k1 > temp/ltemp.txt

################################# [Création du graphique avec GNUPLOT] #####################################	
		
		gnuplot -persist <<- EOF
		
		# Défini le terminal en PNG avec une police et une taille spécifiée
		set terminal pngcairo enhanced font "arial,12" size 800,600
		
		# Défini le fichier de sortie
		set output 'images/traitement_l.png'
		
		# Défini "blue" comme une couleur en hexadécimal
		blue = "#6984a3";
		
		# Ajoute les différents titres 
		set title 'Option -l : Distance = f(Route)'
		set ylabel 'DISTANCE (Km)'
		set xlabel 'ROUTE ID'
		
		# Défini le style de données comme des histogrammes
		set style data histograms
		set style fill solid border -1
		
		# Défini la plage de l'axe des y et fait pivoter les etiquettes de l'axe x
		set yrange [0:3000]
		set xtics rotate by -45
		
		# Spécifie le séparateur de fichier de données comme un espace
		set datafile separator " "
		
		plot 'temp/ltemp.txt' using 2:xtic(1) notitle linecolor rgb blue
		
		EOF
		

#############################################################################################################
		
		# Affiche le graphique dans une fenêtre
		xdg-open "images/traitement_l.png"
		
		# Arrêt du compteur de temps

		end=$(date +%s)	
		echo "Temps d'exécution : $((end-start)) secondes"
		
		return 1;;
		
	#Traitement -t
		
	'-t') 
	
		echo -e "Vous venez de sélectionner le traitement -t, patientez..."
		
		#Démarrage du compteur de temps
		
		start=$(date +%s)
		
		
################################# [Création du graphique avec GNUPLOT] #####################################
		
		gnuplot -persist <<- EOF
		
		# Défini le terminal en PNG avec une police et une taille spécifiée
		set terminal png size 800,500 enhanced font "arial,12"
		
		# Défini le fichier de sortie
		set output 'images/traitement_t.png'
		
		# Défini "blue" et "bluedark" comme une couleur en hexadécimal
		blue = "#6984a3"; bluedark = "#2c3f66"; 
 		
 		# Ajoute les différents titres
 		set title "Option -t Nb routes = f(Towns)"
 		set ylabel "NB ROUTES"
		set xlabel "TOWN NAMES"
 		
 		# Défini le style de données comme des histogrammes
 		set style data histogram
 		set style histogram cluster gap 1
 		set style fill solid
 		
 		# Défini la plage de l'axe des y et fait pivoter les etiquettes de l'axe x
 		set yrange [0:4000]
 		set xtics rotate by -90
		set xtics format ""
		
		# Défini la largeur des barres 
 		set boxwidth 0.9
		set bmargin 10
		set grid ytics
		
		# Spécifie le séparateur de fichier de données comme un point virgule
		set datafile separator ";"

		plot "temp/data_t.dat" using 2:xtic(1) title "Total routes" linecolor rgb bluedark,   \
     		"temp/data_t.dat" using 3 title "First town" linecolor rgb blue
		EOF
		
		
#############################################################################################################		
		
		# Affiche le graphique dans une fenêtre
		xdg-open "images/traitement_t.png"
		
		end=$(date +%s)	
		echo "Temps d'exécution : $((end-start)) secondes"
		return 1;;
		
	#Traitement -s
	
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
		
		set title "Distances des trajets (min, moyenne, max) des 50 premières valeurs"
		set xlabel "Id des trajets"
		set ylabel "Distances (km)"
		
		set xtics autofreq nomirror rotate by 60 right
		
		set style line 100 lc rgb "black" lw 0.5
		
		set datafile separator ';'
		
		plot 'temp/traitement-s.txt' using (2*\$0+1):2:4 with filledcurves lc rgb "#6984a3" fs transparent solid 0.5 title 'Min/Max', \
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
