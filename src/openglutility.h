#pragma once

#include <iostream>
#include "glad/gl.h"

using std::cout;
using std::endl;

/// @brief Wraps OpenGL calls with error-logging.
/// @param innerCall The OpenGL call, such as glClear, glGenVertexArrays, glDrawElements, etc.
#define GLCALL(innerCall) glClearAllErrors();\
    innerCall;\
    glLogAllErrors();

void glClearAllErrors() {
    while (glGetError() != GL_NO_ERROR);
}

/// @brief Logs all pending OpenGL errors.
/// @return The number of errors logged, or 0 if there were no errors.
int glLogAllErrors() {
    GLenum error;
    int count = 0;
    while ((error = glGetError()) != GL_NO_ERROR) {
        cout << "[OpenGL Error] (" << error << ")" << endl;
        count++;
    }
    return count;
}
