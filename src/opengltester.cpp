#include "opengltester.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "openglutility.h"

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

void onGLEnable() {
    GLCALL(glGenVertexArrays(1, &vao));
    GLCALL(glGenBuffers(1, &vbo));
}

void onGLDisable() {
    GLCALL(glDeleteVertexArrays(1, &vao));
    GLCALL(glDeleteBuffers(1, &vbo));
}

void onGLUpdate() {
    // glDrawElements
}
