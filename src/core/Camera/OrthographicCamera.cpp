#include "core/Camera/OrthographicCamera.hpp"

#include <glm/gtc/matrix_transform.hpp> // Pour glm::ortho, glm::translate, glm::rotate

namespace Tesseract {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        // Initialise la matrice de projection via glm::ortho
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
          m_ViewMatrix(1.0f) // Initialise la matrice de vue à l'identité
    {
        // Calcule la matrice combinée initiale
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
        // Met à jour la matrice de projection
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        // Recalcule la matrice combinée
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix() {
        // Crée une transformation identité
        glm::mat4 transform = glm::mat4(1.0f);
        // Applique la translation inverse de la position de la caméra
        transform = glm::translate(transform, -m_Position);
        // Applique la rotation inverse autour de l'axe Z
        transform = glm::rotate(transform, glm::radians(-m_Rotation), glm::vec3(0, 0, 1));

        // Le résultat est la matrice de vue
        m_ViewMatrix = transform;
        // Recalcule la matrice combinée
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

} // namespace Tesseract
