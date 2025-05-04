#include "core/Application.hpp"
#include "core/Logger.hpp"

extern Tesseract::Application* Tesseract::CreateApplication();

int main(int argc, char** argv) {
    Tesseract::Logger::Init();
    auto app = Tesseract::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
