#include "core/Renderer/OpenGLBuffer.hpp"
#include "core/Core.hpp"
#include "core/Logger.hpp"
#include <GL/glew.h>

namespace Tesseract {

    // --- VertexBuffer OpenGL ---

    // Constructeur pour buffer dynamique
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        // Allouer la mémoire sans données initiales, usage dynamique
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        Logger::Trace("Created dynamic OpenGL VertexBuffer (ID: {})", m_RendererID);
    }

    // Constructeur pour données statiques initiales
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
        glCreateBuffers(1, &m_RendererID); // Crée le buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // Le lie
        // Envoie les données, usage statique
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        Logger::Trace("Created static OpenGL VertexBuffer (ID: {}) with size {}", m_RendererID, size);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
        Logger::Trace("Deleted OpenGL VertexBuffer (ID: {})", m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        // Met à jour (une partie ou tout) le contenu du buffer
        // Le 0 indique qu'on commence au début du buffer
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    // --- IndexBuffer OpenGL ---

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
        : m_Count(count)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        // Envoie les données, usage statique pour les indices généralement
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
        Logger::Trace("Created OpenGL IndexBuffer (ID: {}) with count {}", m_RendererID, count);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
        Logger::Trace("Deleted OpenGL IndexBuffer (ID: {})", m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

} // namespace Tesseract
