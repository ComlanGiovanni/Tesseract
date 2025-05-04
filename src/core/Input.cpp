#include "core/Input.hpp"
#include "core/Application.hpp" // Pour accéder à la fenêtre SDL
#include <SDL2/SDL.h>

namespace Tesseract {

    bool Input::IsKeyPressed(int keycode) {
        // Récupère l'état actuel de toutes les touches
        const Uint8* state = SDL_GetKeyboardState(NULL);
        // Vérifie si la touche correspondant au scancode SDL est pressée
        // Note: SDL utilise des Scancodes, pas des Keycodes ici.
        // Il faudrait une table de mapping si on veut utiliser des Keycodes abstraits.
        // Pour l'instant, on utilisera directement les SDL_SCANCODE_*
        if (keycode < SDL_NUM_SCANCODES) { // Vérification de sécurité
            return state[keycode];
        }
        return false;
    }

    bool Input::IsMouseButtonPressed(int button) {
        // Récupère l'état actuel de la souris
        Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
        // Vérifie si le bouton spécifique est pressé en utilisant les masques SDL
        return (mouseState & SDL_BUTTON(button));
    }

    std::pair<int, int> Input::GetMousePosition() {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return {x, y};
    }

    int Input::GetMouseX() {
        return GetMousePosition().first;
    }

    int Input::GetMouseY() {
        return GetMousePosition().second;
    }

} // namespace Tesseract
