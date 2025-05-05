#include "core/Renderer/Shader.hpp"
#include "core/Renderer/OpenGLShader.hpp"
#include "core/Core.hpp" // Pour Ref et CreateRef
#include "core/Logger.hpp"

namespace Tesseract {

    // Factory statique pour créer un Shader
    Ref<Shader> Shader::Create(const std::string& filepath) {
        // Pour l'instant, on crée toujours un OpenGLShader
        // Plus tard, on pourrait avoir une sélection basée sur l'API de rendu actuelle
        return CreateRef<OpenGLShader>(filepath);
    }

} // namespace Tesseract
