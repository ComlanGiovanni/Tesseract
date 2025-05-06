#pragma once // Ajouté pour la propreté

#include "core/Renderer/Shader.hpp" // Inclure la classe de base
#include <glm/glm.hpp>
#include <unordered_map> // Pour le cache d'uniforms
#include <string> // Pour std::string
#include <GL/glew.h> // Ajout de l'include pour les types GL (GLint, GLenum)

typedef unsigned int GLenum; // Peut être redondant avec glew.h mais ne nuit pas

namespace Tesseract {

    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader() override;

        void Bind() const override;
        void Unbind() const override;

        // Implémentation des SetUniform
        void SetInt(const std::string& name, int value) override;
        void SetFloat(const std::string& name, float value) override;
        void SetFloat2(const std::string& name, const glm::vec2& value) override;
        void SetFloat3(const std::string& name, const glm::vec3& value) override;
        void SetFloat4(const std::string& name, const glm::vec4& value) override;
        void SetMat3(const std::string& name, const glm::mat3& matrix) override;
        void SetMat4(const std::string& name, const glm::mat4& matrix) override;

        // Helper pour la matrice de vue-projection
        void SetViewProjection(const glm::mat4& matrix);

        const std::string& GetName() const override { return m_Name; }

        // Fonctions d'upload spécifiques à OpenGL
        void UploadUniformInt(const std::string& name, int value);
        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
        GLint GetUniformLocation(const std::string& name) const;

        std::string m_FilePath;
        std::string m_Name;
        uint32_t m_RendererID;
        mutable std::unordered_map<std::string, int> m_UniformLocationCache;
    };

} // namespace Tesseract
