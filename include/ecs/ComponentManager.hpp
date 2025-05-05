#pragma once

#include "ecs/Core.hpp"
#include "ecs/ComponentArray.hpp"
#include <memory>
#include <unordered_map>

namespace Tesseract {
namespace ECS {

    class ComponentManager {
    public:
        template<typename T>
        void RegisterComponent() {
            const char* typeName = typeid(T).name();

            if (componentTypes.find(typeName) != componentTypes.end()) {
                throw std::runtime_error("Registering component type more than once.");
            }

            componentTypes.insert({typeName, GetComponentTypeID<T>()});
            componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
        }

        template<typename T>
        ComponentTypeID GetComponentType() {
            const char* typeName = typeid(T).name();

            if (componentTypes.find(typeName) == componentTypes.end()) {
                throw std::runtime_error("Component not registered before use.");
            }

            return componentTypes[typeName];
        }

        template<typename T>
        void AddComponent(EntityID entity, T&& component) {
            GetComponentArray<T>()->InsertData(entity, std::forward<T>(component));
        }

        template<typename T>
        void AddComponent(EntityID entity, const T& component) {
            GetComponentArray<T>()->InsertData(entity, component);
        }

        template<typename T>
        void RemoveComponent(EntityID entity) {
            GetComponentArray<T>()->RemoveData(entity);
        }

        template<typename T>
        T& GetComponent(EntityID entity) {
            return GetComponentArray<T>()->GetData(entity);
        }

        template<typename T>
        bool HasComponent(EntityID entity) const {
            const char* typeName = typeid(T).name();

            if (componentArrays.find(typeName) == componentArrays.end()) {
                return false;
            }

            auto componentArray = std::static_pointer_cast<ComponentArray<T>>(componentArrays.at(typeName));
            return componentArray->HasData(entity);
        }

        void EntityDestroyed(EntityID entity) {
            for (auto& pair : componentArrays) {
                auto& component = pair.second;
                component->EntityDestroyed(entity);
            }
        }

    private:
        // Map from type string pointer to a component type ID
        std::unordered_map<const char*, ComponentTypeID> componentTypes;

        // Map from type string pointer to a component array
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays;

        // Convenience method to get the component array of type T
        template<typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray() {
            const char* typeName = typeid(T).name();

            if (componentArrays.find(typeName) == componentArrays.end()) {
                throw std::runtime_error("Component not registered before use.");
            }

            return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
        }
    };

}} // namespace Tesseract::ECS
