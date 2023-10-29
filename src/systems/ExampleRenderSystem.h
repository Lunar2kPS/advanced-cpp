#pragma once

#include "openglutility.h"
#include "interfaces/IGameLoopSystem.h"

using std::string;

namespace carlos {
    class ExampleRenderSystem : public IGameLoopSystem {
        private:
            //NOTE: VS Code tooltips with default C/C++ extension --
            //  Using "@brief" for Doxygen comments causes newlines to become ignored,
            //  So you might as well learn Doxygen tags now!
            //  (So when it comes time to generate documentation for your C++ project, you're already Doxygen-savy)
            //  See also:
            //      - https://stackoverflow.com/questions/68962181/how-to-show-new-line-in-the-hint-generated-from-comment-in-vs-code
            //      - https://www.doxygen.nl/manual/commands.html

            /// @brief The Vertex Array Object (VAO), used for testing in this C++ source file.
            /// @note - VAOs let you define the layout and order of vertex attributes in 1 or more Vertex Buffer Object(s) (VBOs).
            GLuint vao;

            /// @brief The Vertex Buffer Object (VBO), used for testing in this C++ source file. 
            /// @note - VBOs are arrays where you store a (typically huge number of) vertex attributes (positions, normals, uvs, colors, etc.).
            GLuint vbo;

            /// @brief The Index Buffer Object (IBO), used for testing in this C++ source file.
            /// @note - IBOs let you define which order of the vertices, in triplets, should be used to make up the triangles in your mesh.
            GLuint ibo;

            bool isAbleToDraw = true;
            GLuint* indices;
            int indexCount;
            float* vertices;
            int vertexValueCount;

            GLuint program;
            GLuint mainTexture;

            void parseShader(const string& filePath, string& vertexShader, string& fragmentShader);
            bool compileShader(GLuint shaderId);
            GLuint createShader(const string& filePath);
            void deleteShader(GLuint shaderId);
        public:
            ExampleRenderSystem();
            ~ExampleRenderSystem();
            void update();
            void render();
    };
}
