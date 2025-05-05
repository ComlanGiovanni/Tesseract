#include "core/Asset/AssetManager.hpp"
#include "core/Logger.hpp"
#include <filesystem>

namespace Tesseract {

    AssetManager& AssetManager::Get() {
        static AssetManager instance;
        return instance;
    }

    AssetManager::~AssetManager() {
        Clear();
    }

    Ref<Texture2D> AssetManager::LoadTexture(const std::string& filepath, const std::string& name) {
        std::string textureName = name.empty() ? ExtractNameFromFilePath(filepath) : name;

        // Vérifier si la texture est déjà chargée
        if (m_TextureCache.find(textureName) != m_TextureCache.end()) {
            Logger::Debug("Texture '{}' already loaded, returning cached version.", textureName);
            return m_TextureCache[textureName];
        }

        // Charger la nouvelle texture
        Ref<Texture2D> texture = Texture2D::Create(filepath);
        if (!texture) {
            Logger::Error("Failed to load texture: {}", filepath);
            return nullptr;
        }

        // Ajouter au cache
        m_TextureCache[textureName] = texture;
        Logger::Debug("Texture '{}' loaded successfully from '{}'.", textureName, filepath);

        return texture;
    }

    Ref<Texture2D> AssetManager::GetTexture(const std::string& name) {
        auto it = m_TextureCache.find(name);
        if (it != m_TextureCache.end()) {
            return it->second;
        }

        Logger::Warn("Texture '{}' not found in AssetManager!", name);
        return nullptr;
    }

    bool AssetManager::HasTexture(const std::string& name) const {
        return m_TextureCache.find(name) != m_TextureCache.end();
    }

    void AssetManager::RemoveTexture(const std::string& name) {
        auto it = m_TextureCache.find(name);
        if (it != m_TextureCache.end()) {
            m_TextureCache.erase(it);
            Logger::Debug("Texture '{}' removed from AssetManager.", name);
        }
    }

    std::vector<std::string> AssetManager::GetLoadedTextureNames() const {
        std::vector<std::string> names;
        names.reserve(m_TextureCache.size());

        for (const auto& [name, _] : m_TextureCache) {
            names.push_back(name);
        }

        return names;
    }

    void AssetManager::Clear() {
        m_TextureCache.clear();
        Logger::Debug("AssetManager cleared all resources.");
    }

    std::string AssetManager::ExtractNameFromFilePath(const std::string& filepath) const {
        namespace fs = std::filesystem;
        fs::path path(filepath);
        return path.stem().string(); // Retourne le nom du fichier sans extension
    }

} // namespace Tesseract
