#include "systems/GUI.h"

#include "openglutility.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GraphicsAPI.h"
#include "ServiceLocator.h"
#include "interfaces/IWindowSystem.h"

using std::cout;
using std::endl;
using std::string;

namespace carlos {
    void setStyle() {
        ImGuiStyle& style = ImGui::GetStyle();

        // style.WindowMinSize = ImVec2 { 200, 100 };
        // style.FrameRounding = 4;
        // style.WindowRounding = 4;

        //NOTE: Huge thanks to: https://github.com/ocornut/imgui/issues/707#issuecomment-678611331
        style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
        style.Colors[ImGuiCol_Button]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_Separator]             = style.Colors[ImGuiCol_Border];
        style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
        style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
        style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_Tab]                   = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
        style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
        style.Colors[ImGuiCol_TabActive]             = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
        style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
        style.Colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        style.GrabRounding                           = style.FrameRounding = 2.3f;
        style.WindowRounding = style.GrabRounding;
    }

    int onNameChanged(ImGuiInputTextCallbackData* data) {
        cout << "!" << endl;

        return data->BufTextLen;
    }


    GUI::GUI() {
        initialized = false;
        ServiceLocator* locator = ServiceLocator::getInstance();
        IWindowSystem* windowing = locator->getSystem<IWindowSystem>();

        if (windowing == nullptr)
            return;

        Window* mainWindow = windowing->getMainWindow();
        if (mainWindow == nullptr)
            return;

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
        ImGui::StyleColorsDark();

        //NOTE: The second param install_callback = true will install GLFW callbacks and chain to existing ones.
        //WARNING: Keep in mind that with static_cast, the compiler doesn't perform any runtime type-checking,
        //      so it's your responsibility to ensure that the cast is valid.
        //      If the types are unrelated, using static_cast might lead to undefined behavior.
        GLFWwindow* windowPtr = static_cast<GLFWwindow*>(mainWindow->getPtr());
        ImGui_ImplGlfw_InitForOpenGL(windowPtr, true);

        GraphicsAPI api = mainWindow->getGraphicsAPI();
        if (api == GraphicsAPI::OPENGL)
            ImGui_ImplOpenGL3_Init();
        else if (api == GraphicsAPI::OPENGL_ES)
            ImGui_ImplOpenGL3_Init("#version 300 es");

        initialized = true;

        nameBuffer = new char[256];
        strcpy(nameBuffer, "");
        
        customFont = io.Fonts->AddFontFromFileTTF("resources/Fonts/Arimo/Arimo.ttf", 14);
        
        //NOTE: Doesn't work at all:
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        
        setStyle();
    }

    GUI::~GUI() {
        if (initialized) {
            delete[] nameBuffer;

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
    }

    void GUI::onGUI() {
        ImGui::ShowDemoWindow();

        // ImGui::PushFont(customFont);
        // ImGui::Begin("Inspector", isOpen, 0);
        // windowPos = ImGui::GetWindowPos();
        // windowSize = ImGui::GetWindowSize();

        // if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        //     if (windowPos.x < 0) {
        //         windowPos.x = 0;
        //         ImGui::SetWindowPos(windowPos);
        //     }
        // }

        // // if (ImGui::Button("Decrease")) {
        // // }
        // // ImGui::SameLine();
        // // if (ImGui::Button("Increase")) {
        // // }

        // ImGui::AlignTextToFramePadding();
        // ImGui::Text("Name");
        // ImGui::SameLine();
        // ImGui::InputText("", nameBuffer, 256, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CallbackEdit, onNameChanged);
        // // cout << (ImGui::IsItemActive() ? "true" : "false") << endl;
        // ImGui::AlignTextToFramePadding();

        // float pos[3];
        // ImGui::AlignTextToFramePadding();
        // ImGui::Text("Position");
        // ImGui::SameLine();
        // ImGui::InputFloat3("", pos, "%.2f");

        // ImGui::End();
        // ImGui::PopFont();
    }

    void GUI::update() { }

    void GUI::render() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        onGUI();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //TODO: Dear ImGUI leaks a 1280 (0x500) GL_INVALID_ENUM error during drawing!
        //      So far, I've only confirmed it on 64bit Raspberry Pi (OpenGL ES 3.1),
        //      but should confirm whether or not the error appears on any other platform.
        glClearAllErrors();
    }
}