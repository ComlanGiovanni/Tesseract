#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <functional>
#include "core/Renderer/Texture.hpp"
#include "core/Timestep.hpp"

namespace Tesseract {
namespace ECS {

    // --- Composant Transform ---
    struct TransformComponent {
        glm::vec3 Position = {0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation = {0.0f, 0.0f, 0.0f}; // En radians
        glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

        TransformComponent() = default;
        TransformComponent(const glm::vec3& position) : Position(position) {}
        TransformComponent(const glm::vec3& position, const glm::vec3& rotation)
            : Position(position), Rotation(rotation) {}
        TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
            : Position(position), Rotation(rotation), Scale(scale) {}

        // Matrice de modèle pour le rendu
        glm::mat4 GetTransform() const {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.z, {0, 0, 1});
            return glm::translate(glm::mat4(1.0f), Position)
                 * rotation
                 * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    // --- Composant Sprite ---
    struct SpriteComponent {
        glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
        Ref<Texture2D> Texture = nullptr;
        float TilingFactor = 1.0f;

        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color) : Color(color) {}
        SpriteComponent(const Ref<Texture2D>& texture) : Texture(texture) {}
        SpriteComponent(const glm::vec4& color, const Ref<Texture2D>& texture)
            : Color(color), Texture(texture) {}
    };

    // --- Composant Tag ---
    struct TagComponent {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const std::string& tag) : Tag(tag) {}
    };

    // --- Composant Camera (optionnel, utilisé pour définir la vue) ---
    struct CameraComponent {
        bool Primary = true; // Est-ce la caméra principale ?
        bool FixedAspectRatio = false;
        float AspectRatio = 0.0f;
        float Zoom = 1.0f;

        CameraComponent() = default;
        CameraComponent(bool primary)
            : Primary(primary) {}
    };

    // --- Composant de script (pour logique personnalisée) ---
    struct NativeScriptComponent {
        std::function<void()> OnCreate;
        std::function<void()> OnDestroy;
        std::function<void(Timestep)> OnUpdate;

        NativeScriptComponent() = default;
    };

    // Nouveau composant pour la vitesse
    struct VelocityComponent {
        glm::vec3 Velocity = {0.0f, 0.0f, 0.0f};

        VelocityComponent() = default;
        VelocityComponent(const glm::vec3& velocity)
            : Velocity(velocity) {}
    };

}} // namespace Tesseract::ECS
