#include "core/Application.hpp"
#include "core/Logger.hpp"
#include "core/Core.hpp"
#include "core/ImGui/ImGuiLayer.hpp"
#include "core/Timestep.hpp"
#include "core/Renderer/RenderCommand.hpp"
#include "core/Renderer/Renderer2D.hpp"
#include "core/Input.hpp"
#include <SDL2/SDL.h>
#include <functional> // Pour std::bind et placeholders
#include <memory> // Pour std::shared_ptr

namespace Tesseract {
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name) {
        TS_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        Logger::Info("Creating Application: {}", name);

        m_Window = CreateScope<Window>(WindowProps(name));
        // Lier Application::OnEvent au callback de la fenêtre
        m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

        // Initialisation des systèmes de rendu APRES la création de la fenêtre/contexte
        RenderCommand::Init(); // Initialise l'API de rendu (OpenGL)
        Renderer2D::Init();    // Initialise le renderer 2D

        // Création et ajout de la couche ImGui
        m_ImGuiLayer = CreateRef<ImGuiLayer>();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {
        Logger::Warn("Destroying Application...");
        // Assurez-vous que Renderer2D est arrêté avant que le contexte OpenGL ne soit détruit
        Renderer2D::Shutdown();
        // Les smart pointers (m_Window, m_ImGuiLayer, m_LayerStack) gèrent leur propre nettoyage.
    }

    void Application::PushLayer(Ref<Layer> layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Ref<Layer> overlay) {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::Run() {
        Logger::Info("Starting Application Run Loop...");

        while (m_Running) {
            // Calcul du Timestep
            float time = (float)SDL_GetTicks() / 1000.0f; // Temps en secondes
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            // Ne pas rendre si la fenêtre est minimisée
            if (!m_Minimized) {
                // 1. Nettoyage de l'écran
                // TODO: Configurer la couleur via une scène ou settings
                RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
                RenderCommand::Clear();

                // 2. Mise à jour des couches (Logique du jeu/moteur)
                //    Renderer2D::BeginScene() et EndScene() sont appelés DANS les couches
                //    qui effectuent le rendu 2D (comme TesseractLayer)
                for (auto& layer : m_LayerStack)
                    layer->OnUpdate(timestep);

                // 3. Rendu ImGui
                m_ImGuiLayer->Begin(); // Démarre la frame ImGui
                for (auto& layer : m_LayerStack)
                    layer->OnImGuiRender(); // Permet aux couches de dessiner des UI ImGui
                m_ImGuiLayer->End();   // Termine la frame ImGui et prépare le dessin
            }

            // 4. Mise à jour de la fenêtre (Polling events, Swap Buffers)
            m_Window->OnUpdate(); // Gère les events SDL et le swap buffers
        }

        Logger::Info("Exiting Application Run Loop.");
    }

    void Application::Close() {
        m_Running = false;
    }

    // --- Implémentation de Application::OnEvent ---
    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        // Gère la fermeture de la fenêtre directement dans l'Application
        dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

        // Gère le redimensionnement de la fenêtre
        dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

        // Propage l'événement aux couches, du haut vers le bas (overlays d'abord)
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (e.Handled)
                break;
            (*it)->OnEvent(e);
        }
    }

    bool Application::OnWindowClose([[maybe_unused]] WindowCloseEvent& e) {
        m_Running = false;
        return true; // L'événement est géré
    }

    bool Application::OnWindowResize(WindowResizeEvent& e) {
        if (e.GetWidth() == 0 || e.GetHeight() == 0) {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;
        RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
        return false; // Laisser d'autres couches réagir
    }
}
