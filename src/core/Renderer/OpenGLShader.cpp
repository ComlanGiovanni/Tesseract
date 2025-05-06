#include "core/Renderer/OpenGLShader.hpp"
#include "core/Logger.hpp"
#include "core/Core.hpp" // Pour TS_ASSERT

#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h> // Utiliser GLEW au lieu de Glad
#include <glm/gtc/type_ptr.hpp> // Pour glm::value_ptr

namespace Tesseract {

    // --- Fonctions Helper (spécifiques à ce fichier) ---
    namespace Utils {
        // Fonction pour convertir notre enum GLenum (utilisé dans PreProcess)
        // en chaîne de caractères pour les logs.
        static const char* GLShaderTypeToString(GLenum type) {
            switch (type) {
                case GL_VERTEX_SHADER:   return "Vertex";
                case GL_FRAGMENT_SHADER: return "Fragment";
                // Ajouter d'autres types si nécessaire (Geometry, Compute, etc.)
            }
            TS_ASSERT(false, "Unknown shader type!");
            return nullptr;
        }
    } // namespace Utils
    // ---------------------------------------------------

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_FilePath(filepath), m_RendererID(0)
    {
        std::string source = ReadFile(filepath);
        TS_ASSERT(!source.empty(), "Failed to read shader file: {}", filepath);

        auto shaderSources = PreProcess(source);
        TS_ASSERT(shaderSources.count(GL_VERTEX_SHADER) && shaderSources.count(GL_FRAGMENT_SHADER),
                  "Shader file must contain both vertex and fragment shaders! File: {}", filepath);

        Compile(shaderSources);

        // Extraire le nom du fichier (après compilation réussie)
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_Name = filepath.substr(lastSlash, count);
    }

    OpenGLShader::~OpenGLShader() {
        if (m_RendererID) {
            glDeleteProgram(m_RendererID);
        }
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath) {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in) {
            in.seekg(0, std::ios::end);
            // Utiliser std::streamoff pour la taille et vérifier si > 0
            std::streamoff size = in.tellg();
            if (size > 0) {
                result.resize(static_cast<size_t>(size)); // Caster en size_t pour resize
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
                // Pas besoin de in.close() explicitement avec RAII (ifstream)
            } else {
                 // Gérer le cas d'un fichier vide ou erreur de tellg autre que -1
                 if (size == 0) {
                     Logger::Warn("Shader file is empty: {0}", filepath);
                 } else {
                     Logger::Error("Could not determine size of file '{0}'", filepath);
                 }
            }
        } else {
            Logger::Error("Could not open file '{0}'", filepath);
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos) {
            size_t eol = source.find_first_of("\r\n", pos);
            TS_ASSERT(eol != std::string::npos, "Syntax error in shader file: missing newline after #type directive.");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            // Trim whitespace (simple version)
            type.erase(0, type.find_first_not_of(" \t\r\n"));
            type.erase(type.find_last_not_of(" \t\r\n") + 1);

            GLenum shaderType = 0;
            if (type == "vertex")       shaderType = GL_VERTEX_SHADER;
            else if (type == "fragment") shaderType = GL_FRAGMENT_SHADER;
            else {
                TS_ASSERT(false, "Invalid shader type specified in #type directive: '{}'", type);
            }

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            TS_ASSERT(nextLinePos != std::string::npos, "Syntax error: missing shader code after #type directive.");
            pos = source.find(typeToken, nextLinePos);

            size_t startPos = nextLinePos;
            size_t length = (pos == std::string::npos) ? source.length() - startPos : pos - startPos;
            shaderSources[shaderType] = source.substr(startPos, length);
        }

        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
        GLuint program = glCreateProgram();
        std::vector<GLuint> shaderIDs;
        shaderIDs.reserve(shaderSources.size());

        Logger::Trace("Compiling shaders from file: {}", m_FilePath);

        for (auto const& [type, source] : shaderSources) {
            GLuint shader = glCreateShader(type);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            // --- DEBUG: Afficher la source exacte passée à glShaderSource ---
            Logger::Trace("--- Compiling {} Shader Source ---", Utils::GLShaderTypeToString(type));
            // Afficher ligne par ligne pour une meilleure lisibilité
            std::stringstream ss(source);
            std::string line;
            int lineNum = 1;
            while (std::getline(ss, line, '\n')) {
                 // Remplacer les caractères non imprimables potentiels pour l'affichage
                 std::string clean_line = line;
                 std::replace_if(clean_line.begin(), clean_line.end(), [](char c){ return !isprint(c) && c != '\t'; }, '?');
                 Logger::Trace("{:3d}: {}", lineNum++, clean_line);
            }
            Logger::Trace("--- End {} Shader Source ---", Utils::GLShaderTypeToString(type));
            // --- FIN DEBUG ---

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE) {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                Logger::Error("{} shader compilation failure!", Utils::GLShaderTypeToString(type));
                Logger::Error("{}", infoLog.data());
                TS_ASSERT(false, "Shader compilation failed!");
                // Nettoyer les shaders déjà compilés en cas d'erreur
                for(GLuint id : shaderIDs) glDeleteShader(id);
                glDeleteProgram(program); // Supprimer le programme aussi
                return;
            }
            Logger::Trace("  {} shader compiled successfully.", Utils::GLShaderTypeToString(type));
            glAttachShader(program, shader);
            shaderIDs.push_back(shader);
        }

        m_RendererID = program;

        // Lier le programme
        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            for (GLuint id : shaderIDs)
                glDeleteShader(id);

            Logger::Error("Shader link failure!");
            Logger::Error("{}", infoLog.data());
            TS_ASSERT(false, "Shader link failed!");
            m_RendererID = 0; // S'assurer que l'ID est nul en cas d'erreur
            return;
        }
        Logger::Trace("Shader program linked successfully.");

        // Détacher les shaders après le linkage réussi
        for (GLuint id : shaderIDs) {
            glDetachShader(program, id);
            glDeleteShader(id); // On peut supprimer les shaders individuels maintenant
        }
    }

    void OpenGLShader::Bind() const {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const {
        glUseProgram(0);
    }

    GLint OpenGLShader::GetUniformLocation(const std::string& name) {
        // Vérifier d'abord dans le cache
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
            return m_UniformLocationCache[name];
        }

        // S'assurer que le shader est bien lié avant de chercher l'uniform
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        if (currentProgram != m_RendererID) {
            Logger::Warn("Shader not bound when getting uniform location for '{}'!", name);
            Bind();
        }

        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1) {
            Logger::Warn("Uniform '{}' not found in shader '{}' (ID: {})!", name, m_Name, m_RendererID);
            // Vérifier si le shader est bien compilé et lié
            GLint status;
            glGetProgramiv(m_RendererID, GL_LINK_STATUS, &status);
            if (status == GL_FALSE) {
                Logger::Error("Shader program not properly linked!");
            }
        } else {
            Logger::Trace("Found uniform '{}' at location {} in shader '{}'", name, location, m_Name);
        }

        // Mettre en cache même les locations invalides pour éviter des recherches répétées
        m_UniformLocationCache[name] = location;
        return location;
    }

    void OpenGLShader::SetInt(const std::string& name, int value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform1i(location, value);
        }
    }

    void OpenGLShader::SetFloat(const std::string& name, float value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform1f(location, value);
        }
    }

    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform2f(location, value.x, value.y);
        }
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform3f(location, value.x, value.y, value.z);
        }
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform4f(location, value.x, value.y, value.z, value.w);
        }
    }

    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
        }
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
        }
    }

    void OpenGLShader::SetViewProjection(const glm::mat4& matrix) {
        Bind(); // S'assurer que le shader est lié
        SetMat4("u_ViewProjection", matrix);

        // Log pour le debug
        if (Logger::GetLevel() <= Logger::Level::Trace) {
            Logger::Trace("Setting ViewProjection matrix for shader '{}' (ID: {})", m_Name, m_RendererID);
            Logger::Trace("Matrix values:");
            for (int i = 0; i < 4; i++) {
                Logger::Trace("[{:.3f} {:.3f} {:.3f} {:.3f}]",
                    matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3]);
            }
        }
    }
} // namespace Tesseract
