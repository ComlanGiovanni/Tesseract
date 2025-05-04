#include "core/Application.hpp"
#include "core/Logger.hpp"

namespace Tesseract {
    class TesseractApp : public Application {
    public:
        TesseractApp() : Application("Tesseract") {
            Logger::Info("TesseractApp created!");
        }

        ~TesseractApp() {
            Logger::Info("TesseractApp destroyed!");
        }
    };

    Application* CreateApplication() {
        return new TesseractApp();
    }
}
