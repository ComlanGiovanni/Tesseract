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
