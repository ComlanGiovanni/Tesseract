#include "core/Renderer/OpenGLRendererAPI.hpp"
#include "core/Logger.hpp"

#include <GL/glew.h>

namespace Tesseract {

    void OpenGLRendererAPI::Init() {
        Logger::Info("Initializing OpenGL RendererAPI...");
        // Activer le blending pour la transparence
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Activer le test de profondeur
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear() {
        // Nettoyer les buffers de couleur et de profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
        vertexArray->Bind();
        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        // Dessiner les triangles en utilisant les indices
        // Le dernier argument (nullptr ici) est pour l'offset dans l'IBO, si nécessaire.
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        // Optionnel: délier le VAO après usage pour éviter les états persistants
        // vertexArray->Unbind();
    }

} // namespace Tesseract
