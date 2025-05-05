#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp> // Pour les uniforms (matrice, vecteurs)
#include "core/Core.hpp" // Inclure Core.hpp pour Ref

namespace Tesseract {

    class Shader {
    public:
        virtual ~Shader() = default; // Destructeur virtuel par défaut

        // Méthodes virtuelles pures : doivent être implémentées par les classes dérivées
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        // --- Gestion des Uniforms ---
        // ---------------------------

        // Fonctions SetUniform virtuelles pures ajoutées
        virtual void SetInt(const std::string& name, int value) = 0;
        virtual void SetFloat(const std::string& name, float value) = 0;
        virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
        virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;

        virtual const std::string& GetName() const = 0;

        // Méthode statique pour créer un shader (Factory Pattern simple)
        // Pour l'instant, crée directement un OpenGLShader.
        // Prend le chemin vers UN SEUL fichier contenant les deux types de shader
        // (séparés par #type vertex/fragment)
        static Ref<Shader> Create(const std::string& filepath);

        // TODO: Ajouter une méthode Create qui prend directement les sources vertex/fragment
        // static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    };

} // namespace Tesseract
