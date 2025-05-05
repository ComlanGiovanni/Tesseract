#pragma once

#include "core/Renderer/Shader.hpp"
#include <GL/glew.h> // Utiliser GLEW au lieu de Glad
#include <glm/glm.hpp>

namespace Tesseract {

    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& filepath);
        // TODO: Constructeur prenant les sources directes
        // OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        // Implémentation des Setters d'Uniforms
        virtual void SetUniformInt(const std::string& name, int value) override;
        virtual void SetUniformFloat(const std::string& name, float value) override;
        virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) override;
        virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) override;
        virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) override;
        virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) override;

    private:
        // Fonctions utilitaires internes
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
        GLint GetUniformLocation(const std::string& name);

        uint32_t m_RendererID; // ID du programme shader OpenGL
        std::string m_FilePath; // Garder le chemin pour debug/rechargement potentiel
        // Cache pour les locations d'uniforms (optimisation)
        std::unordered_map<std::string, GLint> m_UniformLocationCache;
    };

} // namespace Tesseract
