#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>

//WARNING: Somehow, one of these files probably includes Windows OS headers...
//      #include <Windows.h> NEEDS to be included FIRST before glfw!
//      Or else there will be macro redefinition of APIENTRY.
//      TODO: Have better structure in this entire program to avoid this better.. but just noting for now.
#include "tests/json.h"
#include "tests/entities.h"
#include "basicnethosting.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glm/glm.hpp"

using std::wcout;
using std::cout;
using std::endl;

//TODO: Not sure how to just reduce the need for "std::" here, without "using namespace std;" -- I don't want to use the entire std namespace!
//Neither of these worked:
// using std::this_thread = this_thread;
// using this_thread = std::this_thread;

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::stringstream;
using glm::vec3;

void runImgui();
int tryCreateWindow(const char* title, int width, int height, GLFWwindow*& window);

#if defined(WINDOWS)
int __cdecl wmain(int argCount, wchar_t** args) {
#else
int main(int argCount, char** args) {
#endif
    cout << "Hello world!" << endl;
    testJSON();
    testECS();

    //Get the current executable's directory
    //This assumes the managed assembly to load and its runtime configuration file are next to the host
    char_t rootPath[MAX_PATH];
    #if defined(WINDOWS)
        int size = GetFullPathNameW(args[0], sizeof(rootPath) / sizeof(char_t), rootPath, nullptr);
        if (size <= 0)
            return 1;
    #else
        char* result = realpath(args[0], rootPath);
        if (result == nullptr)
            return 1;
    #endif
    cout << "rootPath = " << rootPath << endl;

    string_t rootPathStr = rootPath;
    for (size_t i = 0; i < rootPathStr.length(); i++) {
        if (rootPathStr[i] == '\\')
            rootPathStr[i] = '/';
    }

    int index = rootPathStr.find_last_of('/');
    rootPathStr = rootPathStr.substr(0, index);

    carlos::runManagedCode(rootPathStr);

    runImgui();

    return 0;
}

void runImgui() {
    GLFWwindow* window;
    int initError = tryCreateWindow("Advanced C++", 800, 600, window);
    if (initError != 0) {
        fprintf(stderr, "%s%d\n", "Exiting with initialization exit code ", initError);
        return; //initError;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // while (true) {
    // for (int i = 0; i < 1000; i++) {
    //     ImGui::Render();
    //     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //     sleep_for(std::chrono::milliseconds(30));
    // }
    
    int windowWidth;
    int windowHeight;
    float timeLastSwitched = 0;
    float prevTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)

        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        float time = glfwGetTime();
        float dt = time - prevTime;

        vec3 input = vec3(0);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            input.x--;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            input.x++;
            
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            input.y--;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            input.y++;

        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            input.z--;
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            input.z++;

        if (glfwGetKey(window, GLFW_KEY_0) && time - timeLastSwitched > 1) {
            // moveTarget = (MovementMode) ((int) moveTarget + 1);
            // if ((int) moveTarget > 1)
            //     moveTarget = (MovementMode) 0;
            // timeLastSwitched = time;
            // printf("moveTarget = %d\n", moveTarget);
        }

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

int tryCreateWindow(const char* title, int width, int height, GLFWwindow*& window) {
    if (!glfwInitialized) {
        if (!glfwInit()) {
            fprintf(stderr, "GLFW initialization failed!\n");
            return 1;
        }

        //NOTE: Let's require a certain (old) version of OpenGL or newer...
        //Like OpenGL 3.0+. HOWEVER,
        //NOTE: Context profiles are only available in OpenGL 3.2+, so we'll require that!
        //TODO: Try to require 4.6, then if we get the error (during callback) of "Requested OpenGL version 4.6, got version 4.1", then request that version instead!
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        //NOTE: BEFORE doing this, I was getting the following results:
        //  Windows:        OpenGL 4.6          ==> NOW OpenGL 3.2
        //  MacOS:          OpenGL 2.1          ==> NOW OpenGL 4.1!
        //  Linux:          OpenGL 4.6          ==> NOW OpenGL 3.2
        //So, we set it to use OpenGL Core profile with forward compatibility: 
        glfwWindowHint(GLFW_OPENGL_PROFILE,             GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,      GL_TRUE);

        glfwSetErrorCallback(errorCallback);
    }

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create window or OpenGL context!\n");
        glfwTerminate();
        return 2;
    }

    if (!glfwInitialized) {
        glfwMakeContextCurrent(window);
        int version = gladLoadGL(glfwGetProcAddress);
        if (version == 0) {
            printf("Failed to initialize OpenGL context with GLAD!\n");
            return 3;
        }
        printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    }
    glfwInitialized = true;
    return 0;
}
