#pragma once

namespace Tesseract {

    class Timestep {
    public:
        // Constructeur : prend le temps en secondes
        Timestep(float time = 0.0f)
            : m_Time(time) {}

        // Opérateur de conversion implicite vers float (pour la facilité d'utilisation)
        operator float() const { return m_Time; }

        // Récupère le temps en secondes
        float GetSeconds() const { return m_Time; }

        // Récupère le temps en millisecondes
        float GetMilliseconds() const { return m_Time * 1000.0f; }

    private:
        float m_Time; // Temps stocké en secondes
    };

} // namespace Tesseract
