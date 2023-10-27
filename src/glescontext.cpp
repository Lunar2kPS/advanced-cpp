#include "glad/gles2.h"
#include "GLFW/glfw3.h"

int foo2() {
    return gladLoadGLES2(glfwGetProcAddress);
}
