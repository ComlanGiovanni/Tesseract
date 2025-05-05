#pragma once

#include <string>
#include <functional>
#include "core/Core.hpp" // Pour Scope
#include "core/Event.hpp"

// Forward declaration de SDL_Window et SDL_GLContext
struct SDL_Window;
typedef void* SDL_GLContext;

namespace Tesseract {

    // Propriétés pour la création de la fenêtre
    struct WindowProps {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        WindowProps(const std::string& title = "Tesseract Engine",
                    uint32_t width = 1600,
                    uint32_t height = 900)
            : Title(title), Width(width), Height(height) {}
    };

    // Interface représentant une fenêtre de bureau
    // !! NOTE: Pour simplifier suite aux erreurs de compilation, on la rend concrete
    // Normalement, ce serait une interface pure et SDLWindow hériterait.
    class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        Window(const WindowProps& props); // Déplacé de protected à public et implémenté dans Window.cpp
        virtual ~Window(); // Retrait de = default

        // Retrait de = 0 pour rendre la classe concrete
        virtual void OnUpdate();
        virtual void SwapBuffers();
        virtual uint32_t GetWidth() const;
        virtual uint32_t GetHeight() const;
        virtual void SetEventCallback(const EventCallbackFn& callback);
        virtual void SetVSync(bool enabled);
        virtual bool IsVSync() const;
        virtual void* GetNativeWindow() const;
        virtual void* GetNativeContext() const;

        static Scope<Window> Create(const WindowProps& props = WindowProps());

    private: // Mise en private des membres d'implémentation
        // Méthodes d'initialisation et de fermeture (appelées par constructeur/destructeur)
        void Init(const WindowProps& props);
        void Shutdown();

        // Données spécifiques à l'implémentation (SDL ici)
        SDL_Window* m_Window = nullptr;
        SDL_GLContext m_Context = nullptr;

        // Données communes stockées
        struct WindowData {
            std::string Title;
            uint32_t Width, Height;
            bool VSync;
            EventCallbackFn EventCallback;

            // Constructeur pour initialiser depuis WindowProps
            WindowData(const WindowProps& props)
                : Title(props.Title), Width(props.Width), Height(props.Height), VSync(true) {}
        };

        WindowData m_Data;
    };

} // namespace Tesseract
