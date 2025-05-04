#!/bin/bash

# Définition du chemin absolu
PROJECT_DIR="/media/ysera/Extreme SSD/sandbox/Tesseract"

# Création du répertoire build s'il n'existe pas
mkdir -p "$PROJECT_DIR/build"
cd "$PROJECT_DIR/build"

# Configuration avec CMake
cmake "$PROJECT_DIR"

# Compilation
make

# Vérification de la compilation
if [ $? -eq 0 ]; then
    echo "Compilation réussie !"
    echo "Pour exécuter le programme : ./Tesseract"
else
    echo "Erreur lors de la compilation"
fi
