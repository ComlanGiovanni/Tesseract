#pragma once

#include "core/Layer.hpp"
#include "core/Event.hpp"
#include "core/Renderer/OrthographicCamera.hpp"
#include "core/Timestep.hpp"
#include "ecs/ECS.hpp"
#include "ecs/Systems/RenderSystem.hpp"
#include "ecs/Systems/MovementSystem.hpp"
#include <imgui.h>
#include <glm/glm.hpp>

namespace Tesseract {

    // Renommé de SandboxLayer
    class TesseractLayer : public Layer {
    public:
        TesseractLayer();
        virtual ~TesseractLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& event) override;

    private:
        // Créer quelques entités de démo
        void CreateDemoEntities();
        // Créer une entité en mouvement
        void CreateMovingEntity(const glm::vec3& position, const glm::vec3& velocity, const glm::vec4& color);

        // Système ECS
        ECS::Registry m_Registry;
        std::shared_ptr<ECS::RenderSystem> m_RenderSystem;
        std::shared_ptr<ECS::MovementSystem> m_MovementSystem;

        // Anciennes variables
        OrthographicCamera m_Camera;
        glm::vec3 m_CameraPosition;
        glm::vec4 m_SquareColor;

        // Variables pour les tailles des fenêtres ImGui
        ImVec2 m_DebugWindowSize;
        ImVec2 m_StatsWindowSize;
        bool m_ShowImGuiSizeConfig;

        // Variables de debug
        bool m_ShowECSDebug;

        // Couleur de fond d'écran
        glm::vec4 m_BackgroundColor;
    };

} // namespace Tesseract
