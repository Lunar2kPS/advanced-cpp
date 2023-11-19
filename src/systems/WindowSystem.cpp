#include "systems/WindowSystem.h"

#include <iostream>
#include <sstream>

#include "openglutility.h"

#include "ServiceLocator.h"
#include "systems/AppSystem.h"

using std::stringstream;

namespace carlos {
    void errorCallback(int errorCode, const char* description) {
        stringstream ss;
        ss << "GLFW Error code: " << errorCode << "\n";
        ss << description;
        ss << "\n";

        const char* message = ss.str().c_str();
        fprintf(stderr, "%s", message);
    }

    void prepareForOpenGL() {
        //NOTE: Let's require a certain (old) version of OpenGL or newer...
        //Like OpenGL 3.0+. HOWEVER,
        //NOTE: Context profiles are only available in OpenGL 3.2+, so we'll require that
        //TODO: Try to require 4.6, then if we get the error (during callback) of "Requested OpenGL version 4.6, got version 4.1", then request that version instead
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        //NOTE: BEFORE doing this, I was getting the following results:
        //  Windows:        OpenGL 4.6          ==> NOW OpenGL 3.2
        //  MacOS:          OpenGL 2.1          ==> NOW OpenGL 4.1!
        //  Linux:          OpenGL 4.6          ==> NOW OpenGL 3.2
        //So, we set it to use OpenGL Core profile with forward compatibility: 
        glfwWindowHint(GLFW_OPENGL_PROFILE,             GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,      GL_TRUE);
    }

    void prepareForOpenGLES() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        glfwWindowHint(GLFW_OPENGL_PROFILE,             GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,      GL_TRUE);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    }

    WindowSystem::WindowSystem() {
        
    }

    WindowSystem::~WindowSystem() {
        for (Window* window : windows) {
            GLFWwindow* ptr = static_cast<GLFWwindow*>(window->getPtr());
            if (ptr != nullptr)
                glfwDestroyWindow(ptr);
            delete window;
        }
        windows.clear();

        glfwTerminate();
    }

    bool WindowSystem::tryCreateWindow(const char* title, int width, int height, Window*& outWindow) {
        GraphicsAPI api = GraphicsAPI::NONE;
        outWindow = nullptr;

        if (!glfwInitialized) {
            if (!glfwInit()) {
                fprintf(stderr, "GLFW initialization failed!\n");
                return false;
            }
            glfwSetErrorCallback(errorCallback);
        }

#if defined(GRAPHICS_API_GL)
        api = GraphicsAPI::OPENGL;
        prepareForOpenGL();
#elif defined(GRAPHICS_API_GLES)
        api = GraphicsAPI::OPENGL_ES;
        prepareForOpenGLES();
#endif
        GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

        if (window == nullptr) {
            api = GraphicsAPI::NONE;
            fprintf(stderr, "Failed to create window or OpenGL/OpenGL ES context!\n");
            glfwTerminate();
            return false;
        }


        if (!glfwInitialized) {
            glfwMakeContextCurrent(window);
             int version =
#if defined(GRAPHICS_API_GL)
                gladLoadGL(glfwGetProcAddress);
#elif defined(GRAPHICS_API_GLES)
                gladLoadGLES2(glfwGetProcAddress);
#else
                0;
#endif

            if (version == 0) {
                printf("Failed to initialize OpenGL context with GLAD!\n");
                return false;
            }
            //TODO: Clean this up:
#if defined(GRAPHICS_API_GL)
            printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
#elif defined(GRAPHICS_API_GLES)
            printf("Loaded OpenGL ES %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
#endif

            //NOTE: V-Sync: Wait 1 frame before rendering each frame --
            //      don't waste CPU resources trying to render at 2000 FPS when our screens can't even display that fast!
            glfwSwapInterval(0);
        }

        outWindow = new Window(window, api, width, height);
        windows.push_back(outWindow);
        glfwInitialized = true;
        return true;
    }

    void WindowSystem::setTitle(Window& window, const char* title) {
        glfwSetWindowTitle(static_cast<GLFWwindow*>(window.getPtr()), title);
    }

    Window* WindowSystem::getMainWindow() {
        if (windows.size() <= 0)
            return nullptr;
        return windows[0];
    }

    bool WindowSystem::anyWindowOpen() {
        return windows.size() > 0;
    }

    void WindowSystem::earlyUpdate() {
        GLCALL(glClearColor(0, 0, 0, 0));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    void WindowSystem::update() {
        for (int i = windows.size() - 1; i >= 0; i--) {
            GLFWwindow* ptr = static_cast<GLFWwindow*>(windows[i]->getPtr());
            if (glfwWindowShouldClose(ptr)) {
                //WARNING: Destroying the window immediately doesn't give other systems the chance to delete their OpenGL state/data, and then may cause significant errors.
                // glfwDestroyWindow(ptr);
                // delete windows[i];
                // windows.erase(windows.begin() + i);

                //Instead, let's request application quit and let the main program handle the ordering of everything gracefully based on system orders.
                ServiceLocator* locator = ServiceLocator::getInstance();
                AppSystem* app = locator->getSystem<AppSystem>();
                app->requestQuit();
                break;
            }
        }

        // if (windows.size() <= 0)
        //     return;
        glfwPollEvents();

        for (Window* window : windows) {
            int w = window->getWidth();
            int h = window->getHeight();
            glfwGetWindowSize(static_cast<GLFWwindow*>(window->getPtr()), &w, &h);
            window->updateSize(w, h);
        }
    }

    void WindowSystem::render() {
        //TODO: Multi-window support
        if (anyWindowOpen())
            GLCALL(glViewport(0, 0, windows[0]->getWidth(), windows[0]->getHeight()));
    }

    void WindowSystem::postRender() {
        for (Window* window : windows)
            glfwSwapBuffers(static_cast<GLFWwindow*>(window->getPtr()));
    }
}
