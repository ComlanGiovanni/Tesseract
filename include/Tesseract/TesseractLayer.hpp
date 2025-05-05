#pragma once

#include "core/Layer.hpp"
#include "core/Event.hpp"
#include "core/Renderer/OrthographicCamera.hpp"
#include "core/Timestep.hpp"
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
        OrthographicCamera m_Camera;
        glm::vec3 m_CameraPosition;
        glm::vec4 m_SquareColor;

        // Variables pour les tailles des fenêtres ImGui
        ImVec2 m_DebugWindowSize;
        ImVec2 m_StatsWindowSize;
        bool m_ShowImGuiSizeConfig;

        // Couleur de fond d'écran
        glm::vec4 m_BackgroundColor;
    };

} // namespace Tesseract
