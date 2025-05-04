cmake_minimum_required(VERSION 3.10)
project(Tesseract)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add spdlog as a subdirectory
add_subdirectory(lib/spdlog)

# Add ImGui as a subdirectory
add_subdirectory(lib/imgui)

# Find required packages
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    include
    lib/spdlog/include
    lib/imgui
)

# Source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

# Create executable
add_executable(Tesseract ${SOURCES})

# Link libraries
target_link_libraries(Tesseract
    ${SDL2_LIBRARIES}
    ${GLEW_LIBRARIES}
    spdlog
    imgui
)
