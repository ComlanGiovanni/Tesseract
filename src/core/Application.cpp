#include "core/Application.hpp"
#include "core/Logger.hpp"
#include "core/Core.hpp"
#include "core/ImGuiLayer.hpp"
#include "core/Timestep.hpp"
#include <SDL2/SDL.h>

namespace Tesseract {
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name) {
        TS_ASSERT(!s_Instance);
        s_Instance = this;

        Logger::Info("Initializing Tesseract Engine...");
        m_Window = CreateScope<Window>(WindowProps(name));

        // Création et ajout de la couche ImGui
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {
        Logger::Info("Shutting down Tesseract Engine...");
        for (Layer* layer : m_LayerStack) {
            layer->OnDetach();
            delete layer;
        }
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

        Uint64 lastFrameTime = SDL_GetPerformanceCounter();
        float time = 0.0f;

        while (m_Running) {
            Uint64 currentFrameTime = SDL_GetPerformanceCounter();
            time = (float)(currentFrameTime - lastFrameTime) / SDL_GetPerformanceFrequency();
            lastFrameTime = currentFrameTime;

            Timestep timestep(time);

            if (m_Window->ShouldClose()) {
                m_Running = false;
                continue;
            }

            m_Window->OnUpdate();
            m_ImGuiLayer->Begin();

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->End();
            m_Window->SwapBuffers();
        }
    }

    void Application::Close() {
        m_Running = false;
    }
}
