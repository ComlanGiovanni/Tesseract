# Configuration des fichiers sources
set(SOURCES main.cpp)

# Création de l'exécutable principal
add_executable(tesseract ${SOURCES})

# Configuration des dépendances
target_link_libraries(tesseract
    PRIVATE
    OpenGL::GL
    SDL2::SDL2
    SDL2::SDL2main
    GLEW::GLEW
    imgui
)

# Configuration des include directories
target_include_directories(tesseract
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/lib/imgui
    ${CMAKE_SOURCE_DIR}/lib/imgui/backends
)

