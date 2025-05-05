#pragma once

#include "ecs/Registry.hpp"
#include "ecs/Components.hpp"

namespace Tesseract {
namespace ECS {

    /**
     * @class Entity
     * @brief Interface utilisateur pour manipuler des entités dans le système ECS
     *
     * Cette classe encapsule un EntityID et offre une API conviviale pour
     * manipuler les composants d'une entité.
     */
    class Entity {
    public:
        Entity() = default;
        Entity(EntityID id, Registry* registry) : m_EntityID(id), m_Registry(registry) {}

        /**
         * @brief Vérifie si l'entité est valide
         * @return true si l'entité est valide, false sinon
         */
        operator bool() const { return m_EntityID != 0 && m_Registry != nullptr; }

        /**
         * @brief Obtient l'ID de l'entité
         * @return ID de l'entité
         */
        EntityID GetID() const { return m_EntityID; }

        /**
         * @brief Vérifie si l'entité a un composant
         * @tparam T Type du composant à vérifier
         * @return true si l'entité a le composant, false sinon
         */
        template<typename T>
        bool HasComponent() const {
            return m_Registry->HasComponent<T>(m_EntityID);
        }

        /**
         * @brief Ajoute un composant à l'entité
         * @tparam T Type du composant à ajouter
         * @tparam Args Types des arguments du constructeur du composant
         * @param args Arguments à transmettre au constructeur du composant
         * @return Référence au composant ajouté
         */
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            return m_Registry->AddComponent<T>(m_EntityID, T(std::forward<Args>(args)...));
        }

        /**
         * @brief Récupère un composant de l'entité
         * @tparam T Type du composant à récupérer
         * @return Référence au composant
         */
        template<typename T>
        T& GetComponent() {
            return m_Registry->GetComponent<T>(m_EntityID);
        }

        /**
         * @brief Supprime un composant de l'entité
         * @tparam T Type du composant à supprimer
         */
        template<typename T>
        void RemoveComponent() {
            m_Registry->RemoveComponent<T>(m_EntityID);
        }

        /**
         * @brief Détruit l'entité
         */
        void Destroy() {
            if (m_Registry)
                m_Registry->DestroyEntity(m_EntityID);
            m_EntityID = 0;
            m_Registry = nullptr;
        }

        /**
         * @brief Opérateurs d'égalité
         */
        bool operator==(const Entity& other) const {
            return m_EntityID == other.m_EntityID && m_Registry == other.m_Registry;
        }

        bool operator!=(const Entity& other) const {
            return !(*this == other);
        }

    private:
        EntityID m_EntityID = 0;
        Registry* m_Registry = nullptr;
    };

}} // namespace Tesseract::ECS
