#pragma once

#include <string>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Tesseract {
    class Logger {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

        template<typename... Args>
        static void Trace(const char* fmt, Args... args) {
            GetCoreLogger()->trace(fmt, args...);
        }

        template<typename... Args>
        static void Info(const char* fmt, Args... args) {
            GetCoreLogger()->info(fmt, args...);
        }

        template<typename... Args>
        static void Warn(const char* fmt, Args... args) {
            GetCoreLogger()->warn(fmt, args...);
        }

        template<typename... Args>
        static void Error(const char* fmt, Args... args) {
            GetCoreLogger()->error(fmt, args...);
        }

        template<typename... Args>
        static void Fatal(const char* fmt, Args... args) {
            GetCoreLogger()->critical(fmt, args...);
        }

        template<typename... Args>
        static void Debug(const char* fmt, Args... args) {
            GetCoreLogger()->debug(fmt, args...);
        }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// --- Macros de Journalisation ---

// Macros pour le Logger du Noyau (Core)
#define TS_CORE_TRACE(...)    ::Tesseract::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define TS_CORE_INFO(...)     ::Tesseract::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define TS_CORE_WARN(...)     ::Tesseract::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define TS_CORE_ERROR(...)    ::Tesseract::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define TS_CORE_FATAL(...)    ::Tesseract::Logger::GetCoreLogger()->critical(__VA_ARGS__)
#define TS_CORE_DEBUG(...)    ::Tesseract::Logger::GetCoreLogger()->debug(__VA_ARGS__)

// Macros pour le Logger du Client (Application)
#define TS_TRACE(...)         ::Tesseract::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define TS_INFO(...)          ::Tesseract::Logger::GetClientLogger()->info(__VA_ARGS__)
#define TS_WARN(...)          ::Tesseract::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define TS_ERROR(...)         ::Tesseract::Logger::GetClientLogger()->error(__VA_ARGS__)
#define TS_FATAL(...)         ::Tesseract::Logger::GetClientLogger()->critical(__VA_ARGS__)
#define TS_DEBUG(...)         ::Tesseract::Logger::GetClientLogger()->debug(__VA_ARGS__)
