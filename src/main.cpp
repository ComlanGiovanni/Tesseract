#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Démarrage de l'application..." << std::endl;

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erreur lors de l'initialisation de SDL: " << SDL_GetError() << std::endl;
        return -1;
    }
    std::cout << "SDL initialisé avec succès" << std::endl;

    // Configuration des attributs OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow(
        "Tesseract - Debug",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,  // Largeur augmentée
        600,  // Hauteur modifiée
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cerr << "Erreur lors de la création de la fenêtre: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    std::cout << "Fenêtre créée avec succès" << std::endl;

    // Création du contexte OpenGL
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        std::cerr << "Erreur lors de la création du contexte OpenGL: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    std::cout << "Contexte OpenGL créé avec succès" << std::endl;

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialisation de GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Erreur lors de l'initialisation de GLEW" << std::endl;
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    std::cout << "GLEW initialisé avec succès" << std::endl;

    // Vérification de la version OpenGL
    const GLubyte* glVersion = glGetString(GL_VERSION);
    std::cout << "Version OpenGL: " << glVersion << std::endl;

    // Configuration d'ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Style d'ImGui
    ImGui::StyleColorsDark();

    // Configuration du backend ImGui
    if (!ImGui_ImplSDL2_InitForOpenGL(window, gl_context)) {
        std::cerr << "Erreur lors de l'initialisation du backend SDL2" << std::endl;
        return -1;
    }
    if (!ImGui_ImplOpenGL3_Init("#version 130")) {
        std::cerr << "Erreur lors de l'initialisation du backend OpenGL3" << std::endl;
        return -1;
    }
    std::cout << "ImGui initialisé avec succès" << std::endl;

    // Variables pour la démo
    bool show_demo_window = true;
    bool show_debug_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Boucle principale
    bool running = true;
    int frame_count = 0;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window))
                running = false;
        }

        // Début du frame ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Fenêtre de démo ImGui
        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        // Fenêtre de debug personnalisée
        if (show_debug_window) {
            ImGui::Begin("Debug Window", &show_debug_window);
            ImGui::Text("Bienvenue dans Tesseract!");
            ImGui::Text("Cette fenêtre devrait être visible");
            ImGui::ColorEdit3("Couleur de fond", (float*)&clear_color);
            ImGui::Checkbox("Demo Window", &show_demo_window);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // Rendu
        ImGui::Render();
        int display_w, display_h;
        SDL_GL_GetDrawableSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
                    clear_color.y * clear_color.w,
                    clear_color.z * clear_color.w,
                    clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // Vérification des erreurs OpenGL
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "Erreur OpenGL: " << err << std::endl;
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);

        // Afficher un message toutes les 60 frames
        frame_count++;
        if (frame_count % 60 == 0) {
            std::cout << "Frame " << frame_count << " - FPS: " << io.Framerate << std::endl;
        }
    }

    // Nettoyage
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
