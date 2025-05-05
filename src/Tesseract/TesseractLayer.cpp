#include "Tesseract/TesseractLayer.hpp"
#include "core/Input.hpp"
#include "core/Logger.hpp"
#include "core/Asset/AssetManager.hpp"
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
          m_ShowECSDebug(true),
          m_BackgroundColor({0.1f, 0.1f, 0.2f, 1.0f}) // Bleu foncé par défaut
    {
        TS_INFO("TesseractLayer created!");
        // Le système de rendu sera configuré dans OnAttach
    }

    TesseractLayer::~TesseractLayer() {
        TS_INFO("TesseractLayer destroyed!");
    }

    void TesseractLayer::OnAttach() {
        Logger::Debug("TesseractLayer attached.");

        try {
            // Enregistrer les composants nécessaires
            m_Registry.RegisterComponent<ECS::TransformComponent>();
            m_Registry.RegisterComponent<ECS::SpriteComponent>();
            m_Registry.RegisterComponent<ECS::TagComponent>();
            m_Registry.RegisterComponent<ECS::VelocityComponent>();

            // Configurer le système de rendu
            m_RenderSystem = m_Registry.RegisterSystem<ECS::RenderSystem>();

            // Déclarer quels composants sont nécessaires pour le système de rendu
            ECS::ComponentSignature renderSignature;
            renderSignature.set(m_Registry.GetComponentType<ECS::TransformComponent>(), true);
            renderSignature.set(m_Registry.GetComponentType<ECS::SpriteComponent>(), true);
            m_Registry.SetSystemSignature<ECS::RenderSystem>(renderSignature);

            // Configurer le système de rendu
            m_RenderSystem->SetRegistry(&m_Registry);
            m_RenderSystem->SetCamera(m_Camera);

            // Configurer le système de mouvement
            m_MovementSystem = m_Registry.RegisterSystem<ECS::MovementSystem>();

            // Déclarer quels composants sont nécessaires pour le système de mouvement
            ECS::ComponentSignature movementSignature;
            movementSignature.set(m_Registry.GetComponentType<ECS::TransformComponent>(), true);
            movementSignature.set(m_Registry.GetComponentType<ECS::VelocityComponent>(), true);
            m_Registry.SetSystemSignature<ECS::MovementSystem>(movementSignature);

            // Configurer le système de mouvement
            m_MovementSystem->SetRegistry(&m_Registry);

            // Créer les entités de démo
            CreateDemoEntities();

            // Créer quelques entités en mouvement
            CreateMovingEntity({-1.5f, -0.5f, 0.0f}, {0.5f, 0.3f, 0.0f}, {1.0f, 0.5f, 0.0f, 1.0f});
            CreateMovingEntity({1.5f, 0.5f, 0.0f}, {-0.4f, 0.2f, 0.0f}, {0.0f, 0.7f, 1.0f, 1.0f});
        } catch (const std::exception& e) {
            Logger::Error("Exception dans OnAttach: {}", e.what());
            throw; // Relancer l'exception pour le débogage
        }
    }

    void TesseractLayer::OnDetach() {
        Logger::Debug("TesseractLayer detached.");
    }

    void TesseractLayer::CreateDemoEntities() {
        try {
            // 1. Un carré rouge à gauche
            auto redSquare = m_Registry.CreateEntity();
            m_Registry.AddComponent<ECS::TagComponent>(redSquare, ECS::TagComponent("RedSquare"));
            m_Registry.AddComponent<ECS::TransformComponent>(redSquare, ECS::TransformComponent());
            m_Registry.AddComponent<ECS::SpriteComponent>(redSquare, ECS::SpriteComponent());

            auto& redTransform = m_Registry.GetComponent<ECS::TransformComponent>(redSquare);
            redTransform.Position = {-1.0f, 0.0f, 0.0f};
            redTransform.Scale = {0.8f, 0.8f, 1.0f};

            auto& redSprite = m_Registry.GetComponent<ECS::SpriteComponent>(redSquare);
            redSprite.Color = {0.8f, 0.2f, 0.3f, 1.0f};

            // 2. Un carré bleu à droite
            auto blueSquare = m_Registry.CreateEntity();
            m_Registry.AddComponent<ECS::TagComponent>(blueSquare, ECS::TagComponent("BlueSquare"));
            m_Registry.AddComponent<ECS::TransformComponent>(blueSquare, ECS::TransformComponent());
            m_Registry.AddComponent<ECS::SpriteComponent>(blueSquare, ECS::SpriteComponent());

            auto& blueTransform = m_Registry.GetComponent<ECS::TransformComponent>(blueSquare);
            blueTransform.Position = {0.5f, -0.5f, 0.0f};
            blueTransform.Scale = {0.5f, 0.75f, 1.0f};

            auto& blueSprite = m_Registry.GetComponent<ECS::SpriteComponent>(blueSquare);
            blueSprite.Color = m_SquareColor;

            // 3. Un carré vert pivoté au centre
            auto greenSquare = m_Registry.CreateEntity();
            m_Registry.AddComponent<ECS::TagComponent>(greenSquare, ECS::TagComponent("GreenSquare"));
            m_Registry.AddComponent<ECS::TransformComponent>(greenSquare, ECS::TransformComponent());
            m_Registry.AddComponent<ECS::SpriteComponent>(greenSquare, ECS::SpriteComponent());

            auto& greenTransform = m_Registry.GetComponent<ECS::TransformComponent>(greenSquare);
            greenTransform.Position = {0.0f, 0.0f, -0.1f};
            greenTransform.Scale = {0.5f, 0.5f, 1.0f};
            greenTransform.Rotation = {0.0f, 0.0f, glm::radians(45.0f)};

            auto& greenSprite = m_Registry.GetComponent<ECS::SpriteComponent>(greenSquare);
            greenSprite.Color = {0.2f, 0.8f, 0.2f, 1.0f};

            Logger::Debug("Entités de démo créées avec succès");
        } catch (const std::exception& e) {
            Logger::Error("Exception dans CreateDemoEntities: {}", e.what());
        }
    }

    void TesseractLayer::CreateMovingEntity(const glm::vec3& position, const glm::vec3& velocity, const glm::vec4& color) {
        try {
            // Créer une entité avec une position, une vitesse et une couleur
            auto entity = m_Registry.CreateEntity();

            // Ajouter tous les composants nécessaires un par un
            m_Registry.AddComponent<ECS::TagComponent>(entity, ECS::TagComponent("MovingEntity"));
            m_Registry.AddComponent<ECS::TransformComponent>(entity, ECS::TransformComponent());
            m_Registry.AddComponent<ECS::SpriteComponent>(entity, ECS::SpriteComponent());
            m_Registry.AddComponent<ECS::VelocityComponent>(entity, ECS::VelocityComponent());

            // Configurer les composants
            auto& transform = m_Registry.GetComponent<ECS::TransformComponent>(entity);
            transform.Position = position;
            transform.Scale = {0.2f, 0.2f, 1.0f};

            auto& sprite = m_Registry.GetComponent<ECS::SpriteComponent>(entity);
            sprite.Color = color;

            auto& velocity_comp = m_Registry.GetComponent<ECS::VelocityComponent>(entity);
            velocity_comp.Velocity = velocity;

            Logger::Debug("Entité en mouvement créée avec succès: {}", entity);
        } catch (const std::exception& e) {
            Logger::Error("Exception dans CreateMovingEntity: {}", e.what());
        }
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

        // Mettre à jour la caméra du système de rendu ECS
        m_RenderSystem->SetCamera(m_Camera);

        // --- Mise à jour des systèmes ECS ---
        m_MovementSystem->OnUpdate(ts);

        // --- PAS DE CODE IMGUI ICI ---

        // Définir la couleur de fond avant le rendu
        RenderCommand::SetClearColor(m_BackgroundColor);
        RenderCommand::Clear();

        // --- Utiliser notre système de rendu directement ---
        m_RenderSystem->OnUpdate(ts);

        // Alternative : continuer à utiliser l'approche directe pour comparaison
        if (false) { // Désactivé pour l'instant
            // --- Rendu 2D direct (ancienne méthode) ---
            Renderer2D::BeginScene(m_Camera);
            Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
            Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.0f }, { 0.5f, 0.75f }, m_SquareColor);
            Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f}, {0.5f, 0.5f}, glm::radians(45.0f), {0.2f, 0.8f, 0.2f, 1.0f});
            Renderer2D::EndScene();
        }
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

            // Utiliser glm::value_ptr pour accéder directement aux composantes de la couleur
            ImGui::ColorPicker4("Background Color", glm::value_ptr(m_BackgroundColor),
                ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex |
                ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_AlphaBar);

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

        // Convertir glm::vec4 en ImVec4 pour le ColorButton
        ImVec4 squareColorImGui(m_SquareColor.r, m_SquareColor.g, m_SquareColor.b, m_SquareColor.a);
        if (ImGui::ColorButton("Square Color Button", squareColorImGui, ImGuiColorEditFlags_AlphaPreview, ImVec2(50, 20)))
            ImGui::OpenPopup("square_color_picker");

        ImGui::SameLine();
        ImGui::Text("Square Color");

        // Popup pour le sélecteur de couleur avancé
        if (ImGui::BeginPopup("square_color_picker")) {
            ImGui::Text("Couleur du carré");
            ImGui::Separator();
            ImGui::ColorPicker4("##picker", glm::value_ptr(m_SquareColor),
                ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex |
                ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_AlphaBar);
            ImGui::EndPopup();
        }

        if (ImGui::Button("Reset Camera")) {
            m_Camera.SetPosition({0.0f, 0.0f, 0.0f});
            m_Camera.SetRotation(0.0f);
        }

        // Boutons pour afficher/masquer les fenêtres de configuration
        if (ImGui::Button(m_ShowImGuiSizeConfig ? "Masquer Config" : "Afficher Config")) {
            m_ShowImGuiSizeConfig = !m_ShowImGuiSizeConfig;
        }

        ImGui::SameLine();

        if (ImGui::Button(m_ShowECSDebug ? "Masquer ECS Debug" : "Afficher ECS Debug")) {
            m_ShowECSDebug = !m_ShowECSDebug;
        }

        ImGui::End();

        // Fenêtre de débogage ECS
        if (m_ShowECSDebug) {
            ImGui::Begin("ECS Debug", &m_ShowECSDebug);

            if (ImGui::TreeNode("Entities")) {
                // Pour l'instant, montrer uniquement les entités avec un TagComponent
                // Dans une version plus complète, nous pourrions parcourir toutes les entités

                // Simulez l'affichage des entités ici
                ImGui::BulletText("RedSquare");
                ImGui::BulletText("BlueSquare");
                ImGui::BulletText("GreenSquare");

                ImGui::TreePop();
            }

            ImGui::Separator();

            if (ImGui::Button("Create Random Entity")) {
                try {
                    // Créer une entité avec une position et une couleur aléatoires
                    auto entity = m_Registry.CreateEntity();

                    // Ajouter les composants
                    m_Registry.AddComponent<ECS::TagComponent>(entity, ECS::TagComponent("RandomEntity"));
                    m_Registry.AddComponent<ECS::TransformComponent>(entity, ECS::TransformComponent());
                    m_Registry.AddComponent<ECS::SpriteComponent>(entity, ECS::SpriteComponent());

                    // Position aléatoire entre -2 et 2
                    float x = ((float)rand() / RAND_MAX) * 4.0f - 2.0f;
                    float y = ((float)rand() / RAND_MAX) * 4.0f - 2.0f;

                    // Configurer la transformation
                    auto& transform = m_Registry.GetComponent<ECS::TransformComponent>(entity);
                    transform.Position = {x, y, 0.0f};

                    // Taille aléatoire entre 0.3 et 0.7
                    float size = ((float)rand() / RAND_MAX) * 0.4f + 0.3f;
                    transform.Scale = {size, size, 1.0f};

                    // Couleur aléatoire
                    float r = (float)rand() / RAND_MAX;
                    float g = (float)rand() / RAND_MAX;
                    float b = (float)rand() / RAND_MAX;

                    // Configurer le sprite
                    auto& sprite = m_Registry.GetComponent<ECS::SpriteComponent>(entity);
                    sprite.Color = {r, g, b, 1.0f};

                    Logger::Debug("Entité aléatoire créée avec succès: {}", entity);
                } catch (const std::exception& e) {
                    Logger::Error("Exception dans Create Random Entity: {}", e.what());
                }
            }

            if (ImGui::Button("Create Moving Entity")) {
                try {
                    // Position aléatoire entre -1.5 et 1.5
                    float x = ((float)rand() / RAND_MAX) * 3.0f - 1.5f;
                    float y = ((float)rand() / RAND_MAX) * 3.0f - 1.5f;

                    // Vitesse aléatoire entre -0.5 et 0.5
                    float vx = ((float)rand() / RAND_MAX) * 1.0f - 0.5f;
                    float vy = ((float)rand() / RAND_MAX) * 1.0f - 0.5f;

                    // Couleur aléatoire
                    float r = (float)rand() / RAND_MAX;
                    float g = (float)rand() / RAND_MAX;
                    float b = (float)rand() / RAND_MAX;

                    // Créer l'entité en mouvement
                    CreateMovingEntity({x, y, 0.0f}, {vx, vy, 0.0f}, {r, g, b, 1.0f});

                    Logger::Debug("Demande de création d'entité en mouvement");
                } catch (const std::exception& e) {
                    Logger::Error("Exception dans Create Moving Entity Button: {}", e.what());
                }
            }

            ImGui::End();
        }

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
