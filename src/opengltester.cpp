#include "opengltester.h"

#include <iostream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "openglutility.h"

using std::cout;
using std::endl;

//NOTE: VS Code tooltips with default C/C++ extension --
//  Using "@brief" for Doxygen comments causes newlines to become ignored,
//  So you might as well learn Doxygen tags now!
//  (So when it comes time to generate documentation for your C++ project, you're already Doxygen-savy)
//  See also:
//      - https://stackoverflow.com/questions/68962181/how-to-show-new-line-in-the-hint-generated-from-comment-in-vs-code
//      - https://www.doxygen.nl/manual/commands.html

/// @brief The Vertex Array Object (VAO), used for testing in this C++ source file.
/// @note - VAOs let you define the layout and order of vertex attributes in 1 or more Vertex Buffer Object(s) (VBOs).
static GLuint vao;

/// @brief The Vertex Buffer Object (VBO), used for testing in this C++ source file. 
/// @note - VBOs are arrays where you store a (typically huge number of) vertex attributes (positions, normals, uvs, colors, etc.).
static GLuint vbo;

static GLuint ibo;

static GLuint* indices;
static int indexCount;
static float* positions;
static int positionCount;

void onGLEnable() {
    indexCount = 3; //6;
    positionCount = 8;

    indices = new GLuint[indexCount] {
        0, 1, 2 //,
        // 3, 2, 1
    };

    positions = new float[positionCount] {
         0.5f, -0.5f,
        -0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    GLCALL(glGenBuffers(1, &vbo));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, positionCount * sizeof(float), positions, GL_STATIC_DRAW));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, NULL)); //TODO: Test if NULL is defined on platforms other than Windows, cause nullptr doesn't work here

    GLCALL(glGenBuffers(1, &ibo));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL)); //TODO: Test if NULL is defined on platforms other than Windows, cause nullptr doesn't work here

    // unsigned int offset = 0;
    // GLCALL(glGenVertexArrays(1, &vao));
    // GLCALL(glBindVertexArray(vao));
    // GLCALL(glEnableVertexAttribArray(0));
    // GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), (const void*) offset)); //NOTE: This requires a currently-bound GL_ARRAY_BUFFER target
}

void onGLDisable() {
    // GLCALL(glDeleteVertexArrays(1, &vao));
    GLCALL(glDeleteBuffers(1, &vbo));
    GLCALL(glDeleteBuffers(1, &ibo));

    delete[] indices;
    delete[] positions;
}

void onGLUpdate() {
    // GLCALL(glBindVertexArray(vao));
    // GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    // GLCALL(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL));
    // GLCALL(glBindVertexArray(NULL));
    // GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));
}
