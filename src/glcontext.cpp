#include "glad/gl.h"
// #include "glad/gles2.h"
#include "GLFW/glfw3.h"

int foo1() {
    return gladLoadGL(glfwGetProcAddress);
}
