#include "core/LayerStack.hpp"
#include "core/Layer.hpp"
#include <algorithm> // Pour std::find

namespace Tesseract {

    LayerStack::~LayerStack() {
        // Les Refs sont gérées automatiquement, mais détachons proprement
        for (Ref<Layer>& layer : m_Layers) {
            layer->OnDetach();
        }
    }

    void LayerStack::PushLayer(Ref<Layer> layer) {
        // Insère la nouvelle couche avant l'itérateur d'insertion (qui pointe vers le début des overlays)
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
        // OnAttach est appelé par Application après PushLayer
    }

    void LayerStack::PushOverlay(Ref<Layer> overlay) {
        // Ajoute l'overlay à la fin du vecteur
        m_Layers.emplace_back(overlay);
        // OnAttach est appelé par Application après PushOverlay
    }

    void LayerStack::PopLayer(Ref<Layer> layer) {
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if (it != m_Layers.begin() + m_LayerInsertIndex) {
            layer->OnDetach();
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(Ref<Layer> overlay) {
        auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end()) {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    }

} // namespace Tesseract
