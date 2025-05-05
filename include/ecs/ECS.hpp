#pragma once

// Fichier d'inclusion principal pour le système ECS

#include "ecs/Core.hpp"
#include "ecs/Entity.hpp"
#include "ecs/Registry.hpp"
#include "ecs/Components.hpp"
#include "ecs/Systems/RenderSystem.hpp"

// Autres inclusions futures à ajouter au besoin

namespace Tesseract {
namespace ECS {

    // Alias pratiques
    using EntityHandle = Entity;

    // Fonctions utilitaires ECS
    inline Entity CreateEntity(Registry& registry, const std::string& name = "") {
        Entity entity(registry.CreateEntity(), &registry);
        entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
        entity.AddComponent<TransformComponent>();
        return entity;
    }

}} // namespace Tesseract::ECS
