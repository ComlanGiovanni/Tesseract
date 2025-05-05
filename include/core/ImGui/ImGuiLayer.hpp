#pragma once

#include "core/Layer.hpp"
#include "core/Event.hpp"

namespace Tesseract {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        // OnUpdate n'est pas nécessaire car le rendu ImGui est géré par Begin/End
        void OnEvent(Event& event) override;
        void OnImGuiRender() override;

        // Méthodes spécifiques à ImGui
        void Begin(); // Commence une frame ImGui
        void End();   // Termine une frame ImGui et la rend

    private:
        float m_Time = 0.0f; // Peut être utile pour certaines démos ImGui
    };

} // namespace Tesseract
