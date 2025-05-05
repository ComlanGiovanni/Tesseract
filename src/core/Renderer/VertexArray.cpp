#include "core/Renderer/VertexArray.hpp"
#include "core/Renderer/OpenGLVertexArray.hpp"
#include "core/Core.hpp"
#include "core/Logger.hpp"

namespace Tesseract {

    Ref<VertexArray> VertexArray::Create() {
        // Pour l'instant, on crée toujours un OpenGLVertexArray
        return CreateRef<OpenGLVertexArray>();
    }

} // namespace Tesseract
