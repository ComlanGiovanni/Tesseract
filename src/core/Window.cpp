#include "core/Window.hpp"
#include "core/Logger.hpp"
#include "core/Core.hpp" // Pour TS_ASSERT et Scope/CreateScope
#include <GL/glew.h>
#include <SDL.h> // Pour les fonctions SDL
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>

// Inclure les événements spécifiques
#include "core/Events/ApplicationEvent.hpp"
#include "core/Events/KeyEvent.hpp"
#include "core/Events/MouseEvent.hpp"

namespace Tesseract {

    // --- Window Implementation --- //

    // Factory
    Scope<Window> Window::Create(const WindowProps& props) {
        // À l'avenir, on pourrait avoir une classe SDLWindow qui hérite de Window.
        // return CreateScope<SDLWindow>(props);
        // Pour l'instant, comme l'implémentation est dans la classe de base Window,
        // on retourne juste une instance de Window.
        // C'est un peu inhabituel mais fonctionne pour cette structure simplifiée.
        return CreateScope<Window>(props);
    }

    // Constructeur appelé par Create
    Window::Window(const WindowProps& props)
     : m_Data(props) // Initialise m_Data ici
    {
        Init(props); // Appelle la méthode d'initialisation
    }

    // Destructeur
    Window::~Window() {
        Shutdown(); // Appelle la méthode de fermeture
    }

    // Initialisation (ancien contenu du constructeur)
    void Window::Init([[maybe_unused]] const WindowProps& props) {
        Logger::Info("Initializing Window: {} ({}, {})", m_Data.Title, m_Data.Width, m_Data.Height);

        int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
        TS_ASSERT(sdlResult == 0, "Could not initialize SDL: {}", SDL_GetError());
        if (sdlResult != 0) return;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        m_Window = SDL_CreateWindow(
            m_Data.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            m_Data.Width, m_Data.Height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
        );
        TS_ASSERT(m_Window, "Could not create SDL window: {}", SDL_GetError());
        if (!m_Window) { SDL_Quit(); return; }

        m_Context = SDL_GL_CreateContext(m_Window);
        TS_ASSERT(m_Context, "Could not create OpenGL context: {}", SDL_GetError());
        if (!m_Context) { SDL_DestroyWindow(m_Window); SDL_Quit(); return; }

        SetVSync(m_Data.VSync); // Appliquer l'état VSync initial

        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        TS_ASSERT(glewError == GLEW_OK, "Failed to initialize GLEW: {}", (const char*)glewGetErrorString(glewError));
        if (glewError != GLEW_OK) {
            Shutdown(); // Utiliser Shutdown pour nettoyer
            return;
        }
        Logger::Info("GLEW Initialized.");

        Logger::Info("OpenGL Info:");
        Logger::Info("  Vendor: {}", (const char*)glGetString(GL_VENDOR));
        Logger::Info("  Renderer: {}", (const char*)glGetString(GL_RENDERER));
        Logger::Info("  Version: {}", (const char*)glGetString(GL_VERSION));

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        SDL_SetWindowData(m_Window, "WindowDataPtr", &m_Data);
        Logger::Info("Window initialized successfully.");
    }

    // Fermeture (ancien contenu du destructeur)
    void Window::Shutdown() {
        Logger::Warn("Shutting down window...");
        if (m_Context) {
            SDL_GL_DeleteContext(m_Context);
            m_Context = nullptr;
        }
        if (m_Window) {
            SDL_DestroyWindow(m_Window);
            m_Window = nullptr;
        }
        SDL_Quit();
        Logger::Warn("Window shutdown complete.");
    }

    // Mise à jour (Polling d'événements)
    void Window::OnUpdate() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (m_Data.EventCallback) {
                // --- Traduction des événements SDL vers événements Tesseract ---
                switch (event.type) {
                    case SDL_QUIT:
                    {
                        WindowCloseEvent closeEvent;
                        m_Data.EventCallback(closeEvent);
                        break;
                    }
                    case SDL_WINDOWEVENT:
                    {
                        switch (event.window.event) {
                            case SDL_WINDOWEVENT_RESIZED:
                            {
                                m_Data.Width = event.window.data1;
                                m_Data.Height = event.window.data2;
                                WindowResizeEvent resizeEvent(m_Data.Width, m_Data.Height);
                                m_Data.EventCallback(resizeEvent);
                                break;
                            }
                            case SDL_WINDOWEVENT_CLOSE:
                            {
                                WindowCloseEvent closeEvent;
                                m_Data.EventCallback(closeEvent);
                                break;
                            }
                        }
                        break;
                    }
                    case SDL_KEYDOWN:
                    {
                        KeyPressedEvent keyEvent(event.key.keysym.scancode, event.key.repeat);
                        m_Data.EventCallback(keyEvent);
                        break;
                    }
                    case SDL_KEYUP:
                    {
                        KeyReleasedEvent keyEvent(event.key.keysym.scancode);
                        m_Data.EventCallback(keyEvent);
                        break;
                    }
                    case SDL_MOUSEMOTION:
                    {
                        MouseMovedEvent mouseEvent(event.motion.x, event.motion.y);
                        m_Data.EventCallback(mouseEvent);
                        break;
                    }
                    case SDL_MOUSEWHEEL:
                    {
                        MouseScrolledEvent scrollEvent((float)event.wheel.x, (float)event.wheel.y);
                        m_Data.EventCallback(scrollEvent);
                        break;
                    }
                    case SDL_MOUSEBUTTONDOWN:
                    {
                        MouseButtonPressedEvent buttonEvent(event.button.button);
                        m_Data.EventCallback(buttonEvent);
                        break;
                    }
                    case SDL_MOUSEBUTTONUP:
                    {
                        MouseButtonReleasedEvent buttonEvent(event.button.button);
                        m_Data.EventCallback(buttonEvent);
                        break;
                    }
                }
            }
        }

        // Échanger les buffers à la fin de chaque mise à jour
        SDL_GL_SwapWindow(m_Window);
    }

    // Échange des buffers
    void Window::SwapBuffers() {
        SDL_GL_SwapWindow(m_Window);
    }

    // Définition des Setters/Getters virtuels
    void Window::SetEventCallback(const EventCallbackFn& callback) {
        m_Data.EventCallback = callback;
    }

    void Window::SetVSync(bool enabled) {
        if (SDL_GL_SetSwapInterval(enabled ? 1 : 0) != 0) {
            Logger::Warn("Failed to set VSync: {}", SDL_GetError());
        }
        m_Data.VSync = enabled;
    }

    bool Window::IsVSync() const {
        return m_Data.VSync;
    }

    uint32_t Window::GetWidth() const {
        return m_Data.Width;
    }

    uint32_t Window::GetHeight() const {
        return m_Data.Height;
    }

    void* Window::GetNativeWindow() const {
        return m_Window;
    }

    void* Window::GetNativeContext() const {
        return m_Context;
    }

} // namespace Tesseract
