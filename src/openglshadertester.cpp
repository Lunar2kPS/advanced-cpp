#include "openglshadertester.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "glad/gl.h"
#include "stb/stb_image.h"

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
static GLuint mainTexture;

void parseShader(const string& filePath, string& vertexShader, string& fragmentShader) {
    ifstream file = ifstream(filePath);
    if (!file.is_open()) {
        cout << "Unable to find shader at file path: \"" << filePath << "\"" << endl;
        vertexShader = "";
        fragmentShader = "";
        return;
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
            ss[(int) currentType] << line << '\n';
        }
    }
    file.close();

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

    string filePath = "";
    parseShader(filePath, vSrc, fSrc);

    const GLchar* vSrcCStr = vSrc.c_str();
    GLCALL(GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER));
    GLCALL(glShaderSource(vertexShader, 1, &vSrcCStr, nullptr));

    const GLchar* fSrcCStr = fSrc.c_str();
    GLCALL(GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
    GLCALL(glShaderSource(fragmentShader, 1, &fSrcCStr, nullptr));

    bool compileSuccess = compileShader(vertexShader) && compileShader(fragmentShader);
    if (compileSuccess) {
        glClearPreviousErrorFlag();
        GLCALL(program = glCreateProgram());
        GLCALL(glAttachShader(program, vertexShader));
        GLCALL(glAttachShader(program, fragmentShader));
        GLCALL(glLinkProgram(program));
        GLCALL(glValidateProgram(program));
        GLCALL(glDeleteShader(vertexShader));
        GLCALL(glDeleteShader(fragmentShader));

        GLCALL(glUseProgram(program));
        if (glHadPreviousErrors())
            compileSuccess = false;

        if (compileSuccess) {
            GLCALL(int location = glGetUniformLocation(program, "color"));
            GLCALL(glUniform4f(location, 0, 1, 0.5f, 1));

            GLCALL(location = glGetUniformLocation(program, "mainTexture"));
            if (location >= 0) {
                stbi_set_flip_vertically_on_load(1);
                
                int width;
                int height;
                int channelCount;
                unsigned char* imageData = stbi_load("resources/Grass Tile.png", &width, &height, &channelCount, STBI_rgb_alpha);
                if (imageData != nullptr) {
                    GLCALL(glActiveTexture(GL_TEXTURE0));

                    GLCALL(glGenTextures(1, &mainTexture));
                    GLCALL(glBindTexture(GL_TEXTURE_2D, mainTexture));
                    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

                    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData));
                    GLCALL(glUniform1i(location, 0));
                }
                stbi_image_free(imageData);
            }
        }
    }
    
    if (!compileSuccess) {
        cout << "Failed to compile shader at file path: \"" << filePath << "\"\n" << endl;
        program = GL_NONE;
    }
}

void deleteShader() {
    GLCALL(glDeleteProgram(program));
    GLCALL(glDeleteTextures(1, &mainTexture));
}

