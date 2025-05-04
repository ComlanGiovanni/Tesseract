#!/bin/bash

# Script pour supprimer complètement CMake du système
# ATTENTION: Ce script nécessite les droits sudo

echo "=== Suppression complète de CMake ==="
echo

# 1. Désinstaller via le gestionnaire de paquets
echo "1. Désinstallation via le gestionnaire de paquets..."
if command -v apt-get >/dev/null 2>&1; then
    sudo apt-get purge -y cmake cmake-data
    sudo apt-get autoremove -y
    sudo apt-get autoclean
elif command -v brew >/dev/null 2>&1; then
    brew uninstall --force cmake
    brew cleanup
elif command -v dnf >/dev/null 2>&1; then
    sudo dnf remove -y cmake
elif command -v pacman >/dev/null 2>&1; then
    sudo pacman -R cmake
else
    echo "Gestionnaire de paquets non reconnu."
fi

# 2. Supprimer les binaires potentiels
echo
echo "2. Suppression des binaires..."
sudo rm -f /usr/bin/cmake
sudo rm -f /usr/local/bin/cmake
sudo rm -f /opt/cmake/bin/cmake
sudo rm -f /snap/bin/cmake
sudo rm -f ~/bin/cmake

# 3. Supprimer les répertoires d'installation
echo
echo "3. Suppression des répertoires d'installation..."
sudo rm -rf /usr/share/cmake
sudo rm -rf /usr/share/cmake-*
sudo rm -rf /etc/cmake
sudo rm -rf /opt/cmake
sudo rm -rf /opt/cmake-*
sudo rm -rf /Applications/CMake.app/  # pour macOS

# 4. Supprimer les snap packages
echo
echo "4. Suppression des installations snap..."
if command -v snap >/dev/null 2>&1; then
    sudo snap remove cmake
fi

# 5. Supprimer les fichiers de configuration
echo
echo "5. Suppression des fichiers de configuration..."
rm -rf ~/.cmake
rm -rf ~/.cmake-*
rm -rf ~/.config/CMake
rm -rf ~/.CMake*

# 6. Nettoyer le cache
echo
echo "6. Nettoyage du cache..."
rm -rf ~/.cache/cmake
rm -rf /var/cache/cmake
sudo rm -rf /var/lib/cmake

# 7. Supprimer les variables d'environnement persistantes
echo
echo "7. Nettoyage des variables d'environnement..."
unset CMAKE_ROOT
unset CMAKE_PREFIX_PATH
unset CMAKE_MODULE_PATH

# Nettoyer les fichiers de configuration shell
for file in ~/.bashrc ~/.zshrc ~/.profile ~/.bash_profile; do
    if [ -f "$file" ]; then
        sed -i '/CMAKE_ROOT/d' "$file"
        sed -i '/CMAKE_PREFIX_PATH/d' "$file"
        sed -i '/CMAKE_MODULE_PATH/d' "$file"
        sed -i '/cmake/d' "$file"
    fi
done

# 8. Vérifier que CMake est bien supprimé
echo
echo "8. Vérification finale..."
if command -v cmake >/dev/null 2>&1; then
    echo "⚠️ CMake est toujours présent dans le système."
    echo "Emplacement: $(which cmake)"
else
    echo "✅ CMake a été complètement supprimé du système."
fi

echo
echo "=== Nettoyage terminé ==="
echo
echo "Pour installer une nouvelle version de CMake:"
echo "1. Téléchargez-la depuis le site officiel de CMake"
echo "2. Ou utilisez votre gestionnaire de paquets préféré"
echo "   - apt-get install cmake (Ubuntu/Debian)"
echo "   - brew install cmake (macOS)"
echo "   - dnf install cmake (Fedora)"
echo

# Redémarrer le shell pour appliquer les changements
echo "Il est recommandé de redémarrer votre terminal ou votre session pour appliquer tous les changements."
