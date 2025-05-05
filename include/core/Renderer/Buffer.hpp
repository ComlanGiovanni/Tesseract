#pragma once

#include <string>
#include <vector>
#include "core/Core.hpp" // Pour TS_ASSERT

namespace Tesseract {

    // Énumération pour les types de données dans les shaders
    enum class ShaderDataType {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    // Fonction utilitaire pour obtenir la taille d'un type de ShaderDataType
    static uint32_t ShaderDataTypeSize(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:    return 4;
            case ShaderDataType::Float2:   return 4 * 2;
            case ShaderDataType::Float3:   return 4 * 3;
            case ShaderDataType::Float4:   return 4 * 4;
            case ShaderDataType::Mat3:     return 4 * 3 * 3;
            case ShaderDataType::Mat4:     return 4 * 4 * 4;
            case ShaderDataType::Int:      return 4;
            case ShaderDataType::Int2:     return 4 * 2;
            case ShaderDataType::Int3:     return 4 * 3;
            case ShaderDataType::Int4:     return 4 * 4;
            case ShaderDataType::Bool:     return 1;
            case ShaderDataType::None:     break; // Gérer explicitement ou laisser le default
        }
        // Message plus spécifique pour l'assertion
        TS_ASSERT(false, "Unknown ShaderDataType! Type: {}", (int)type);
        return 0;
    }

    // Structure représentant un élément dans le layout d'un buffer
    struct BufferElement {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        size_t Offset; // Décalage de cet élément depuis le début du vertex
        bool Normalized;

        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

        // Retourne le nombre de composants (ex: 3 pour Float3)
        uint32_t GetComponentCount() const {
            switch (Type) {
                case ShaderDataType::Float:   return 1;
                case ShaderDataType::Float2:  return 2;
                case ShaderDataType::Float3:  return 3;
                case ShaderDataType::Float4:  return 4;
                case ShaderDataType::Mat3:    return 3; // 3* float3
                case ShaderDataType::Mat4:    return 4; // 4* float4
                case ShaderDataType::Int:     return 1;
                case ShaderDataType::Int2:    return 2;
                case ShaderDataType::Int3:    return 3;
                case ShaderDataType::Int4:    return 4;
                case ShaderDataType::Bool:    return 1;
                case ShaderDataType::None:    break; // Gérer explicitement ou laisser le default
            }
            TS_ASSERT(false, "Unknown ShaderDataType! Type: {}", (int)Type);
            return 0;
        }
    };

    // Classe décrivant le layout complet d'un vertex buffer
    class BufferLayout {
    public:
        // Constructeur par défaut
        BufferLayout() {}

        // Constructeur avec une liste d'initialisation d'éléments
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        // Getters
        uint32_t GetStride() const { return m_Stride; }
        const std::vector<BufferElement>& GetElements() const { return m_Elements; }

        // Itérateurs pour pouvoir faire un range-based for loop sur les éléments
        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        // Calcule les offsets de chaque élément et la taille totale (stride)
        void CalculateOffsetsAndStride() {
            size_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements) {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0; // Taille totale d'un vertex en octets
    };

    // --- Interface VertexBuffer ---
    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;

        // Factory
        static Ref<VertexBuffer> Create(uint32_t size); // Pour allocation dynamique
        static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
    };

    // --- Interface IndexBuffer ---
    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0; // Nombre d'indices

        // Factory
        static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
    };

} // namespace Tesseract
