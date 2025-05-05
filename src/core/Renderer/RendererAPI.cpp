#include "core/Renderer/RendererAPI.hpp"

namespace Tesseract {

    // Initialise l'API par défaut à OpenGL
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

} // namespace Tesseract
