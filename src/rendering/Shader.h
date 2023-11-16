#pragma once

#include <string>

#include "openglutility.h"

using std::string;

namespace carlos {
    /// @brief Represents a program that runs on the GPU (graphics card), specifically that has a vertex shader program and a fragment (pixel) shader program.
    /// Shaders are used to customize how your meshes are rendered.
    class Shader {
        private:
            static bool parseShader(const string& filePath, string& vertexShader, string& fragmentShader);
            static bool compileShader(GLuint shaderId);

        private:
            GLuint id;

            Shader(GLuint id) : id(id) { }
        public:
            static Shader* compileFrom(const string& filePath);
            ~Shader();

            void bind();
            GLuint getId() { return id; }
    };
}
