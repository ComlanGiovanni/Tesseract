# Tesseract

Un environnement de développement de jeu moderne en C++ pour explorer les concepts fondamentaux du développement de jeux.

## Prérequis

- C++17 ou supérieur
- CMake 3.15 ou supérieur
- SDL2
- OpenGL 3.3 ou supérieur
- GLEW
- Compilateur C++ moderne (GCC, Clang, ou MSVC)

## Installation des dépendances

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libsdl2-dev libglew-dev
```

### Fedora
```bash
sudo dnf install gcc-c++ cmake SDL2-devel glew-devel
```

### macOS (avec Homebrew)
```bash
brew install cmake sdl2 glew
```

## Compilation

```bash
# Créer un répertoire de build
mkdir build
cd build

# Configurer avec CMake
cmake ..

# Compiler
make
```

## Structure du projet

```
tesseract/
├── src/                 # Fichiers sources C++
├── include/             # Fichiers d'en-tête
├── lib/                 # Bibliothèques externes
├── assets/              # Ressources (textures, audio, shaders)
├── CMakeLists.txt       # Script de build principal
└── README.md            # Documentation
```

## Fonctionnalités prévues

- [ ] Boucle de jeu personnalisée
- [ ] Gestion des entrées avec SDL2
- [ ] Rendu OpenGL (2D, shaders, 3D)
- [ ] Système ECS (Entity-Component-System)
- [ ] Interface de débogage avec ImGui
- [ ] Chargement d'assets
- [ ] Intégration de scripts (Lua)
- [ ] Physique et réseau de base

## Licence

Ce projet est sous licence MIT. Voir le fichier LICENSE pour plus de détails.
