#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <memory>

namespace Tesseract {
    struct WindowProps {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        bool WindowShouldClose;

        WindowProps(const std::string& title = "Tesseract",
                   uint32_t width = 1280,
                   uint32_t height = 720)
            : Title(title), Width(width), Height(height), WindowShouldClose(false) {}
    };

    class Window {
    public:
        Window(const WindowProps& props = WindowProps());
        ~Window();

        void OnUpdate();
        void SetVSync(bool enabled);
        bool IsVSync() const { return m_VSync; }
        bool ShouldClose() const;

        uint32_t GetWidth() const { return m_Data.Width; }
        uint32_t GetHeight() const { return m_Data.Height; }

        SDL_Window* GetNativeWindow() const { return m_Window; }
        SDL_GLContext GetGLContext() const { return m_Context; }

    private:
        SDL_Window* m_Window;
        SDL_GLContext m_Context;
        WindowProps m_Data;
        bool m_VSync = true;
    };
}
