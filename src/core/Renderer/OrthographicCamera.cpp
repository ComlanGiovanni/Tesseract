#include "core/Renderer/OrthographicCamera.hpp"

namespace Tesseract {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), // zNear=-1, zFar=1 pour 2D simple
          m_ViewMatrix(1.0f) // Matrice identité au départ
    {
        // Calcul initial de la matrice combinée
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; // Recalculer
    }

    void OrthographicCamera::RecalculateViewMatrix() {
        // Calcul de la transformation inverse : Translation puis Rotation
        // Note: L'ordre est important pour obtenir la matrice de VUE
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        // La matrice de vue est l'inverse de la transformation de la caméra
        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; // Recalculer la matrice combinée
    }

} // namespace Tesseract
