#include "core/Application.hpp"
#include "core/Logger.hpp"
#include "core/Core.hpp"
#include "core/ImGuiLayer.hpp"

namespace Tesseract {
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name) {
        TS_ASSERT(!s_Instance);
        s_Instance = this;

        Logger::Info("Initializing Tesseract Engine...");
        m_Window = CreateScope<Window>(WindowProps(name));

        // Création et ajout de la couche ImGui
        PushOverlay(new ImGuiLayer());
    }

    Application::~Application() {
        Logger::Info("Shutting down Tesseract Engine...");
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.push_back(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay) {
        m_LayerStack.push_back(overlay);
        overlay->OnAttach();
    }

    void Application::Run() {
        Logger::Info("Starting main loop...");
        while (m_Running) {
            if (m_Window->ShouldClose()) {
                m_Running = false;
                continue;
            }

            // Mise à jour des layers
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_Window->OnUpdate();
        }
    }

    void Application::Close() {
        m_Running = false;
    }
}
