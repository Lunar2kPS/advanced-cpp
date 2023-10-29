#include "systems/ExampleRenderSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//TODO: Have this 1 special include in a better place..
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

//Grass Tiling!
//  - 1 mesh
//  - X transforms
//  - TODO: Batching or GPU instancing

namespace carlos {
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    void ExampleRenderSystem::parseShader(const string& filePath, string& vertexShader, string& fragmentShader) {
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
    }

    bool ExampleRenderSystem::compileShader(GLuint shaderId) {
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

    GLuint ExampleRenderSystem::createShader(const string& filePath) {
        string vSrc;
        string fSrc;

        parseShader(filePath, vSrc, fSrc);

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

            GLCALL(glUseProgram(shaderId));
            if (glHadPreviousErrors())
                compileSuccess = false;

            if (compileSuccess) {
                GLCALL(int location = glGetUniformLocation(shaderId, "color"));
                GLCALL(glUniform4f(location, 0, 1, 0.5f, 1));

                GLCALL(location = glGetUniformLocation(shaderId, "mainTexture"));
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
            shaderId = GL_NONE;
        }
        return shaderId;
    }

    void ExampleRenderSystem::deleteShader(GLuint shaderId) {
        GLCALL(glDeleteProgram(shaderId));
        GLCALL(glDeleteTextures(1, &mainTexture));
    }

    ExampleRenderSystem::ExampleRenderSystem() {
        indexCount = 6;
        int vertexCount = 4;
        int vertexAttributeDimensionCount = 2 + 2; //xy positions, uv texcoords
        vertexValueCount = vertexCount * vertexAttributeDimensionCount;

        indices = new GLuint[indexCount] {
            0, 1, 2,
            3, 2, 1
        };

        vertices = new float[vertexValueCount] {
            0.5f, -0.5f, 1, 0,
            -0.5f, -0.5f, 0, 0,
            0.5f,  0.5f, 1, 1,
            -0.5f,  0.5f, 0, 1
        };

        GLCALL(glGenBuffers(1, &vbo));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, vertexValueCount * sizeof(float), vertices, GL_STATIC_DRAW));

        GLCALL(glGenBuffers(1, &ibo));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));

        GLCALL(glGenVertexArrays(1, &vao));
        GLCALL(glBindVertexArray(vao));
        
        unsigned int offset = 0;
        GLsizei totalStride = vertexAttributeDimensionCount * sizeof(float);
        GLCALL(glEnableVertexAttribArray(0));
        GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, false, totalStride, (const void*) offset)); //NOTE: This requires a currently-bound GL_ARRAY_BUFFER target
        offset += 2 * sizeof(float);

        GLCALL(glEnableVertexAttribArray(1));
        GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, false, totalStride, (const void*) offset));
        offset += 2 * sizeof(float);

        GLCALL(glBindVertexArray(NULL));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, NULL));

        program = createShader("resources/Unlit Texture.glsl");
    }

    ExampleRenderSystem::~ExampleRenderSystem() {
        GLCALL(glDeleteVertexArrays(1, &vao));
        GLCALL(glDeleteBuffers(1, &vbo));
        GLCALL(glDeleteBuffers(1, &ibo));

        delete[] indices;
        delete[] vertices;

        deleteShader(program);
        program = GL_NONE;
    }

    void ExampleRenderSystem::update() {

    }

    void ExampleRenderSystem::render() {
        if (isAbleToDraw) {
            glClearPreviousErrorFlag();
            GLCALL(glBindVertexArray(vao));
            GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
            GLCALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
            GLCALL(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL));
            GLCALL(glBindVertexArray(NULL));
            GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));
            GLCALL(glBindBuffer(GL_ARRAY_BUFFER, NULL));
            if (glHadPreviousErrors()) {
                cout << "Encountered an error! Stopping drawing..." << endl;
                isAbleToDraw = false;
            }
        }
    }
}
