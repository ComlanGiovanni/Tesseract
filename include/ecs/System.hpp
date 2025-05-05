#pragma once

#include "ecs/Core.hpp"
#include <set>
#include <memory>
#include <unordered_map>
#include "core/Timestep.hpp"

namespace Tesseract {
namespace ECS {

    // Interface de base pour tous les systèmes
    class System {
    public:
        virtual ~System() = default;

        // Obtenir l'ensemble des entités gérées par ce système
        std::set<EntityID>& GetEntities() { return entities; }
        const std::set<EntityID>& GetEntities() const { return entities; }

        // Méthodes virtuelles que les systèmes dérivés peuvent surcharger
        virtual void OnEntityAdded([[maybe_unused]] EntityID entity) {}
        virtual void OnEntityRemoved([[maybe_unused]] EntityID entity) {}
        virtual void OnUpdate([[maybe_unused]] Timestep ts) {}

    protected:
        std::set<EntityID> entities;
    };

    class SystemManager {
    public:
        template<typename T>
        std::shared_ptr<T> RegisterSystem() {
            const char* typeName = typeid(T).name();

            if (systems.find(typeName) != systems.end()) {
                throw std::runtime_error("Registering system more than once.");
            }

            auto system = std::make_shared<T>();
            systems.insert({typeName, system});
            return system;
        }

        template<typename T>
        void SetSignature(ComponentSignature signature) {
            const char* typeName = typeid(T).name();

            if (systems.find(typeName) == systems.end()) {
                throw std::runtime_error("System used before registered.");
            }

            signatures.insert({typeName, signature});
        }

        void EntityDestroyed(EntityID entity) {
            // Supprimer l'entité de tous les systèmes
            for (auto& pair : systems) {
                auto& system = pair.second;
                auto& systemEntities = system->GetEntities();

                if (systemEntities.find(entity) != systemEntities.end()) {
                    system->OnEntityRemoved(entity);
                    systemEntities.erase(entity);
                }
            }
        }

        void EntitySignatureChanged(EntityID entity, ComponentSignature entitySignature) {
            // Notifier chaque système de la modification de signature
            for (auto& pair : systems) {
                auto& type = pair.first;
                auto& system = pair.second;
                auto& systemSignature = signatures[type];
                auto& systemEntities = system->GetEntities();

                // Si la signature de l'entité correspond à celle du système
                if ((entitySignature & systemSignature) == systemSignature) {
                    // Si l'entité n'est pas déjà dans le système, l'ajouter
                    if (systemEntities.find(entity) == systemEntities.end()) {
                        systemEntities.insert(entity);
                        system->OnEntityAdded(entity);
                    }
                } else {
                    // Si l'entité est dans le système mais ne correspond plus, la supprimer
                    if (systemEntities.find(entity) != systemEntities.end()) {
                        system->OnEntityRemoved(entity);
                        systemEntities.erase(entity);
                    }
                }
            }
        }

        void UpdateSystems(Timestep ts) {
            for (auto& pair : systems) {
                auto& system = pair.second;
                system->OnUpdate(ts);
            }
        }

    private:
        // Map from system type string pointer to a signature
        std::unordered_map<const char*, ComponentSignature> signatures{};

        // Map from system type string pointer to a system instance
        std::unordered_map<const char*, std::shared_ptr<System>> systems{};
    };

}} // namespace Tesseract::ECS
