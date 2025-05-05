#pragma once

#include "core/Renderer/Texture.hpp"
#include <GL/glew.h> // Utiliser GLEW au lieu de Glad
#include <string> // Pour std::string

namespace Tesseract {

    class OpenGLTexture2D : public Texture2D {
    public:
        // Constructeur pour créer une texture vide
        OpenGLTexture2D(uint32_t width, uint32_t height);
        // Constructeur pour charger depuis un fichier
        OpenGLTexture2D(const std::string& path);

        virtual ~OpenGLTexture2D();

        virtual uint32_t GetWidth() const override { return m_Width; }
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual uint32_t GetRendererID() const override { return m_RendererID; }

        virtual void SetData(void* data, uint32_t size) override;

        virtual void Bind(uint32_t slot = 0) const override;

        virtual bool operator==(const Texture& other) const override {
            // Compare simplement les IDs OpenGL
            return m_RendererID == static_cast<const OpenGLTexture2D&>(other).m_RendererID;
        }

    private:
        std::string m_Path; // Chemin du fichier (si chargé)
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID; // ID OpenGL de la texture
        GLenum m_InternalFormat, m_DataFormat; // Formats OpenGL (ex: GL_RGBA8, GL_RGBA)
    };

} // namespace Tesseract
