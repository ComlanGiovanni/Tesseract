#pragma once

#include <utility> // Pour std::pair

namespace Tesseract {

    // Classe statique pour gérer les entrées (clavier/souris)
    class Input {
    public:
        // Désactiver constructeur/destructeur (classe statique)
        Input() = delete;

        // Vérifie si une touche spécifique est pressée
        static bool IsKeyPressed(int keycode);

        // Vérifie si un bouton de la souris est pressé
        static bool IsMouseButtonPressed(int button);

        // Récupère la position actuelle de la souris
        static std::pair<int, int> GetMousePosition();

        // Raccourcis pour récupérer X et Y séparément
        static int GetMouseX();
        static int GetMouseY();
    };

} // namespace Tesseract
