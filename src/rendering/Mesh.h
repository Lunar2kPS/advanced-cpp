#pragma once

#include "openglutility.h"

#include <vector>

using std::vector;

namespace carlos {
    class Mesh {
        private:
            int vertexCount;
            int vertexValueCount;
            float* vertices;

            int indexCount;
            GLuint* indices;

            //NOTE: Assumes GL_FLOAT and uses sizeof(float) = 4 for now,
            //  These just count the attributes and how many of each, like xy positions will be 2 floats, UV coords will be 2 floats, etc.
            int attributeCount;
            vector<int> attributeDimensions;

            //OpenGL data:

            //NOTE: VS Code tooltips with default C/C++ extension --
            //  Using "@brief" for Doxygen comments causes newlines to become ignored,
            //  So you might as well learn Doxygen tags now!
            //  (So when it comes time to generate documentation for your C++ project, you're already Doxygen-savy)
            //  See also:
            //      - https://stackoverflow.com/questions/68962181/how-to-show-new-line-in-the-hint-generated-from-comment-in-vs-code
            //      - https://www.doxygen.nl/manual/commands.html

            /// @brief The Vertex Buffer Object (VBO) on the GPU that is used for this mesh.
            /// @note - VBOs are arrays where you store a (typically huge number of) vertex attributes (positions, normals, uvs, colors, etc.).
            GLuint vbo;

            /// @brief The Index Buffer Object (IBO) on the GPU that is used for this mesh.
            /// @note - IBOs let you define which order of the vertices, in triplets, should be used to make up the triangles in your mesh.
            GLuint ibo;

            /// @brief The Vertex Array Object (VAO) on the GPU that is used for this mesh.
            /// @note - VAOs let you define the layout and order of vertex attributes in 1 or more Vertex Buffer Object(s) (VBOs).
            GLuint vao;
        public:
            Mesh(int vertexCount,
                int vertexValueCount,
                float* vertices,
                int indexCount,
                GLuint* indices,
                int attributeCount,
                vector<int> attributeDimensions
            );
            ~Mesh();

            void bind();
            void unbind();
            int getIndexCount() { return indexCount; }

            void renderDirect();
    };
}
