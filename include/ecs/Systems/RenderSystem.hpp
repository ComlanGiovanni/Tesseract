#pragma once

#include "ecs/System.hpp"
#include "ecs/Components.hpp"
#include "core/Renderer/Renderer2D.hpp"
#include "core/Renderer/OrthographicCamera.hpp"
#include "core/Logger.hpp"

namespace Tesseract {
namespace ECS {

    // Forward declaration pour éviter la dépendance circulaire
    class Registry;

    class RenderSystem : public System {
    public:
        // Constructeur par défaut avec caméra initialisée par défaut
        RenderSystem() : m_Camera(-1.0f, 1.0f, -1.0f, 1.0f) {}

        virtual void OnUpdate([[maybe_unused]] Timestep ts) override {
            try {
                // Trouver la caméra principale si définie
                OrthographicCamera* mainCamera = nullptr;
                TransformComponent* cameraTransform = nullptr;

                // Parcourir toutes les entités avec une caméra
                for (auto entity : GetEntities()) {
                    if (m_Registry && m_Registry->HasComponent<CameraComponent>(entity)) {
                        auto& camera = m_Registry->GetComponent<CameraComponent>(entity);
                        if (camera.Primary) {
                            mainCamera = &m_Camera; // Utiliser la caméra par défaut du système pour l'instant
                            if (m_Registry->HasComponent<TransformComponent>(entity)) {
                                cameraTransform = &m_Registry->GetComponent<TransformComponent>(entity);
                            }
                            break;
                        }
                    }
                }

                // Si aucune caméra principale n'est trouvée, utiliser la caméra par défaut
                if (!mainCamera) {
                    mainCamera = &m_Camera;
                }

                // Mettre à jour la caméra si elle a un transform
                if (cameraTransform) {
                    m_Camera.SetPosition(cameraTransform->Position);
                    m_Camera.SetRotation(cameraTransform->Rotation.z);
                }

                // Commencer le rendu de la scène
                Renderer2D::BeginScene(*mainCamera);

                // Rendre toutes les entités avec Transform et Sprite
                for (auto entity : GetEntities()) {
                    if (!m_Registry ||
                        !m_Registry->HasComponent<TransformComponent>(entity) ||
                        !m_Registry->HasComponent<SpriteComponent>(entity))
                        continue;

                    auto& transform = m_Registry->GetComponent<TransformComponent>(entity);
                    auto& sprite = m_Registry->GetComponent<SpriteComponent>(entity);

                    // Si l'entité a une texture
                    if (sprite.Texture) {
                        // TODO: Implémenter le rendu de textures quand Renderer2D le supportera
                        // Pour l'instant, on utilise juste la couleur
                        Renderer2D::DrawQuad(transform.Position, transform.Scale, sprite.Color);
                    } else {
                        // Dessiner un quad coloré
                        if (transform.Rotation.z != 0.0f) {
                            Renderer2D::DrawRotatedQuad(transform.Position, transform.Scale,
                                                    transform.Rotation.z, sprite.Color);
                        } else {
                            Renderer2D::DrawQuad(transform.Position, transform.Scale, sprite.Color);
                        }
                    }
                }

                // Terminer le rendu de la scène
                Renderer2D::EndScene();
            } catch (const std::exception& e) {
                Logger::Error("Exception dans RenderSystem::OnUpdate: {}", e.what());
                // Ne pas relancer l'exception pour continuer l'exécution
            }
        }

        void SetRegistry(Registry* registry) {
            m_Registry = registry;
        }

        void SetCamera(const OrthographicCamera& camera) {
            m_Camera = camera;
        }

    private:
        Registry* m_Registry = nullptr;
        OrthographicCamera m_Camera;
    };

}} // namespace Tesseract::ECS
