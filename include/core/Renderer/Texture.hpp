#pragma once

#include "core/Core.hpp" // Pour Ref
#include "core/Renderer/RendererAPI.hpp" // Pour RendererAPI::GetAPI()
#include <string>
#include <cstdint> // Pour uint32_t

namespace Tesseract {

    // Interface de base pour toutes les textures
    class Texture {
    public:
        virtual ~Texture() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetRendererID() const = 0; // Pour l'ID OpenGL/API spécifique

        // Définit les données de la texture (potentiellement une partie seulement)
        virtual void SetData(void* data, uint32_t size) = 0;

        // Lie la texture à un slot donné (par défaut 0)
        virtual void Bind(uint32_t slot = 0) const = 0;

        // Pour la comparaison (utile pour le batching ou les maps)
        virtual bool operator==(const Texture& other) const = 0;
    };

    // Interface spécifique pour les textures 2D
    class Texture2D : public Texture {
    public:
        // Factory pour créer des textures 2D
        // Crée une texture vide avec largeur/hauteur spécifiées
        static Ref<Texture2D> Create(uint32_t width, uint32_t height);
        // Crée une texture en chargeant depuis un fichier
        static Ref<Texture2D> Create(const std::string& path);
    };


} // namespace Tesseract
