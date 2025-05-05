#include "core/Renderer/Buffer.hpp"
#include "core/Renderer/OpenGLBuffer.hpp"
#include "core/Core.hpp"
#include "core/Logger.hpp"

namespace Tesseract {

    // --- VertexBuffer Factory ---
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
        // Pour l'instant, on crée toujours un OpenGLVertexBuffer
        return CreateRef<OpenGLVertexBuffer>(size);
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
        return CreateRef<OpenGLVertexBuffer>(vertices, size);
    }

    // --- IndexBuffer Factory ---
    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
        return CreateRef<OpenGLIndexBuffer>(indices, count);
    }

} // namespace Tesseract
