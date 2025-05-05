#include "core/Renderer/RenderCommand.hpp"
#include "core/Renderer/OpenGLRendererAPI.hpp" // Inclut l'implémentation concrète
#include "core/Core.hpp" // Pour CreateScope

namespace Tesseract {

    // Crée et initialise l'instance de l'API de rendu OpenGL
    // NOTE: Pour supporter d'autres API, il faudrait une logique
    //       conditionnelle ici (basée sur une config, define, etc.)
    Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();

    // Aucune autre implémentation nécessaire ici car les fonctions
    // dans le .hpp sont inline et appellent directement s_RendererAPI

} // namespace Tesseract
