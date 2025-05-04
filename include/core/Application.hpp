#pragma once

#include <memory>
#include <string>
#include <vector>
#include "core/Window.hpp"
#include "core/Logger.hpp"
#include "core/Layer.hpp"
#include "core/ImGuiLayer.hpp"

namespace Tesseract {
    class Application {
    public:
        Application(const std::string& name = "Tesseract");
        virtual ~Application();

        void Run();
        void Close();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        static Application& Get() { return *s_Instance; }
        Window& GetWindow() { return *m_Window; }

    private:
        static Application* s_Instance;
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        std::vector<Layer*> m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
    };

    // Déclaration de la fonction à implémenter par l'application cliente
    Application* CreateApplication();
}
