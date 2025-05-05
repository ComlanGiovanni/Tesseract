#include "core/Renderer/OpenGLVertexArray.hpp"
#include "core/Core.hpp"
#include "core/Logger.hpp"
#include <GL/glew.h>

namespace Tesseract {

    // Fonction helper pour convertir notre ShaderDataType en type OpenGL de base (GL_FLOAT, etc.)
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::None:      TS_ASSERT(false, "ShaderDataType::None is not supported!"); return 0;
            case ShaderDataType::Float:     return GL_FLOAT;
            case ShaderDataType::Float2:    return GL_FLOAT;
            case ShaderDataType::Float3:    return GL_FLOAT;
            case ShaderDataType::Float4:    return GL_FLOAT;
            case ShaderDataType::Mat3:      return GL_FLOAT;
            case ShaderDataType::Mat4:      return GL_FLOAT;
            case ShaderDataType::Int:       return GL_INT;
            case ShaderDataType::Int2:      return GL_INT;
            case ShaderDataType::Int3:      return GL_INT;
            case ShaderDataType::Int4:      return GL_INT;
            case ShaderDataType::Bool:      return GL_BOOL;
        }
        // Ajout d'un default pour satisfaire -Wswitch et attraper les cas imprévus
        TS_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        glCreateVertexArrays(1, &m_RendererID);
        Logger::Trace("Created OpenGL VertexArray (ID: {})", m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        glDeleteVertexArrays(1, &m_RendererID);
        Logger::Trace("Deleted OpenGL VertexArray (ID: {})", m_RendererID);
    }

    void OpenGLVertexArray::Bind() const {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
        TS_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_RendererID); // Lie notre VAO
        vertexBuffer->Bind(); // Lie le VBO à ajouter

        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout) {
            glEnableVertexAttribArray(m_VertexBufferIndex);
            glVertexAttribPointer(
                m_VertexBufferIndex, // index de l'attribut (0 pour pos, 1 pour couleur, etc.)
                element.GetComponentCount(), // nombre de composants par vertex (ex: 3 pour vec3)
                ShaderDataTypeToOpenGLBaseType(element.Type), // type de données OpenGL
                element.Normalized ? GL_TRUE : GL_FALSE, // normaliser ?
                layout.GetStride(), // taille d'un vertex complet (stride)
                (const void*)element.Offset // décalage (offset) de cet attribut dans le vertex
            );
            Logger::Trace("  Added VBO attribute index {}, type {}, count {}, stride {}, offset {}",
                          m_VertexBufferIndex, (int)element.Type, element.GetComponentCount(), layout.GetStride(), element.Offset);
            m_VertexBufferIndex++; // Incrémente pour le prochain attribut
        }

        m_VertexBuffers.push_back(vertexBuffer); // Stocke la référence au VBO

        // On pourrait délier ici, mais ce n'est pas strictement nécessaire car
        // le VAO enregistre l'état.
        // vertexBuffer->Unbind();
        // glBindVertexArray(0);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
        glBindVertexArray(m_RendererID); // Lie notre VAO
        indexBuffer->Bind(); // Lie l'IBO

        m_IndexBuffer = indexBuffer; // Stocke la référence à l'IBO
        Logger::Trace("Set IndexBuffer (ID: {}) for VAO (ID: {})",
                      "(TODO: Get IBO ID)", m_RendererID); // TODO: Ajouter GetRendererID() à IndexBuffer si besoin

        // Décommenter si nécessaire, mais pas obligatoire
        // indexBuffer->Unbind();
        // glBindVertexArray(0);
    }

} // namespace Tesseract
