#include "Tesseract/TesseractLayer.hpp"
#include "core/Input.hpp"
#include "core/Logger.hpp"
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_mouse.h>
#include <imgui.h>

#include "core/Event.hpp"
#include "core/Events/KeyEvent.hpp"
#include "core/Events/MouseEvent.hpp"
#include "core/Timestep.hpp"
#include "core/Renderer/RenderCommand.hpp"
#include "core/Renderer/Renderer2D.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Tesseract {

    TesseractLayer::TesseractLayer()
        : Layer("TesseractLayer"),
          // Initialiser la caméra avec un aspect ratio basé sur la fenêtre
          // et une taille verticale (ex: de -1.0 à 1.0)
          m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
          m_CameraPosition({0.0f, 0.0f, 0.0f}),
          m_SquareColor({0.2f, 0.3f, 0.8f, 1.0f}),
          m_DebugWindowSize(400, 300),
          m_StatsWindowSize(400, 200),
          m_ShowImGuiSizeConfig(true),
          m_BackgroundColor({0.1f, 0.1f, 0.2f, 1.0f}) // Bleu foncé par défaut
    {
        TS_INFO("TesseractLayer created!");
    }

    TesseractLayer::~TesseractLayer() {
        TS_INFO("TesseractLayer destroyed!");
    }

    void TesseractLayer::OnAttach() {
        Logger::Debug("TesseractLayer attached.");
    }

    void TesseractLayer::OnDetach() {
        Logger::Debug("TesseractLayer detached.");
    }

    void TesseractLayer::OnUpdate(Timestep ts) {
        // --- Mises à jour Logiques Uniquement ---

        // --- Contrôle Caméra ---
        glm::vec3 position = m_Camera.GetPosition(); // Lire la position
        float rotation = m_Camera.GetRotation(); // Lire la rotation
        float cameraMoveSpeed = 5.0f;
        float cameraRotationSpeed = 180.0f;

        if (Input::IsKeyPressed(SDL_SCANCODE_A))      position.x -= cameraMoveSpeed * ts;
        else if (Input::IsKeyPressed(SDL_SCANCODE_D)) position.x += cameraMoveSpeed * ts;
        if (Input::IsKeyPressed(SDL_SCANCODE_W))      position.y += cameraMoveSpeed * ts;
        else if (Input::IsKeyPressed(SDL_SCANCODE_S)) position.y -= cameraMoveSpeed * ts;
        if (Input::IsKeyPressed(SDL_SCANCODE_Q))      rotation += cameraRotationSpeed * ts;
        if (Input::IsKeyPressed(SDL_SCANCODE_E))      rotation -= cameraRotationSpeed * ts;

        m_Camera.SetPosition(position); // Mettre à jour la position
        m_Camera.SetRotation(rotation); // Mettre à jour la rotation
        // -----------------------

        // --- PAS DE CODE IMGUI ICI ---

        // Définir la couleur de fond avant le rendu
        RenderCommand::SetClearColor(m_BackgroundColor);
        RenderCommand::Clear();

        // --- Rendu 2D ---
        Renderer2D::BeginScene(m_Camera);
        Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.0f }, { 0.5f, 0.75f }, m_SquareColor);
        Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f}, {0.5f, 0.5f}, glm::radians(45.0f), {0.2f, 0.8f, 0.2f, 1.0f});
        Renderer2D::EndScene();
    }

    void TesseractLayer::OnImGuiRender() {
        // --- TOUT LE CODE IMGUI POUR CETTE COUCHE ICI ---

        // Fenêtre de configuration des tailles ImGui
        if (m_ShowImGuiSizeConfig) {
            ImGui::Begin("ImGui Size Configuration", &m_ShowImGuiSizeConfig);
            ImGui::Text("Ajustez la taille des fenêtres de débogage");

            ImGui::SliderFloat("Debug Window Width", &m_DebugWindowSize.x, 200.0f, 800.0f);
            ImGui::SliderFloat("Debug Window Height", &m_DebugWindowSize.y, 150.0f, 600.0f);

            ImGui::SliderFloat("Stats Window Width", &m_StatsWindowSize.x, 200.0f, 800.0f);
            ImGui::SliderFloat("Stats Window Height", &m_StatsWindowSize.y, 150.0f, 600.0f);

            ImGui::Separator();
            ImGui::Text("Couleur de fond");
            ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));

            if (ImGui::Button("Reset Settings")) {
                m_DebugWindowSize = ImVec2(400, 300);
                m_StatsWindowSize = ImVec2(400, 200);
                m_BackgroundColor = glm::vec4(0.1f, 0.1f, 0.2f, 1.0f);
            }

            ImGui::End();
        }

        // Fenêtre de debug avec taille personnalisée
        ImGui::SetNextWindowSize(m_DebugWindowSize, ImGuiCond_Always);
        ImGui::Begin("Tesseract Debug", nullptr, ImGuiWindowFlags_NoResize);
        auto [mouseX, mouseY] = Input::GetMousePosition();
        ImGui::Text("Position Souris: (%d, %d)", mouseX, mouseY);
        ImGui::Separator();
        ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);
        ImGui::Text("Camera Rotation: %.2f deg", m_Camera.GetRotation());
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
        if (ImGui::Button("Reset Camera")) {
            m_Camera.SetPosition({0.0f, 0.0f, 0.0f});
            m_Camera.SetRotation(0.0f);
        }

        // Bouton pour afficher/masquer la fenêtre de configuration
        if (ImGui::Button(m_ShowImGuiSizeConfig ? "Masquer Config" : "Afficher Config")) {
            m_ShowImGuiSizeConfig = !m_ShowImGuiSizeConfig;
        }

        ImGui::End();

        // Fenêtre stats avec taille personnalisée
        ImGui::SetNextWindowSize(m_StatsWindowSize, ImGuiCond_Always);
        ImGui::Begin("Tesseract Debug - Layer Stats", nullptr, ImGuiWindowFlags_NoResize);
        ImGui::Text("Layer specific debug info here.");
        // Afficher les stats du Renderer2D
        auto stats = Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        ImGui::End();
        Renderer2D::ResetStats(); // Remettre à zéro après affichage
    }

    void TesseractLayer::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>([](KeyPressedEvent& e) -> bool {
            if (e.GetKeyCode() == SDL_SCANCODE_ESCAPE) {
                Logger::Warn("Échap pressée ! (Événement géré par TesseractLayer)");
                return true;
            }
            return false;
        });

        dispatcher.Dispatch<MouseButtonPressedEvent>([](MouseButtonPressedEvent& e) -> bool {
            Logger::Debug("Clic souris détecté par TesseractLayer: Bouton {}", e.GetMouseButton());
            return false;
        });
    }

} // namespace Tesseract
