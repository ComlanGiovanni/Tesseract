#pragma once

#include "core/Core.hpp"
#include "core/Window.hpp"
#include "core/LayerStack.hpp"
#include "core/Event.hpp"
#include "core/Events/ApplicationEvent.hpp"
#include "core/ImGui/ImGuiLayer.hpp"

#include <string>
#include <memory>

namespace Tesseract {

    class Application {
    public:
        Application(const std::string& name = "Tesseract App");
        virtual ~Application();

        void Run();
        void Close();

        void OnEvent(Event& e);

        void PushLayer(Ref<Layer> layer);
        void PushOverlay(Ref<Layer> overlay);

        inline Window& GetWindow() { return *m_Window; }
        inline static Application& Get() { return *s_Instance; }

    private:
        // Méthodes de gestion d'événements internes
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        Scope<Window> m_Window;
        Ref<ImGuiLayer> m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

        static Application* s_Instance;
    };

    Application* CreateApplication();

}
