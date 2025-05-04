#pragma once

#include "core/Layer.hpp"
#include "core/Window.hpp"
#include "core/Event.hpp"
#include "core/Timestep.hpp"

namespace Tesseract {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Timestep ts) override;
        virtual void OnEvent(Event& event) override;

        void Begin();
        void End();

    private:
        float m_Time = 0.0f;
    };
}
