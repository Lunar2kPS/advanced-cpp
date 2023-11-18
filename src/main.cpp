#include <iostream>
#include <future>
#include <thread>
#include <string>
#include <vector>
#include <sstream>

//NOTE: basicnethosting.h includes Windows OS headers, and
//      #include <Windows.h> NEEDS to be included FIRST before glfw!
//      Or else there will be macro redefinition of APIENTRY.
#include "basicnethosting.h"
#include "openglutility.h"

#include "ServiceLocator.h"
#include "interfaces/IGameLoopSystem.h"
#include "systems/WindowSystem.h"
#include "systems/TimeSystem.h"
#include "systems/InputSystem.h"
#include "systems/ExampleRenderSystem.h"
#include "systems/GameObjectTester.h"
#include "systems/GUISystem.h"
#include "systems/AppSystem.h"
#include "systems/CameraSystem.h"
#include "systems/SceneSystem.h"
#include "systems/CSharpTestSystem.h"
#include "GameObject.h"
#include "components/Transform.h"
#include "components/MeshRenderer.h"

using std::wcout;
using std::cout;
using std::endl;

using std::vector;
using std::string;
using std::stringstream;

using namespace carlos;

#if defined(WINDOWS)
    //TODO: What was __cdecl about?
    #define MAIN_PROGRAM wmain
#else
    #define MAIN_PROGRAM main
#endif

int MAIN_PROGRAM(int argCount, char_t** args) {
    ServiceLocator::createInstance();
    ServiceLocator* locator = ServiceLocator::getInstance();

    try {
        AppSystem* app = new AppSystem(argCount, args);
        IWindowSystem* windowing = new WindowSystem();
        SceneSystem* scenes = new SceneSystem();

        //TODO: Clean up to keep this initialization order in sync with system ordering (ISystem.getOrder())
        locator->addSystem<AppSystem>(app);
        locator->addSystem<IWindowSystem>(windowing);

        Window* mainWindow;
        if (!windowing->tryCreateWindow("Advanced C++", 800, 600, mainWindow)) {
            fprintf(stderr, "%s\n", "Failed to create main window.");
            return 1;
        }

        locator->addSystem<IInputSystem>(new InputSystem());
        
        locator->addSystem<TimeSystem>(new TimeSystem());
        // locator->addSystem<GUISystem>(new GUISystem());

        locator->addSystem<CameraSystem>(new CameraSystem());
        locator->addSystem<SceneSystem>(scenes);

        // locator->addSystem<ExampleRenderSystem>(new ExampleRenderSystem());
        
        locator->addSystem<GameObjectTester>(new GameObjectTester());
        locator->addSystem<CSharpTestSystem>(new CSharpTestSystem());

        IGameLoopSystem* test = scenes;
        vector<IGameLoopSystem*> systems = { };
        
        stringstream ss = { };
        const int AVG_FRAME_COUNT = 256;
        int index = 0;
        int frameCount = 0;
        float fps[AVG_FRAME_COUNT];

        while (windowing->anyWindowOpen()) {
            locator->getSystems(systems, SortMode::BY_ORDER);
            for (IGameLoopSystem* s : systems)
                s->earlyUpdate();
            for (IGameLoopSystem* s : systems)
                s->update();

            for (IGameLoopSystem* s : systems)
                s->render();
            for (IGameLoopSystem* s : systems)
                s->postRender();

            if (frameCount < AVG_FRAME_COUNT)
                frameCount++;
            fps[index] = 1 / locator->getSystem<TimeSystem>()->deltaTime();
            index = (index + 1) % AVG_FRAME_COUNT;

            float avgFPS = 0;
            for (int i = 0; i < frameCount; i++)
                avgFPS += fps[i];
            avgFPS /= frameCount;

            ss << "Advanced C++" << " (FPS = " << (int) avgFPS << ")";
            windowing->setTitle(*mainWindow, ss.str().c_str());

            ss.str(""); //NOTE: Sets the entire contents back to the empty string
            ss.clear(); //NOTE: Only clears error flags

            if (app->isQuitRequested())
                break;
        }

        locator->getSystems(systems, SortMode::BY_REVERSE_ORDER);
        for (IGameLoopSystem* s : systems)
            delete s;
    } catch (...) {
        cout << "An exception was thrown. TODO: Handle this properly..." << endl;
    }
    ServiceLocator::destroyInstance();
    return 0;
}
