#pragma once

#include "core/Core.hpp"
#include "core/Renderer/Texture.hpp"
#include <unordered_map>
#include <string>
#include <memory>

namespace Tesseract {

    /**
     * @class AssetManager
     * @brief Système centralisé de gestion des ressources du moteur
     *
     * Permet de charger, stocker et récupérer différents types de ressources
     * tels que des textures, fonts, etc. Gère le cache pour éviter les chargements
     * redondants.
     */
    class AssetManager {
    public:
        /**
         * @brief Obtient l'instance unique de l'AssetManager (singleton)
         * @return Référence à l'instance unique
         */
        static AssetManager& Get();

        // --- Gestion des textures ---

        /**
         * @brief Charge une texture depuis un fichier
         * @param filepath Chemin du fichier texture
         * @param name Nom optionnel pour référencer la texture (utilise le nom du fichier si non spécifié)
         * @return Référence à la texture chargée
         */
        Ref<Texture2D> LoadTexture(const std::string& filepath, const std::string& name = "");

        /**
         * @brief Récupère une texture déjà chargée par son nom
         * @param name Nom de la texture à récupérer
         * @return Référence à la texture, ou nullptr si non trouvée
         */
        Ref<Texture2D> GetTexture(const std::string& name);

        /**
         * @brief Vérifie si une texture existe dans le gestionnaire
         * @param name Nom de la texture à vérifier
         * @return true si la texture existe, false sinon
         */
        bool HasTexture(const std::string& name) const;

        /**
         * @brief Supprime une texture du gestionnaire
         * @param name Nom de la texture à supprimer
         */
        void RemoveTexture(const std::string& name);

        /**
         * @brief Obtient la liste des noms de textures chargées
         * @return Vecteur contenant les noms des textures
         */
        std::vector<std::string> GetLoadedTextureNames() const;

        // --- Gestion générale ---

        /**
         * @brief Nettoie toutes les ressources
         */
        void Clear();

    private:
        AssetManager() = default;  // Constructeur privé (singleton)
        ~AssetManager();

        // Cache de textures (nom -> texture)
        std::unordered_map<std::string, Ref<Texture2D>> m_TextureCache;

        // Extrait le nom du fichier à partir d'un chemin
        std::string ExtractNameFromFilePath(const std::string& filepath) const;
    };

} // namespace Tesseract
