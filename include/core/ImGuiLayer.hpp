#pragma once

#include "core/Layer.hpp"
#include "core/Window.hpp"

namespace Tesseract {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event& event) override;

        void Begin();
        void End();

    private:
        float m_Time = 0.0f;
    };
}
