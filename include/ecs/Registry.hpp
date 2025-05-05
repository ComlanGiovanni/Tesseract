#pragma once

#include "ecs/Core.hpp"
#include "ecs/ComponentManager.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/System.hpp"
#include "core/Core.hpp"
#include <memory>

namespace Tesseract {
namespace ECS {

    /**
     * @class Registry
     * @brief Coordonne toutes les classes du système ECS
     *
     * Le Registry est le point d'entrée principal pour l'utilisation du système ECS.
     * Il coordonne les entités, composants et systèmes.
     */
    class Registry {
    public:
        Registry() {
            componentManager = CreateScope<ComponentManager>();
            entityManager = CreateScope<EntityManager>();
            systemManager = CreateScope<SystemManager>();
        }

        // --- Entity methods ---

        /**
         * @brief Crée une nouvelle entité
         * @return ID de l'entité créée
         */
        EntityID CreateEntity() {
            return entityManager->CreateEntity();
        }

        /**
         * @brief Détruit une entité et tous ses composants
         * @param entity ID de l'entité à détruire
         */
        void DestroyEntity(EntityID entity) {
            entityManager->DestroyEntity(entity);
            componentManager->EntityDestroyed(entity);
            systemManager->EntityDestroyed(entity);
        }

        // --- Component methods ---

        /**
         * @brief Enregistre un type de composant dans le système
         * @tparam T Type du composant à enregistrer
         */
        template<typename T>
        void RegisterComponent() {
            componentManager->RegisterComponent<T>();
        }

        /**
         * @brief Ajoute un composant à une entité
         * @tparam T Type du composant à ajouter
         * @param entity ID de l'entité
         * @param component Instance du composant à ajouter
         * @return Référence au composant ajouté
         */
        template<typename T>
        T& AddComponent(EntityID entity, const T& component) {
            componentManager->AddComponent<T>(entity, component);

            auto signature = entityManager->GetSignature(entity);
            signature.set(componentManager->GetComponentType<T>(), true);
            entityManager->SetSignature(entity, signature);

            systemManager->EntitySignatureChanged(entity, signature);

            return componentManager->GetComponent<T>(entity);
        }

        /**
         * @brief Supprime un composant d'une entité
         * @tparam T Type du composant à supprimer
         * @param entity ID de l'entité
         */
        template<typename T>
        void RemoveComponent(EntityID entity) {
            componentManager->RemoveComponent<T>(entity);

            auto signature = entityManager->GetSignature(entity);
            signature.set(componentManager->GetComponentType<T>(), false);
            entityManager->SetSignature(entity, signature);

            systemManager->EntitySignatureChanged(entity, signature);
        }

        /**
         * @brief Récupère un composant d'une entité
         * @tparam T Type du composant à récupérer
         * @param entity ID de l'entité
         * @return Référence au composant
         */
        template<typename T>
        T& GetComponent(EntityID entity) {
            return componentManager->GetComponent<T>(entity);
        }

        /**
         * @brief Vérifie si une entité possède un composant
         * @tparam T Type du composant à vérifier
         * @param entity ID de l'entité
         * @return true si l'entité possède le composant, false sinon
         */
        template<typename T>
        bool HasComponent(EntityID entity) const {
            return componentManager->HasComponent<T>(entity);
        }

        /**
         * @brief Récupère l'ID de type d'un composant
         * @tparam T Type du composant
         * @return ID du type de composant
         */
        template<typename T>
        ComponentTypeID GetComponentType() {
            return componentManager->GetComponentType<T>();
        }

        // --- System methods ---

        /**
         * @brief Enregistre un système dans le registre
         * @tparam T Type du système à enregistrer
         * @return Pointeur partagé vers le système enregistré
         */
        template<typename T>
        std::shared_ptr<T> RegisterSystem() {
            return systemManager->RegisterSystem<T>();
        }

        /**
         * @brief Définit la signature d'un système (quels composants il traite)
         * @tparam T Type du système
         * @param signature Signature des composants requis
         */
        template<typename T>
        void SetSystemSignature(ComponentSignature signature) {
            systemManager->SetSignature<T>(signature);
        }

        /**
         * @brief Met à jour tous les systèmes
         * @param ts Pas de temps pour la mise à jour
         */
        void UpdateSystems(Timestep ts) {
            systemManager->UpdateSystems(ts);
        }

    private:
        Scope<ComponentManager> componentManager;
        Scope<EntityManager> entityManager;
        Scope<SystemManager> systemManager;
    };

}} // namespace Tesseract::ECS
