#pragma once

#include <cstdint>
#include <bitset>
#include <array>
#include <unordered_map>
#include <memory>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <type_traits>

namespace Tesseract {
namespace ECS {

    // Définitions des types de base pour l'ECS
    using EntityID = std::uint32_t;
    using ComponentTypeID = std::uint8_t;

    // Constantes
    constexpr EntityID MAX_ENTITIES = 5000;
    constexpr ComponentTypeID MAX_COMPONENTS = 32;

    // "Signature" pour identifier les composants qu'une entité possède
    using ComponentSignature = std::bitset<MAX_COMPONENTS>;

    // Forward declarations
    class Entity;
    class Registry;
    class IComponentArray;

    // Helper pour obtenir l'ID de type d'un composant
    template<typename T>
    ComponentTypeID GetComponentTypeID() {
        static_assert(std::is_class<T>::value, "T must be a class/struct type");

        static ComponentTypeID typeID = ComponentTypeID(-1);
        if (typeID == ComponentTypeID(-1)) {
            // Cette partie est exécutée une seule fois par type de composant
            static ComponentTypeID nextTypeID = 0;
            typeID = nextTypeID++;

            // Vérifier le dépassement
            if (nextTypeID >= MAX_COMPONENTS) {
                throw std::runtime_error("Component type limit exceeded!");
            }
        }
        return typeID;
    }

}} // namespace Tesseract::ECS
