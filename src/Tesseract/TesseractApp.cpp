#include "Tesseract/TesseractApp.hpp"
#include "core/Core.hpp"
#include <core/Application.hpp>
#include <core/Logger.hpp>
#include "Tesseract/TesseractLayer.hpp"

namespace Tesseract {
    TesseractApp::TesseractApp() : Application("Tesseract Sandbox") {
        TS_INFO("TesseractApp created!");
        PushLayer(CreateRef<TesseractLayer>());
    }

    TesseractApp::~TesseractApp() {
        TS_INFO("TesseractApp destroyed!");
    }

    Application* CreateApplication() {
        return new TesseractApp();
    }
}
