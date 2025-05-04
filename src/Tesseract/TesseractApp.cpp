#include <core/Core.hpp>
#include <core/Application.hpp>
#include <core/Logger.hpp>
#include "Tesseract/SandboxLayer.hpp"

namespace Tesseract {
    class TesseractApp : public Application {
    public:
        TesseractApp() : Application("Tesseract Sandbox") {
            Logger::Info("TesseractApp created!");
            PushLayer(new SandboxLayer());
        }

        ~TesseractApp() {
            Logger::Info("TesseractApp destroyed!");
        }
    };

    Application* CreateApplication() {
        return new TesseractApp();
    }
}
