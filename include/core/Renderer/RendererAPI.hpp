#pragma once

#include <glm/glm.hpp> // Pour la couleur (vec4)
#include "core/Renderer/VertexArray.hpp" // Pour VertexArray
#include "core/Core.hpp" // Pour Ref

namespace Tesseract {

    class RendererAPI {
    public:
        // Énumération pour identifier l'API (utile si on supporte plusieurs API)
        enum class API {
            None = 0, OpenGL = 1 // Ajouter DirectX, Vulkan, Metal etc. si besoin
        };

    public:
        virtual ~RendererAPI() = default;

        // Initialisation de l'API
        virtual void Init() = 0;

        // Définir la couleur de nettoyage
        virtual void SetClearColor(const glm::vec4& color) = 0;
        // Nettoyer l'écran
        virtual void Clear() = 0;

        // Définir la zone de rendu (viewport)
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        // Commande de dessin indexé
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

        // Obtenir l'API de rendu actuelle (statique)
        static API GetAPI() { return s_API; }

    private:
        // API de rendu actuellement utilisée (pourrait être changée globalement)
        static API s_API;
    };

} // namespace Tesseract
