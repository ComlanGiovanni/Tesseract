#pragma once

#include "ecs/Core.hpp"
#include "core/Logger.hpp"
#include <unordered_map>
#include <array>

namespace Tesseract {
namespace ECS {

    // Interface pour le stockage des composants
    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(EntityID entity) = 0;
    };

    // Implémentation typée du stockage de composants
    template<typename T>
    class ComponentArray : public IComponentArray {
    public:
        void InsertData(EntityID entity, T&& component) {
            if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
                Logger::Warn("Component added to same entity more than once.");
                return;
            }

            // Ajouter un nouveau composant à la fin du tableau
            size_t newIndex = size;
            entityToIndexMap[entity] = newIndex;
            indexToEntityMap[newIndex] = entity;
            componentArray[newIndex] = std::move(component);
            size++;
        }

        void InsertData(EntityID entity, const T& component) {
            if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
                Logger::Warn("Component added to same entity more than once.");
                return;
            }

            // Ajouter un nouveau composant à la fin du tableau
            size_t newIndex = size;
            entityToIndexMap[entity] = newIndex;
            indexToEntityMap[newIndex] = entity;
            componentArray[newIndex] = component;
            size++;
        }

        void RemoveData(EntityID entity) {
            if (entityToIndexMap.find(entity) == entityToIndexMap.end()) {
                Logger::Warn("Trying to remove non-existent component.");
                return;
            }

            // Copier le dernier élément à l'emplacement de l'élément supprimé
            size_t indexOfRemovedEntity = entityToIndexMap[entity];
            size_t indexOfLastElement = size - 1;
            componentArray[indexOfRemovedEntity] = std::move(componentArray[indexOfLastElement]);

            // Mettre à jour les maps
            EntityID entityOfLastElement = indexToEntityMap[indexOfLastElement];
            entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            entityToIndexMap.erase(entity);
            indexToEntityMap.erase(indexOfLastElement);

            size--;
        }

        T& GetData(EntityID entity) {
            if (entityToIndexMap.find(entity) == entityToIndexMap.end()) {
                throw std::runtime_error("Trying to get non-existent component.");
            }

            return componentArray[entityToIndexMap[entity]];
        }

        bool HasData(EntityID entity) const {
            return entityToIndexMap.find(entity) != entityToIndexMap.end();
        }

        void EntityDestroyed(EntityID entity) override {
            if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
                RemoveData(entity);
            }
        }

    private:
        std::array<T, MAX_ENTITIES> componentArray;
        std::unordered_map<EntityID, size_t> entityToIndexMap;
        std::unordered_map<size_t, EntityID> indexToEntityMap;
        size_t size = 0;
    };

}} // namespace Tesseract::ECS
