#include "openglutility.h"

void glClearAllErrors() {
    while (glGetError() != GL_NO_ERROR);
}

int glLogAllErrors() {
    GLenum error;
    int count = 0;
    while ((error = glGetError()) != GL_NO_ERROR) {
        cout << "[OpenGL Error] (" << error << " => 0x" << hex << error << dec << ")" << endl;
        count++;
    }
    return count;
}
