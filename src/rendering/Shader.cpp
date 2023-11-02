#include "rendering/Shader.h"

#include <fstream>
#include <sstream>

using std::ifstream;
using std::stringstream;

namespace carlos {
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    bool Shader::parseShader(const string& filePath, string& vertexShader, string& fragmentShader) {
        ifstream file = ifstream(filePath);
        if (!file.is_open()) {
            cout << "Unable to find shader at file path: \"" << filePath << "\"" << endl;
            vertexShader = "";
            fragmentShader = "";
            return false;
        }

        stringstream ss[2];
        ShaderType currentType = ShaderType::NONE;

        string line;
        while (getline(file, line)) {
            if (line.find("#shader") != string::npos) {
                if (line.find("vertex") != string::npos)
                    currentType = ShaderType::VERTEX;
                else if (line.find("fragment") != string::npos)
                    currentType = ShaderType::FRAGMENT;
            } else if (currentType != ShaderType::NONE) {
                int index = (int) currentType;
                //TODO: Support for this #version line to be ignored when commented out
                if (line.find("#version AUTO") != string::npos) {
#if defined(GRAPHICS_API_GL)
                    ss[index] << "#version 330 core" << '\n';
#elif defined(GRAPHICS_API_GLES)
                    ss[index] << "#version 300 es" << '\n';
#endif
                } else {
                    ss[index] << line << '\n';
                }
            }
        }
        file.close();

        vertexShader = ss[(int) ShaderType::VERTEX].str();
        fragmentShader = ss[(int) ShaderType::FRAGMENT].str();
        return true;
    }

    bool Shader::compileShader(GLuint shaderId) {
        GLCALL(glCompileShader(shaderId));

        int result;
        GLCALL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result));
        if (result == GL_FALSE) {
            int length;
            GLCALL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));

            GLchar* message = new GLchar[length];
            GLCALL(glGetShaderInfoLog(shaderId, length, &length, message));

            //TODO: Make sure that GLchar is compatible with cout (basically, that GLchar === char)
            cout << "Failed to compile a shader!" << endl;
            cout << message << endl;
            delete[] message;

            GLCALL(glDeleteShader(shaderId));
            return false;
        }
        return true;
    }

    Shader* Shader::compileFrom(const string& filePath) {
        string vSrc;
        string fSrc;

        if (!parseShader(filePath, vSrc, fSrc))
            return nullptr;

        const GLchar* vSrcCStr = vSrc.c_str();
        GLCALL(GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER));
        GLCALL(glShaderSource(vertexShader, 1, &vSrcCStr, nullptr));

        const GLchar* fSrcCStr = fSrc.c_str();
        GLCALL(GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
        GLCALL(glShaderSource(fragmentShader, 1, &fSrcCStr, nullptr));

        GLuint shaderId = GL_NONE;
        bool compileSuccess = compileShader(vertexShader) && compileShader(fragmentShader);
        if (compileSuccess) {
            glClearPreviousErrorFlag();
            GLCALL(shaderId = glCreateProgram());
            GLCALL(glAttachShader(shaderId, vertexShader));
            GLCALL(glAttachShader(shaderId, fragmentShader));
            GLCALL(glLinkProgram(shaderId));
            GLCALL(glValidateProgram(shaderId));
            GLCALL(glDeleteShader(vertexShader));
            GLCALL(glDeleteShader(fragmentShader));

            if (glHadPreviousErrors())
                compileSuccess = false;
        }
        
        if (!compileSuccess) {
            cout << "Failed to compile shader at file path: \"" << filePath << "\"\n" << endl;
            shaderId = GL_NONE;
            return nullptr;
        }
        return new Shader(shaderId);
    }

    void Shader::bind() {
        GLCALL(glUseProgram(id));
    }

    Shader::~Shader() {
        GLCALL(glDeleteProgram(id));
    }
}
