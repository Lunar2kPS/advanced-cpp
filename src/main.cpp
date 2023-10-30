#include <iostream>
#include <future>
#include <thread>
#include <string>
#include <vector>

//WARNING: Somehow, one of these files probably includes Windows OS headers...
//      #include <Windows.h> NEEDS to be included FIRST before glfw!
//      Or else there will be macro redefinition of APIENTRY.
//      TODO: Have better structure in this entire program to avoid this better.. but just noting for now.
#include "basicnethosting.h"

#include "ServiceLocator.h"
#include "interfaces/IGameLoopSystem.h"
#include "systems/WindowSystem.h"
#include "systems/TimeSystem.h"
#include "systems/ExampleRenderSystem.h"
#include "systems/GUISystem.h"

using std::wcout;
using std::cout;
using std::endl;

//TODO: Not sure how to just reduce the need for "std::" here, without "using namespace std;" -- I don't want to use the entire std namespace!
//Neither of these worked:
// using std::this_thread = this_thread;
// using this_thread = std::this_thread;

using std::this_thread::sleep_for;
using std::future;
using std::async;
using std::vector;

using namespace carlos;

string_t path;

#if defined(WINDOWS)
int __cdecl wmain(int argCount, wchar_t** args) {
#else
int main(int argCount, char** args) {
#endif
    path = getCurrentDirectory(argCount, args);

    ServiceLocator::createInstance();
    ServiceLocator* locator = ServiceLocator::getInstance();
    IWindowSystem* windowing = new WindowSystem();

    //TODO: Clean up to keep this initialization order in sync with system ordering (ISystem.getOrder())
    locator->addSystem<IWindowSystem>(windowing);

    Window* mainWindow;
    if (!windowing->tryCreateWindow("Advanced C++", 800, 600, mainWindow)) {
        fprintf(stderr, "%s\n", "Failed to create main window.");
        return 1;
    }
    
    locator->addSystem<TimeSystem>(new TimeSystem());
    locator->addSystem<ExampleRenderSystem>(new ExampleRenderSystem());
    // locator->addSystem<GUISystem>(new GUISystem());

    vector<IGameLoopSystem*> systems = { };

    while (windowing->anyWindowOpen()) {
        locator->getSystems(systems, SortMode::BY_ORDER);
        for (IGameLoopSystem* s : systems)
            s->earlyUpdate();
        for (IGameLoopSystem* s : systems) {
            s->update();

            //TODO: Clean this up from being hard-coded/dependent on WindowSystem.cpp implementing their check during update()
            if (!windowing->anyWindowOpen())
                goto Exit;
        }

        for (IGameLoopSystem* s : systems)
            s->render();
        for (IGameLoopSystem* s : systems)
            s->postRender();
    }

    Exit:
    locator->getSystems(systems, SortMode::BY_REVERSE_ORDER);
    for (IGameLoopSystem* s : systems)
        delete s;
    ServiceLocator::destroyInstance();
    return 0;
}
