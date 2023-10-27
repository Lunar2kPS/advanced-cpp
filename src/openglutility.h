#pragma once

#include <iostream>
#include "glad/gl.h"

using std::cout;
using std::endl;
using std::hex;
using std::dec;

//TODO: Why? https://stackoverflow.com/questions/1787822/why-arent-include-guards-or-pragma-once-working
//We're forced to NOT implement these here in the header.. but I'm not sure really why it matters
//  whether or not I implement them here in the .h or in a corresponding .cpp file...

/// @brief Silently clears all current OpenGL errors
void glClearAllErrors();

/// @brief Logs all pending OpenGL errors.
/// @return The number of errors logged, or 0 if there were no errors.
int glLogAllErrors();

void glClearPreviousErrorFlag();
bool glHadPreviousErrors();

/// @brief Wraps OpenGL calls with error-logging.
/// @param innerCall The OpenGL call, such as glClear, glGenVertexArrays, glDrawElements, etc.
/// @note See: [OpenGL error codes](https://www.khronos.org/opengl/wiki/OpenGL_Error)
#define GLCALL(innerCall) \
    innerCall;\
    if (glLogAllErrors() > 0)\
        cout << "    from:  " << #innerCall << "\n" << endl;
