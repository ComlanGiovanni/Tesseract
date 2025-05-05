#include "core/ImGui/ImGuiLayer.hpp"
#include "core/Application.hpp" // Pour GetWindow()
#include "core/Logger.hpp"

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <SDL.h>
#include <GL/glew.h> // Pour les fonctions OpenGL directes (ex: Clear)

namespace Tesseract {

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer() {}

    void ImGuiLayer::OnAttach() {
        Logger::Info("Attaching ImGuiLayer...");
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        Application& app = Application::Get();
        SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());
        SDL_GLContext context = static_cast<SDL_GLContext>(app.GetWindow().GetNativeContext());

        ImGui_ImplSDL2_InitForOpenGL(window, context);
        ImGui_ImplOpenGL3_Init("#version 330"); // GLSL version

        Logger::Info("ImGuiLayer Attached and Initialized.");
    }

    void ImGuiLayer::OnDetach() {
        Logger::Info("Detaching ImGuiLayer...");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        Logger::Info("ImGuiLayer Detached.");
    }

    void ImGuiLayer::Begin() {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
        // Pas besoin de clear ici, c'est fait par Application::Run
        // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to be polite to the user.)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
    }

    void ImGuiLayer::OnImGuiRender() {
        // Mettre ici du code ImGui spécifique à la couche elle-même, si nécessaire.
        // Par exemple, la fenêtre de démo ImGui :
        // static bool show_demo_window = true;
        // if (show_demo_window)
        //     ImGui::ShowDemoWindow(&show_demo_window);
    }

    void ImGuiLayer::OnEvent(Event& event) {
        // Gérer les événements pour ImGui ici si nécessaire,
        // bien que ImGui_ImplSDL2_ProcessEvent dans Window::OnUpdate s'en charge déjà.
        // On pourrait vouloir bloquer les événements si ImGui les capture.
        ImGuiIO& io = ImGui::GetIO();
        event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }

} // namespace Tesseract
