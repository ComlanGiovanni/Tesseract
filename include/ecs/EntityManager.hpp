#pragma once

#include "ecs/Core.hpp"
#include <array>
#include <queue>

namespace Tesseract {
namespace ECS {

    class EntityManager {
    public:
        EntityManager() {
            // Initialiser les entités disponibles
            for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity) {
                availableEntities.push(entity);
            }
        }

        EntityID CreateEntity() {
            if (livingEntityCount >= MAX_ENTITIES) {
                throw std::runtime_error("Too many entities in existence.");
            }

            // Prendre un ID dans la file
            EntityID id = availableEntities.front();
            availableEntities.pop();
            ++livingEntityCount;

            return id;
        }

        void DestroyEntity(EntityID entity) {
            if (entity >= MAX_ENTITIES) {
                throw std::runtime_error("Entity out of range.");
            }

            // Réinitialiser la signature et rendre l'ID disponible à nouveau
            signatures[entity].reset();
            availableEntities.push(entity);
            --livingEntityCount;
        }

        void SetSignature(EntityID entity, ComponentSignature signature) {
            if (entity >= MAX_ENTITIES) {
                throw std::runtime_error("Entity out of range.");
            }

            signatures[entity] = signature;
        }

        ComponentSignature GetSignature(EntityID entity) const {
            if (entity >= MAX_ENTITIES) {
                throw std::runtime_error("Entity out of range.");
            }

            return signatures[entity];
        }

    private:
        // Queue of unused entity IDs
        std::queue<EntityID> availableEntities;

        // Array of signatures where the index corresponds to the entity ID
        std::array<ComponentSignature, MAX_ENTITIES> signatures;

        // Total number of living entities
        uint32_t livingEntityCount = 0;
    };

}} // namespace Tesseract::ECS
