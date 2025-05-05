#pragma once

#include "ecs/System.hpp"
#include "ecs/Components.hpp"
#include "core/Timestep.hpp"
#include "core/Logger.hpp"

namespace Tesseract {
namespace ECS {

    // Forward declaration pour éviter la dépendance circulaire
    class Registry;

    class MovementSystem : public System {
    public:
        MovementSystem() = default;

        virtual void OnUpdate(Timestep ts) override {
            try {
                // Parcourir toutes les entités avec Transform et Velocity
                for (auto entity : GetEntities()) {
                    if (!m_Registry ||
                        !m_Registry->HasComponent<TransformComponent>(entity) ||
                        !m_Registry->HasComponent<VelocityComponent>(entity))
                        continue;

                    auto& transform = m_Registry->GetComponent<TransformComponent>(entity);
                    auto& velocity = m_Registry->GetComponent<VelocityComponent>(entity);

                    // Mettre à jour la position en fonction de la vitesse
                    transform.Position += velocity.Velocity * (float)ts;

                    // Option: Limites de l'écran avec rebond
                    // Si l'entité sort des limites, on inverse sa vitesse
                    if (transform.Position.x < -2.0f || transform.Position.x > 2.0f) {
                        velocity.Velocity.x = -velocity.Velocity.x;
                    }
                    if (transform.Position.y < -2.0f || transform.Position.y > 2.0f) {
                        velocity.Velocity.y = -velocity.Velocity.y;
                    }
                }
            } catch (const std::exception& e) {
                Logger::Error("Exception dans MovementSystem::OnUpdate: {}", e.what());
                // Ne pas relancer l'exception pour continuer l'exécution
            }
        }

        void SetRegistry(Registry* registry) {
            m_Registry = registry;
        }

    private:
        Registry* m_Registry = nullptr;
    };

}} // namespace Tesseract::ECS
