#pragma once

#include <glm/glm.hpp> // Utilisation de GLM pour les matrices et vecteurs

namespace Tesseract {

    class OrthographicCamera {
    public:
        // Constructeur : Définit la projection orthographique
        OrthographicCamera(float left, float right, float bottom, float top);

        // Définit la projection après la création
        void SetProjection(float left, float right, float bottom, float top);

        // Getters
        const glm::vec3& GetPosition() const { return m_Position; }
        float GetRotation() const { return m_Rotation; }
        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

        // Setters
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
        void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

    private:
        // Recalcule la matrice de vue (et vue-projection) quand la position/rotation change
        void RecalculateViewMatrix();

        glm::mat4 m_ProjectionMatrix; // Matrice de projection
        glm::mat4 m_ViewMatrix;       // Matrice de vue (transformations caméra)
        glm::mat4 m_ViewProjectionMatrix; // Combinaison précalculée

        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f }; // Position de la caméra
        float m_Rotation = 0.0f; // Rotation autour de l'axe Z (pour la 2D)
    };

} // namespace Tesseract
