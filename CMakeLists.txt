cmake_minimum_required(VERSION 3.15)
project(Tesseract VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options de compilation
option(BUILD_TESTS "Build test programs" OFF)
option(BUILD_DOCS "Build documentation" OFF)

# Configuration des sous-répertoires
add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(lib)

# Configuration des dépendances externes
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

# Configuration des options de compilation
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Werror -Wpedantic)
endif()

# Configuration des variables globales
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
