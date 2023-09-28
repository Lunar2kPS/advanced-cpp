#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "tests/json.h"
#include "tests/entities.h"
#include "basicnethosting.h"

using std::wcout;
using std::cout;
using std::endl;

//TODO: Not sure how to just reduce the need for "std::" here, without "using namespace std;" -- I don't want to use the entire std namespace!
//Neither of these worked:
// using std::this_thread = this_thread;
// using this_thread = std::this_thread;

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

void runImgui();

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
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    while (true) {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        sleep_for(std::chrono::milliseconds(30));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
