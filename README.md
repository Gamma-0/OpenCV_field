# Lot 1 : Terrain


## Membres de l'équipe
- Nour Grati
- Marion Mounaix
- Margaux Seoane

## Notice d'utilisation
Pour compiler les binaires, il y a deux méthodes :
### Utilisation du makefile
A utiliser en premier :
1. Utiliser la commande `make` pour compiler les fichiers .c
2. Modifer le makefile
	- TEST_FOLDER: chemin vers les vérités terrains
	- RESULT_FOLDER: chemin ou les résultats du programme sont sauvegardés
3. Utiliser `make test` pour lancer les tests
4. `make clean` et `make cleanall` pour nettoyer le dossier.


### Utilisation des fichiers .pro
Sinon :
1. Utiliser la commande `qmake-qt4 field.pro`
2. `make`
3. `mkdir result`
4. `./field image_a_traiter.png nom_image_a_sauver` (il ne faut pas préciser l'extension pour la deuxième image)

## Fonctionnement de l'algorithme
1. Detection du vert dans l'image
2. Seuillage sur le vert
3. Detection de la plus grande partie verte par labeling
4. Appliquer des opérations morphologiques
5. Détection de la partie convexe pour éliminer les imperfections
6. Géneration du masque final
