#pragma once

#include <memory>

#ifdef TS_PLATFORM_WINDOWS
    #define TS_DEBUG_BREAK __debugbreak()
#else
    #include <signal.h>
    #define TS_DEBUG_BREAK raise(SIGTRAP)
#endif

#define TS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define TS_ASSERT(x) if (!(x)) { TS_ERROR("Assertion Failed: {0}", #x); TS_DEBUG_BREAK; }

#define BIT(x) (1 << x)

// Macros de logging
#define TS_TRACE(...) Tesseract::Logger::Trace(__VA_ARGS__)
#define TS_DEBUG(...) Tesseract::Logger::Debug(__VA_ARGS__)
#define TS_INFO(...)  Tesseract::Logger::Info(__VA_ARGS__)
#define TS_WARN(...)  Tesseract::Logger::Warn(__VA_ARGS__)
#define TS_ERROR(...) Tesseract::Logger::Error(__VA_ARGS__)
#define TS_FATAL(...) Tesseract::Logger::Fatal(__VA_ARGS__)

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
