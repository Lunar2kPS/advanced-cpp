#include "opengltester.h"

#include <iostream>

#include "glad/gl.h"
#include "openglutility.h"

#define USE_SHADER

#if defined(USE_SHADER)
    #include "openglshadertester.h"
#endif

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

/// @brief The Index Buffer Object (IBO), used for testing in this C++ source file.
/// @note - IBOs let you define which order of the vertices, in triplets, should be used to make up the triangles in your mesh.
static GLuint ibo;

static GLuint* indices;
static int indexCount;
static float* vertices;
static int vertexValueCount;

void createMesh();
void deleteMesh();
void drawMesh();

void onGLEnable() {
    createMesh();
    #if defined(USE_SHADER)
        createShader();
    #endif
}

void onGLDisable() {
    deleteMesh();
    #if defined(USE_SHADER)
        deleteShader();
    #endif
}

void onGLUpdate() {
    drawMesh();
}


void createMesh() {
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

    //TODO: Test if NULL is defined on platforms other than Windows, cause nullptr doesn't work here with glBindBuffer accepting in an integer (like NULL = 0).

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
}

void deleteMesh() {
    GLCALL(glDeleteVertexArrays(1, &vao));
    GLCALL(glDeleteBuffers(1, &vbo));
    GLCALL(glDeleteBuffers(1, &ibo));

    delete[] indices;
    delete[] vertices;
}

void drawMesh() {
    GLCALL(glBindVertexArray(vao));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCALL(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL));
    GLCALL(glBindVertexArray(NULL));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));
}

