#pragma once

#include "core/Renderer/OrthographicCamera.hpp"
#include "core/Renderer/Texture.hpp" // On ajoutera Texture plus tard, mais préparons le terrain
#include "core/Renderer/Shader.hpp"
#include "core/Renderer/VertexArray.hpp"

#include <glm/glm.hpp>

namespace Tesseract {

    class Renderer2D {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera); // TODO: Camera 3D plus tard
        static void EndScene();
        static void Flush(); // Force le rendu du batch actuel

        // Primitives
        // Position + Couleur
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color); // z pour le tri éventuel

        // Position + Taille + Rotation + Couleur
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationRadians, const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationRadians, const glm::vec4& color);

        // TODO: Ajouter des versions avec textures
        // static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        // static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        // static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationRadians, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        // static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationRadians, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        // Statistiques de Rendu
        struct Statistics {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
        };
        static void ResetStats();
        static Statistics GetStats();

    private:
        // Fonctions internes pour gérer le batching
        static void StartBatch();
        static void NextBatch();
    };

} // namespace Tesseract
