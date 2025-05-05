#include "core/Renderer/Renderer2D.hpp"
#include "core/Renderer/VertexArray.hpp"
#include "core/Renderer/Shader.hpp"
#include "core/Renderer/RenderCommand.hpp"
#include "core/Logger.hpp"
#include "core/Core.hpp"
#include "core/Renderer/Texture.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <array> // Pour std::array (si utilisé pour les textures slots)

namespace Tesseract {

    // Structure pour stocker les données du quad pour le batch rendering
    struct QuadVertex {
        glm::vec3 Position;
        glm::vec4 Color;
        // glm::vec2 TexCoord; // Pour les textures
        // float TexIndex;
        // float TilingFactor;
    };

    // Données internes du Renderer2D
    struct Renderer2DData {
        static const uint32_t MaxQuads = 10000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        // static const uint32_t MaxTextureSlots = 32; // TODO: Pour le rendu multi-textures

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        // std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        // uint32_t TextureSlotIndex = 1; // 0 = White Texture

        glm::vec4 QuadVertexPositions[4];

        // Suppression de la définition interne de Statistics
        // struct Statistics { ... };
        // Utilisation du type défini dans Renderer2D.hpp
        Renderer2D::Statistics Stats;
    };

    static Renderer2DData s_Data;

    void Renderer2D::Init() {
        Logger::Info("Initializing Renderer2D...");

        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" }
            // Ajouter ici les autres attributs (TexCoord, TexIndex, TilingFactor) quand ils seront utilisés
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
            offset += 4;
        }
        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        // Création de la texture blanche
        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        // Création du Shader
        // Assurez-vous que le chemin est correct par rapport à l'exécutable
        // Si l'exécutable est dans build/, alors ../assets/ est correct.
        s_Data.TextureShader = Shader::Create("../assets/shaders/Texture.glsl");
        TS_ASSERT(s_Data.TextureShader, "Failed to load Texture shader!");

        // Configuration initiale du shader (si nécessaire, ex: définir les slots de texture)
        // s_Data.TextureShader->Bind();
        // int samplers[s_Data.MaxTextureSlots];
        // for(uint32_t i = 0; i < s_Data.MaxTextureSlots; ++i) samplers[i] = i;
        // s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

        // Initialiser les slots de texture (optionnel pour le moment)
        // s_Data.TextureSlots[0] = s_Data.WhiteTexture;
        // s_Data.TextureSlotIndex = 1; // Commencer à remplir à partir du slot 1

        // Positions de base des sommets
        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::Shutdown() {
        Logger::Info("Shutting down Renderer2D...");
        delete[] s_Data.QuadVertexBufferBase;
        // Les Refs (VAO, VBO, Shader, Texture) seront nettoyés automatiquement
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera) {
        TS_ASSERT(s_Data.TextureShader, "Texture shader is not initialized!");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        StartBatch(); // Commence un nouveau batch
    }

    void Renderer2D::EndScene() {
        Flush(); // Dessine tout ce qui reste dans le batch
    }

    void Renderer2D::StartBatch() {
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        // s_Data.TextureSlotIndex = 1; // Réinitialiser l'index du slot de texture
    }

    void Renderer2D::Flush() {
        if (s_Data.QuadIndexCount == 0)
            return; // Rien à dessiner

        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        // Lier les textures (pour plus tard)
        // for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        //     s_Data.TextureSlots[i]->Bind(i);

        // Utiliser la texture blanche par défaut pour le moment
        s_Data.WhiteTexture->Bind(0);

        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer2D::NextBatch() {
        EndScene();
        Flush();
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        // Suppression de l'affectation de TextureSlotIndex car commenté/inutilisé
        // s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
            NextBatch(); // Remplacé FlushAndReset par NextBatch
        }

        const float quadVertices[4 * 5] = {
            position.x,             position.y,             0.0f, 0.0f, 0.0f,
            position.x + size.x,    position.y,             0.0f, 1.0f, 0.0f,
            position.x + size.x,    position.y + size.y,    0.0f, 1.0f, 1.0f,
            position.x,             position.y + size.y,    0.0f, 0.0f, 1.0f
        };

        [[maybe_unused]] const float texIndex = 0.0f; // Texture blanche par défaut
        [[maybe_unused]] const float tilingFactor = 1.0f;

        for (int i = 0; i < 4; ++i) {
            s_Data.QuadVertexBufferPtr->Position = { quadVertices[i*5 + 0], quadVertices[i*5 + 1], quadVertices[i*5 + 2] };
            s_Data.QuadVertexBufferPtr->Color = color;
            // Suppression des affectations TexCoord, TexIndex, TilingFactor
            // s_Data.QuadVertexBufferPtr->TexCoord = { quadVertices[i*5 + 3], quadVertices[i*5 + 4] };
            // s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            // s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }


    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationRadians, const glm::vec4& color) {
         if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
            NextBatch();
        }

        [[maybe_unused]] const float texIndex = 0.0f;
        // Suppression de texCoords car non utilisée
        // const glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        [[maybe_unused]] const float tilingFactor = 1.0f;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                            * glm::rotate(glm::mat4(1.0f), rotationRadians, { 0.0f, 0.0f, 1.0f })
                            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        glm::vec4 quadVertexPositions[4];
        quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        for (int i = 0; i < 4; i++) {
            s_Data.QuadVertexBufferPtr->Position = transform * quadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::ResetStats() {
        // Utiliser l'initialisation par agrégats au lieu de memset
        s_Data.Stats = {};
    }

    Renderer2D::Statistics Renderer2D::GetStats() {
        return s_Data.Stats; // Conversion implicite ok maintenant
    }

} // namespace Tesseract
