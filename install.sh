#!/bin/bash

# Script d'installation propre de CMake pour éviter les problèmes précédents
# Version: 1.0

echo "=== Installation propre de CMake ==="
echo

# Détecter le système d'exploitation
detect_os() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        OS=$NAME
        DISTRO=$ID
    elif type lsb_release >/dev/null 2>&1; then
        OS=$(lsb_release -si)
        DISTRO=$(lsb_release -si | tr '[:upper:]' '[:lower:]')
    elif [ "$(uname)" == "Darwin" ]; then
        OS="macOS"
        DISTRO="macos"
    else
        OS=$(uname -s)
        DISTRO="unknown"
    fi
}

# Fonction pour installer CMake sur Ubuntu/Debian
install_ubuntu_debian() {
    echo "Installation sur Ubuntu/Debian..."

    # Mettre à jour la liste des paquets
    sudo apt-get update

    # Supprimer tout CMake existant si présent
    sudo apt-get remove --purge -y cmake cmake-data
    sudo apt-get autoremove -y

    # Installer CMake
    sudo apt-get install -y cmake

    # Vérifier l'installation
    cmake --version
}

# Fonction pour installer CMake sur macOS
install_macos() {
    echo "Installation sur macOS..."

    # Vérifier si Homebrew est installé
    if ! command -v brew >/dev/null 2>&1; then
        echo "Homebrew n'est pas installé. Installation de Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi

    # Désinstaller CMake si présent
    brew uninstall --force cmake 2>/dev/null || true
    brew cleanup

    # Installer CMake
    brew install cmake

    # Vérifier l'installation
    cmake --version
}

# Fonction pour installer CMake sur Fedora
install_fedora() {
    echo "Installation sur Fedora..."

    # Désinstaller CMake si présent
    sudo dnf remove -y cmake 2>/dev/null || true

    # Installer CMake
    sudo dnf install -y cmake

    # Vérifier l'installation
    cmake --version
}

# Fonction pour installer CMake sur Arch Linux
install_arch() {
    echo "Installation sur Arch Linux..."

    # Désinstaller CMake si présent
    sudo pacman -R --noconfirm cmake 2>/dev/null || true

    # Installer CMake
    sudo pacman -S --noconfirm cmake

    # Vérifier l'installation
    cmake --version
}

# Installation manuelle (alternative)
install_manual() {
    echo "Installation manuelle de CMake..."

    # Version à installer
    CMAKE_VERSION="3.29.2"

    # Créer un dossier temporaire
    TEMP_DIR=$(mktemp -d)
    cd "$TEMP_DIR"

    # Détecter l'architecture
    ARCH=$(uname -m)

    # Télécharger la version appropriée
    if [ "$(uname)" == "Darwin" ]; then
        # Pour macOS
        URL="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-macos-universal.tar.gz"
        FILENAME="cmake-${CMAKE_VERSION}-macos-universal.tar.gz"
        INSTALL_DIR="/opt/cmake"
    else
        # Pour Linux
        URL="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-${ARCH}.tar.gz"
        FILENAME="cmake-${CMAKE_VERSION}-linux-${ARCH}.tar.gz"
        INSTALL_DIR="/opt/cmake"
    fi

    echo "Téléchargement de CMake..."
    wget "$URL" -O "$FILENAME"

    # Extraire l'archive
    tar -xzvf "$FILENAME"

    # Supprimer l'ancienne installation si elle existe
    sudo rm -rf "$INSTALL_DIR"

    # Installer CMake
    DIR_NAME=$(tar -tzf "$FILENAME" | head -1 | cut -f1 -d"/")
    sudo mv "$DIR_NAME" "$INSTALL_DIR"

    # Créer des liens symboliques
    sudo ln -sf "$INSTALL_DIR/bin/cmake" /usr/local/bin/cmake
    sudo ln -sf "$INSTALL_DIR/bin/ctest" /usr/local/bin/ctest
    sudo ln -sf "$INSTALL_DIR/bin/cpack" /usr/local/bin/cpack
    sudo ln -sf "$INSTALL_DIR/bin/ccmake" /usr/local/bin/ccmake

    # Nettoyer
    cd - > /dev/null
    rm -rf "$TEMP_DIR"

    # Vérifier l'installation
    cmake --version
}

# Fonction pour configurer l'environnement
configure_environment() {
    echo
    echo "Configuration de l'environnement..."

    # S'assurer que CMAKE_ROOT n'est pas défini
    unset CMAKE_ROOT

    # Nettoyer les fichiers de configuration shell
    for file in ~/.bashrc ~/.zshrc ~/.profile ~/.bash_profile; do
        if [ -f "$file" ]; then
            sed -i '/CMAKE_ROOT/d' "$file"
            sed -i '/cmake/d' "$file"
        fi
    done

    # Créer le répertoire .cmake dans le home
    mkdir -p ~/.cmake

    echo "Configuration terminée."
}

# Fonction principale
main() {
    echo "Détection du système d'exploitation..."
    detect_os
    echo "Système détecté: $OS"
    echo

    # Proposer le type d'installation
    echo "Choisissez le type d'installation:"
    echo "1) Via le gestionnaire de paquets (recommandé)"
    echo "2) Installation manuelle"
    echo
    read -p "Votre choix (1 ou 2): " choice

    case $choice in
        1)
            case $DISTRO in
                ubuntu|debian)
                    install_ubuntu_debian
                    ;;
                macos)
                    install_macos
                    ;;
                fedora)
                    install_fedora
                    ;;
                arch)
                    install_arch
                    ;;
                *)
                    echo "Système non supporté pour l'installation automatique."
                    echo "Passons à l'installation manuelle..."
                    install_manual
                    ;;
            esac
            ;;
        2)
            install_manual
            ;;
        *)
            echo "Choix invalide. Installation annulée."
            exit 1
            ;;
    esac

    # Configurer l'environnement
    configure_environment

    echo
    echo "=== Installation terminée ==="
    echo
    echo "CMake a été installé avec succès!"
    echo "Pour tester l'installation, ouvrez un nouveau terminal et exécutez:"
    echo "  cmake --version"
    echo
    echo "Si vous rencontrez des problèmes, redémarrez votre terminal ou votre session."
}

# Exécuter le script
main
