cmake_minimum_required(VERSION 3.16)

project(Tesseract LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Dépendances via FetchContent ---
include(FetchContent)

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.14.1
)
FetchContent_MakeAvailable(spdlog)

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        docking # Utiliser la branche docking pour les viewports/docking
)
FetchContent_MakeAvailable(imgui)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

# stb_image (header-only, mais FetchContent simplifie la gestion)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master # Ou une release tag spécifique
)
FetchContent_MakeAvailable(stb)

# --- Recherche des paquets système ---
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# --- Cible Exécutable ---
add_executable(Tesseract)

# --- Fichiers Sources ---
# Noyau
target_sources(Tesseract PRIVATE
    src/core/EntryPoint.cpp
    src/core/Application.cpp
    src/core/Window.cpp
    src/core/Logger.cpp
    src/core/Layer.cpp
    src/core/LayerStack.cpp
    src/core/Input.cpp
    src/core/events/Event.cpp
    # Renderer Core
    src/core/Renderer/OrthographicCamera.cpp
    src/core/Renderer/Shader.cpp
    src/core/Renderer/Buffer.cpp
    src/core/Renderer/VertexArray.cpp
    src/core/Renderer/RendererAPI.cpp
    src/core/Renderer/RenderCommand.cpp
    src/core/Renderer/Renderer2D.cpp
    # Renderer OpenGL
    src/core/Renderer/OpenGLShader.cpp
    src/core/Renderer/OpenGLBuffer.cpp
    src/core/Renderer/OpenGLVertexArray.cpp
    src/core/Renderer/OpenGLRendererAPI.cpp
    src/core/Renderer/OpenGLTexture.cpp
    # Application spécifique
    src/Tesseract/TesseractApp.cpp
    src/Tesseract/TesseractLayer.cpp

    # ImGui Sources (compilées directement dans l'exécutable)
    src/core/ImGui/ImGuiLayer.cpp
    # src/core/ImGui/ImGuiBuild.cpp # Plus nécessaire si on liste les sources ImGui ici
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    # Optionnel: ${imgui_SOURCE_DIR}/imgui_demo.cpp
)

# --- Répertoires d'inclusion ---
target_include_directories(Tesseract PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src # Si des includes utilisent <Tesseract/...> depuis src
    ${spdlog_SOURCE_DIR}/include
    ${imgui_SOURCE_DIR} # Inclure le répertoire source d'ImGui
    ${imgui_SOURCE_DIR}/backends # Pour imgui_impl_*
    ${glm_SOURCE_DIR}
    ${stb_SOURCE_DIR} # Pour stb_image.h
)

# --- Définitions de compilation ---
# Définir STB_IMAGE_IMPLEMENTATION une seule fois
# On le fait ici plutôt que dans un .cpp pour éviter les redéfinitions si plusieurs .cpp incluent stb_image.h
# Alternative: le définir dans un seul .cpp (ex: OpenGLTexture.cpp) et s'assurer qu'il est compilé
target_compile_definitions(Tesseract PRIVATE STB_IMAGE_IMPLEMENTATION)

# --- Liaisons (Linking) ---
target_link_libraries(Tesseract PRIVATE
    spdlog::spdlog # Cible importée par FetchContent spdlog
    # imgui          # Retiré car les sources sont compilées directement
    SDL2::SDL2     # Cible importée par find_package(SDL2)
    OpenGL::GL     # Cible importée par find_package(OpenGL)
    GLEW::GLEW     # Cible importée par find_package(GLEW)
)

# --- Options de Compilation (Optionnel, mais recommandé) ---
if(MSVC)
    # Options spécifiques à MSVC
    target_compile_options(Tesseract PRIVATE /W4 /WX)
else()
    # Options pour GCC/Clang
    target_compile_options(Tesseract PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Pour que l'exécutable soit dans le répertoire build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
