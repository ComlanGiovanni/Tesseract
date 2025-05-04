#include "core/Application.hpp"
#include "core/Logger.hpp"
#include "core/Core.hpp"

class TesseractApp : public Tesseract::Application {
public:
    TesseractApp() {
        TS_INFO("TesseractApp created!");
    }

    ~TesseractApp() {
        TS_INFO("TesseractApp destroyed!");
    }
};

Tesseract::Application* Tesseract::CreateApplication() {
    return new TesseractApp();
}
