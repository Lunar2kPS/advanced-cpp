#pragma once

#include <string>

#include "openglutility.h"

using std::string;

namespace carlos {
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
