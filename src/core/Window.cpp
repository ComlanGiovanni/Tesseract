#include "core/Window.hpp"
#include "core/Logger.hpp"
#include <GL/glew.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>

namespace Tesseract {
    Window::Window(const WindowProps& props) : m_Data(props) {
        Logger::Info("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0) {
            Logger::Fatal("Could not initialize SDL: {0}", SDL_GetError());
            return;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        m_Window = SDL_CreateWindow(
            props.Title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            props.Width,
            props.Height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

        if (!m_Window) {
            Logger::Fatal("Could not create window: {0}", SDL_GetError());
            return;
        }

        m_Context = SDL_GL_CreateContext(m_Window);
        if (!m_Context) {
            Logger::Fatal("Could not create OpenGL context: {0}", SDL_GetError());
            return;
        }

        SDL_GL_MakeCurrent(m_Window, m_Context);
        SetVSync(true);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            Logger::Fatal("Could not initialize GLEW");
            return;
        }

        // Configuration OpenGL de base
        glViewport(0, 0, props.Width, props.Height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        Logger::Info("OpenGL Info:");
        Logger::Info("  Vendor: {}", std::string((const char*)glGetString(GL_VENDOR)));
        Logger::Info("  Renderer: {}", std::string((const char*)glGetString(GL_RENDERER)));
        Logger::Info("  Version: {}", std::string((const char*)glGetString(GL_VERSION)));
    }

    Window::~Window() {
        SDL_GL_DeleteContext(m_Context);
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }

    void Window::OnUpdate() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            switch (event.type) {
                case SDL_QUIT:
                    m_Data.WindowShouldClose = true;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        m_Data.Width = event.window.data1;
                        m_Data.Height = event.window.data2;
                        glViewport(0, 0, m_Data.Width, m_Data.Height);
                    }
                    break;
            }
        }

        // Nettoyage du buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::SwapBuffers() {
        SDL_GL_SwapWindow(m_Window);
    }

    void Window::SetVSync(bool enabled) {
        SDL_GL_SetSwapInterval(enabled ? 1 : 0);
        m_VSync = enabled;
    }

    bool Window::ShouldClose() const {
        return m_Data.WindowShouldClose;
    }
}
