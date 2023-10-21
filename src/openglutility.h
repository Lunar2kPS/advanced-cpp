#pragma once

#include <iostream>
#include "glad/gl.h"

using std::cout;
using std::endl;
using std::hex;
using std::dec;

/// @brief Wraps OpenGL calls with error-logging.
/// @param innerCall The OpenGL call, such as glClear, glGenVertexArrays, glDrawElements, etc.
/// @note See: [OpenGL error codes](https://www.khronos.org/opengl/wiki/OpenGL_Error)
#define GLCALL(innerCall) glClearAllErrors();\
    innerCall;\
    if (glLogAllErrors() > 0)\
        cout << "    from:  " << #innerCall << "\n" << endl;

void glClearAllErrors() {
    while (glGetError() != GL_NO_ERROR);
}

/// @brief Logs all pending OpenGL errors.
/// @return The number of errors logged, or 0 if there were no errors.
int glLogAllErrors() {
    GLenum error;
    int count = 0;
    while ((error = glGetError()) != GL_NO_ERROR) {
        cout << "[OpenGL Error] (" << error << " => 0x" << hex << error << dec << ")" << endl;
        count++;
    }
    return count;
}
