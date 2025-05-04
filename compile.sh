#!/bin/bash

# Compilation avec g++
g++ -std=c++17 -Wall -Wextra -pedantic \
    -I/usr/include/SDL2 \
    -I/usr/include/GL \
    -L/usr/lib/x86_64-linux-gnu \
    src/main.cpp \
    -lSDL2 \
    -lSDL2main \
    -lGLEW \
    -lGL \
    -o tesseract

# Vérification de la compilation
if [ $? -eq 0 ]; then
    echo "Compilation réussie !"
    echo "Pour exécuter le programme : ./tesseract"
else
    echo "Erreur lors de la compilation"
fi
