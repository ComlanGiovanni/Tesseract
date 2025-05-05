#include "core/Renderer/OpenGLTexture.hpp"
#include "core/Logger.hpp"
#include "core/Core.hpp"
#include "core/Renderer/RendererAPI.hpp"

// #define STB_IMAGE_IMPLEMENTATION // Supprimé car défini via g++ -D
#include <stb_image.h>
#include <GL/glew.h>

namespace Tesseract {

    // --- Implémentation de la factory Texture2D ---

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:    TS_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height);
            // Ajouter d'autres APIs ici (Vulkan, DirectX...)
        }
        TS_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path) {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:    TS_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
            // Ajouter d'autres APIs ici
        }
        TS_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    // --- Implémentation OpenGLTexture2D ---

    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height) {

        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

        // Paramètres par défaut
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }


    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : m_Path(path) {

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = nullptr;
        {
            // TODO: Mesurer le temps de chargement ?
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }

        if (data) {
            m_Width = width;
            m_Height = height;

            GLenum internalFormat = 0, dataFormat = 0;
            if (channels == 4) {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            } else if (channels == 3) {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
            // TODO: Gérer d'autres nombres de canaux si nécessaire (ex: grayscale)

            m_InternalFormat = internalFormat;
            m_DataFormat = dataFormat;

            TS_ASSERT(internalFormat & dataFormat, "Image format not supported! Path: {}, Channels: {}", path, channels);

            if(internalFormat & dataFormat) {
                 glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
                 glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

                 glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                 glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                 glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
                 glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

                 glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
            } else {
                 m_RendererID = 0; // Marquer comme invalide si format non supporté
                 m_Width = 0;
                 m_Height = 0;
            }

            stbi_image_free(data);
        } else {
            TS_ERROR("Failed to load image! Path: {}", path);
            m_Width = 0; m_Height = 0; m_RendererID = 0;
        }
    }

    OpenGLTexture2D::~OpenGLTexture2D() {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size) {
        // Calcule la taille attendue en octets
        uint32_t bpp = (m_DataFormat == GL_RGBA || m_DataFormat == GL_RGBA8) ? 4 : 3;
        uint32_t expectedSize = m_Width * m_Height * bpp;

        // Utilisation de TS_ASSERT formaté
        TS_ASSERT(size == expectedSize, "Data size mismatch! Expected {} bytes, got {}. Texture: {}x{}", expectedSize, size, m_Width, m_Height);
        if(size != expectedSize) return;

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const {
        glBindTextureUnit(slot, m_RendererID);
    }

} // namespace Tesseract
