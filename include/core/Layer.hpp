#pragma once

#include <string>
#include "core/Core.hpp"
#include "core/Event.hpp"
#include "core/Timestep.hpp"

namespace Tesseract {
    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate([[maybe_unused]] Timestep ts) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent([[maybe_unused]] Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}
