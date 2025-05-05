#pragma once

#include <memory>
#include "core/Logger.hpp"

#ifdef TS_PLATFORM_WINDOWS
    #define TS_DEBUG_BREAK __debugbreak()
#else
    #include <signal.h>
    #define TS_DEBUG_BREAK raise(SIGTRAP)
#endif

#define TS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#ifdef TS_ENABLE_ASSERTS
    // Macro interne pour gérer __VA_ARGS__ proprement
    #define TS_ASSERT_RESOLVE(arg1, arg2, ...) arg2
    #define TS_GET_ASSERT_MACRO(...) TS_ASSERT_RESOLVE(__VA_ARGS__, TS_ASSERT_MSG, TS_ASSERT_NO_MSG)

    // TS_ASSERT(condition, ...) -> __VA_ARGS__ contient le message formaté
    #define TS_ASSERT_MSG(condition, ...) if (!(condition)) { TS_CRITICAL("Assertion Failed: {0}", fmt::format(__VA_ARGS__)); TS_DEBUG_BREAK; }
    // TS_ASSERT(condition) -> __VA_ARGS__ est vide
    #define TS_ASSERT_NO_MSG(condition) if (!(condition)) { TS_CRITICAL("Assertion Failed: {0}", #condition); TS_DEBUG_BREAK; }

    // Macro principale TS_ASSERT
    #define TS_ASSERT(...) TS_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__)

#else
    #define TS_ASSERT(...) // Désactivé en release
#endif

#define BIT(x) (1 << x)

namespace Tesseract {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
