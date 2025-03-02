#include <iostream>
#include <thread>
#include <chrono>
#include "CoroutineSystem.h"

using std::cout;
using std::endl;

using namespace carlos;
namespace this_thread = std::this_thread;
namespace chrono = std::chrono;

void example(Coroutine& self) {
    cout << "Started!" << endl;
    self.waitForSeconds(2, nullptr, [](Coroutine& self, void* userData) {
        cout << "2sec later..." << endl;
        self.waitForSeconds(1, nullptr, [](Coroutine& self, void* userData) {
            cout << "1sec passed after that..." << endl;
        });
    });
}

int main() {
    cout << "Game Coroutines!" << endl;

    CoroutineSystem system = { };
    system.startCoroutine(example);

    float dt = 0.1f;
    int dtMS = (int) (1000 * dt);
    float time = 0;

    while (time < 5) {
        cout << "t = " << time << endl;
        system.update(dt);
        time += dt;
        this_thread::sleep_for(chrono::milliseconds(dtMS));
    }

    return 1;
}
