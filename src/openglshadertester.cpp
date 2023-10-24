#include "openglshadertester.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "glad/gl.h"
#include "openglutility.h"

using std::ifstream;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;

enum class ShaderType {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
};

static GLuint program;

void parseShader(const string& filePath, string& vertexShader, string& fragmentShader) {
    ifstream file = ifstream(filePath);
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
            ss[(int) currentType] << line << '\n';
        }
    }

    vertexShader = ss[(int) ShaderType::VERTEX].str();
    fragmentShader = ss[(int) ShaderType::FRAGMENT].str();
}

bool compileShader(GLuint shaderId) {
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

void createShader() {
    string vSrc;
    string fSrc;
    parseShader("resources/Color Shader.glsl", vSrc, fSrc);

    const GLchar* vSrcCStr = vSrc.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glLogAllErrors();
    GLCALL(glShaderSource(vertexShader, 1, &vSrcCStr, nullptr));

    const GLchar* fSrcCStr = fSrc.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glLogAllErrors();
    GLCALL(glShaderSource(fragmentShader, 1, &fSrcCStr, nullptr));

    if (compileShader(vertexShader) && compileShader(fragmentShader)) {
        program = glCreateProgram();
        glLogAllErrors();
        GLCALL(glAttachShader(program, vertexShader));
        GLCALL(glAttachShader(program, fragmentShader));
        GLCALL(glLinkProgram(program));
        GLCALL(glValidateProgram(program));
        GLCALL(glDeleteShader(vertexShader));
        GLCALL(glDeleteShader(fragmentShader));

        GLCALL(glUseProgram(program));
    } else {
        program = GL_NONE;
    }
}

void deleteShader() {
    if (program != GL_NONE)
        GLCALL(glDeleteProgram(program));
}

