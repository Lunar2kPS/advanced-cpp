#include <iostream>
#include <future>
#include <thread>
#include <string>
#include <sstream>

//WARNING: Somehow, one of these files probably includes Windows OS headers...
//      #include <Windows.h> NEEDS to be included FIRST before glfw!
//      Or else there will be macro redefinition of APIENTRY.
//      TODO: Have better structure in this entire program to avoid this better.. but just noting for now.
#include "basicnethosting.h"

// #define IMGUI_IMPL_OPENGL_ES3
#include "glad/egl.h"
#include "glad/gles2.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using std::wcout;
using std::cout;
using std::endl;

//TODO: Not sure how to just reduce the need for "std::" here, without "using namespace std;" -- I don't want to use the entire std namespace!
//Neither of these worked:
// using std::this_thread = this_thread;
// using this_thread = std::this_thread;

using std::this_thread::sleep_for;
using std::stringstream;
using std::future;
using std::async;

#define ENUM_TO_STRING(s) #s
enum class GraphicsAPI : int {
    NONE = 0,
    OPENGL,
    OPENGL_ES
};

string_t path;
GraphicsAPI graphicsAPI;

int tryCreateWindow(const char* title, int width, int height, GLFWwindow*& window, GraphicsAPI& api);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);

#if defined(WINDOWS)
int __cdecl wmain(int argCount, wchar_t** args) {
#else
int main(int argCount, char** args) {
#endif
    GLFWwindow* window;
    int initError = tryCreateWindow("Advanced C++", 800, 600, window, graphicsAPI);
    if (initError != 0) {
        fprintf(stderr, "%s%d\n", "Exiting with initialization exit code ", initError);
        return initError;
    }
    //NOTE: V-Sync: Wait 1 frame before rendering each frame --
    //      don't waste CPU resources trying to render at 2000 FPS when our screens can't even display that fast!
    glfwSwapInterval(1);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.

    if (graphicsAPI == GraphicsAPI::OPENGL)
        ImGui_ImplOpenGL3_Init();
    else if (graphicsAPI == GraphicsAPI::OPENGL_ES)
        ImGui_ImplOpenGL3_Init("#version 300 es");
    
    int windowWidth;
    int windowHeight;
    float timeLastSwitched = 0;
    float prevTime = glfwGetTime();
    bool was0PressedLastFrame = false;
    bool was1PressedLastFrame = false;

    path = carlos::getCurrentDirectory(argCount, args);
    glfwSetKeyCallback(window, keyCallback);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)

        float time = glfwGetTime();
        float dt = time - prevTime;
        float instantaneousFPS = 1 / dt;

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        prevTime = time;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers) {
    switch (key) {
        case GLFW_KEY_0:
            if (action == GLFW_PRESS) {
                future task = async(std::launch::async, []() {
                    carlos::runManagedCode(path);
                });
            }
            break;
        case GLFW_KEY_1:
            if (action == GLFW_PRESS) {
                cout << "Attempting to unload hostfxr/nethost... but I don't think this is officially supported, so it won't work!" << endl;
                if (carlos::closeFunction != nullptr)
                    carlos::closeFunction(nullptr);
            }
            break;
    }
}


static bool glfwInitialized = false;
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

int tryCreateWindow(const char* title, int width, int height, GLFWwindow*& window, GraphicsAPI& api) {
    if (!glfwInitialized) {
        api = GraphicsAPI::NONE;
        if (!glfwInit()) {
            fprintf(stderr, "GLFW initialization failed!\n");
            return 1;
        }
        glfwSetErrorCallback(errorCallback);
    }

    prepareForOpenGL();
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window != nullptr) {
        api = GraphicsAPI::OPENGL;
    } else {
        prepareForOpenGLES();
        window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (window != nullptr)
            api = GraphicsAPI::OPENGL_ES;
    }
    if (window == nullptr) {
        fprintf(stderr, "Failed to create window or OpenGL/OpenGL ES context!\n");
        glfwTerminate();
        return 2;
    }

    if (!glfwInitialized) {
        glfwMakeContextCurrent(window);
        int version = gladLoadGLES2(glfwGetProcAddress);
        if (version == 0) {
            printf("Failed to initialize OpenGL context with GLAD!\n");
            return 3;
        }
        //TODO: Clean this up:
        if (api == GraphicsAPI::OPENGL)
            printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
        else if (api == GraphicsAPI::OPENGL_ES)
            printf("Loaded OpenGL ES %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    }
    glfwInitialized = true;
    return 0;
}
