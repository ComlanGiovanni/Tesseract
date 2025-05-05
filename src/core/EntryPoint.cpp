#include "core/Application.hpp"
#include "core/Logger.hpp"

// Point d'entrée principal de l'application Tesseract
// Défini dans le client (ex: SandboxApp.cpp via CreateApplication)
extern Tesseract::Application* Tesseract::CreateApplication();

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    // Initialisation du Logger
    Tesseract::Logger::Init();
    TS_CORE_WARN("Initialized Core Logger");
    TS_INFO("Initialized Client Logger");

    // Création de l'application client
    auto app = Tesseract::CreateApplication();
    TS_CORE_INFO("Application created, running...");

    // Exécution de la boucle principale
    app->Run();

    // Nettoyage
    TS_CORE_INFO("Application finished, cleaning up.");
    delete app;

    return 0;
}
