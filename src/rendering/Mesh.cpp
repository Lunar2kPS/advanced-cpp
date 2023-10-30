#include "rendering/Mesh.h"

namespace carlos {
    Mesh::Mesh(int vertexCount,
        int vertexValueCount,
        float* vertices,
        int indexCount,
        GLuint* indices,
        int attributeCount,
        vector<int> attributeDimensions
    ) :
        vertexCount(vertexCount),
        vertexValueCount(vertexValueCount),
        vertices(vertices),
        indexCount(indexCount),
        indices(indices),
        attributeCount(attributeCount),
        attributeDimensions(attributeDimensions) {
        
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
        GLsizei totalStride = 0;

        for (int a = 0; a < attributeDimensions.size(); a++)
            totalStride += attributeDimensions[a];
        totalStride *= sizeof(float);

        //NOTE: For now, we're just assuming all vertex attributes will be floats.
        for (int a = 0; a < attributeDimensions.size(); a++) {
            int dimensions = attributeDimensions[a]; //NOTE: Must be 1, 2, 3, or 4.

            GLCALL(glEnableVertexAttribArray(a));
            GLCALL(glVertexAttribPointer(a, dimensions, GL_FLOAT, false, totalStride, (const void*) offset)); //NOTE: This requires a currently-bound GL_ARRAY_BUFFER target (the VBO)
            offset += dimensions * sizeof(float);
        }

        GLCALL(glBindVertexArray(NULL));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, NULL));
    }

    Mesh::~Mesh() {
        GLCALL(glDeleteVertexArrays(1, &vao));
        GLCALL(glDeleteBuffers(1, &vbo));
        GLCALL(glDeleteBuffers(1, &ibo));

        delete[] indices;
        delete[] vertices;
    }

    void Mesh::bind() {
        GLCALL(glBindVertexArray(vao));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    }

    void Mesh::unbind() {
        GLCALL(glBindVertexArray(NULL));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, NULL));
    }

    void Mesh::renderDirect() {
        GLCALL(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL));
    }
}