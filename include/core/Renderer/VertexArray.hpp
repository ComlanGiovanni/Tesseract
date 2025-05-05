#pragma once

#include <memory> // Pour std::vector
#include <vector>
#include "core/Renderer/Buffer.hpp" // Pour VertexBuffer, IndexBuffer
#include "core/Core.hpp" // Pour Ref

namespace Tesseract {

    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        // Ajoute un VertexBuffer à ce VAO
        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
        // Définit l'IndexBuffer pour ce VAO
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

        // Getters
        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

        // Factory
        static Ref<VertexArray> Create();
    };

} // namespace Tesseract
