#include "openglutility.h"

static bool errorDirtyFlag;

void glClearAllErrors() {
    while (glGetError() != GL_NO_ERROR)
        errorDirtyFlag = true;
}

int glLogAllErrors() {
    GLenum error;
    int count = 0;
    while ((error = glGetError()) != GL_NO_ERROR) {
        cout << "[OpenGL Error] (" << error << " => 0x" << hex << error << dec << ")" << endl;
        count++;
    }
    if (count > 0)
        errorDirtyFlag = true;
    return count;
}

void glClearPreviousErrorFlag() {
    errorDirtyFlag = false;
}
bool glHadPreviousErrors() {
    bool previous = errorDirtyFlag;
    errorDirtyFlag = false;
    return previous;
}