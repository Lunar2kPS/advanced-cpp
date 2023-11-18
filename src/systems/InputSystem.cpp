#include "systems/InputSystem.h"

#include <iostream>

#include "openglutility.h"
#include "ServiceLocator.h"

using std::cout;
using std::endl;

namespace carlos {
    //NOTE: See these for GLFW-based input handling:
    //  - https://www.glfw.org/docs/3.3/input_guide.html
    //  - https://www.glfw.org/docs/3.3/group__keys.html

    //NOTE: For later when using input callbacks:
    // glfwSetKeyCallback(window, keyCallback);

    // void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers) {
    //     switch (key) {
    //         case GLFW_KEY_0:
    //             if (action == GLFW_PRESS) {
    //                 future task = async(std::launch::async, []() {
    //                     carlos::runManagedCode(path);
    //                 });
    //             }
    //             break;
    //     }
    // }

    //TODO: (Eventually) C++ 20 Upgrade!
    //We should be able to use a range-based for loop with enum_values to iterate over all enum values instead of re-hardcoding them all!
    // for (const auto& key : std::ranges::enum_values<carlos::Key>) {
    //     std::cout << static_cast<int>(key) << std::endl;
    // }

    InputSystem::InputSystem() {
        windowing = DefaultServiceLocator::getSystem<IWindowSystem>();
        keyStates = new KeyState[KEY_COUNT] {
            KeyState(Key::SPACE),
            KeyState(Key::APOSTROPHE),
            KeyState(Key::COMMA),
            KeyState(Key::MINUS),
            KeyState(Key::PERIOD),
            KeyState(Key::SLASH),
            KeyState(Key::ALPHA0),
            KeyState(Key::ALPHA1),
            KeyState(Key::ALPHA2),
            KeyState(Key::ALPHA3),
            KeyState(Key::ALPHA4),
            KeyState(Key::ALPHA5),
            KeyState(Key::ALPHA6),
            KeyState(Key::ALPHA7),
            KeyState(Key::ALPHA8),
            KeyState(Key::ALPHA9),
            KeyState(Key::SEMICOLON),
            KeyState(Key::EQUAL),
            KeyState(Key::A),
            KeyState(Key::B),
            KeyState(Key::C),
            KeyState(Key::D),
            KeyState(Key::E),
            KeyState(Key::F),
            KeyState(Key::G),
            KeyState(Key::H),
            KeyState(Key::I),
            KeyState(Key::J),
            KeyState(Key::K),
            KeyState(Key::L),
            KeyState(Key::M),
            KeyState(Key::N),
            KeyState(Key::O),
            KeyState(Key::P),
            KeyState(Key::Q),
            KeyState(Key::R),
            KeyState(Key::S),
            KeyState(Key::T),
            KeyState(Key::U),
            KeyState(Key::V),
            KeyState(Key::W),
            KeyState(Key::X),
            KeyState(Key::Y),
            KeyState(Key::Z),
            KeyState(Key::LEFT_BRACKET),
            KeyState(Key::BACKSLASH),
            KeyState(Key::RIGHT_BRACKET),
            KeyState(Key::GRAVE_ACCENT),
            // KeyState(Key::WORLD_1),
            // KeyState(Key::WORLD_2),
            KeyState(Key::ESCAPE),
            KeyState(Key::ENTER),
            KeyState(Key::TAB),
            KeyState(Key::BACKSPACE),
            KeyState(Key::INSERT),
            KeyState(Key::DELETE_KEY),
            KeyState(Key::RIGHT_ARROW),
            KeyState(Key::LEFT_ARROW),
            KeyState(Key::DOWN_ARROW),
            KeyState(Key::UP_ARROW),
            KeyState(Key::PAGE_UP),
            KeyState(Key::PAGE_DOWN),
            KeyState(Key::HOME),
            KeyState(Key::END),
            KeyState(Key::CAPS_LOCK),
            KeyState(Key::SCROLL_LOCK),
            KeyState(Key::NUM_LOCK),
            KeyState(Key::PRINT_SCREEN),
            KeyState(Key::PAUSE),
            KeyState(Key::F1),
            KeyState(Key::F2),
            KeyState(Key::F3),
            KeyState(Key::F4),
            KeyState(Key::F5),
            KeyState(Key::F6),
            KeyState(Key::F7),
            KeyState(Key::F8),
            KeyState(Key::F9),
            KeyState(Key::F10),
            KeyState(Key::F11),
            KeyState(Key::F12),
            KeyState(Key::F13),
            KeyState(Key::F14),
            KeyState(Key::F15),
            KeyState(Key::F16),
            KeyState(Key::F17),
            KeyState(Key::F18),
            KeyState(Key::F19),
            KeyState(Key::F20),
            KeyState(Key::F21),
            KeyState(Key::F22),
            KeyState(Key::F23),
            KeyState(Key::F24),
            KeyState(Key::F25),
            KeyState(Key::NUMPAD0),
            KeyState(Key::NUMPAD1),
            KeyState(Key::NUMPAD2),
            KeyState(Key::NUMPAD3),
            KeyState(Key::NUMPAD4),
            KeyState(Key::NUMPAD5),
            KeyState(Key::NUMPAD6),
            KeyState(Key::NUMPAD7),
            KeyState(Key::NUMPAD8),
            KeyState(Key::NUMPAD9),
            KeyState(Key::NUMPAD_DECIMAL),
            KeyState(Key::NUMPAD_DIVIDE),
            KeyState(Key::NUMPAD_MULTIPLY),
            KeyState(Key::NUMPAD_SUBTRACT),
            KeyState(Key::NUMPAD_ADD),
            KeyState(Key::NUMPAD_ENTER),
            KeyState(Key::NUMPAD_EQUALS),
            KeyState(Key::LEFT_SHIFT),
            KeyState(Key::LEFT_CONTROL),
            KeyState(Key::LEFT_ALT),
            KeyState(Key::LEFT_SUPER),
            KeyState(Key::RIGHT_SHIFT),
            KeyState(Key::RIGHT_CONTROL),
            KeyState(Key::RIGHT_ALT),
            KeyState(Key::RIGHT_SUPER),
            KeyState(Key::MENU)
        };

        keyLookup = { };
        for (int i = 0; i < KEY_COUNT; i++)
            keyLookup[keyStates[i].key] = i;
    }

    InputSystem::~InputSystem() {
        delete[] keyStates;
    }

    bool InputSystem::getKeyDown(Key key) {
        unordered_map<Key, int>::iterator result = keyLookup.find(key);
        if (result == keyLookup.end()) {
            cout << "Unable to find key: " << (int) key << endl;
            return false;
        }

        bool isPressed = getKey(key);
        bool wasPressed = keyStates[result->second].wasPressed;
        return isPressed && !wasPressed;
    }

    bool InputSystem::getKey(Key key) {
        GLFWwindow* mainWindowPtr = static_cast<GLFWwindow*>(windowing->getMainWindow()->getPtr());
        return glfwGetKey(mainWindowPtr, (int) key) == GLFW_PRESS;
    }

    bool InputSystem::getKeyUp(Key key) {
        GLFWwindow* mainWindowPtr = static_cast<GLFWwindow*>(windowing->getMainWindow()->getPtr());
        return glfwGetKey(mainWindowPtr, (int) key) == GLFW_RELEASE;
    }

    void InputSystem::saveKeySnapshot() {
        GLFWwindow* mainWindowPtr = static_cast<GLFWwindow*>(windowing->getMainWindow()->getPtr());
        for (int i = 0; i < KEY_COUNT; i++)
            keyStates[i].wasPressed = glfwGetKey(mainWindowPtr, (int) keyStates[i].key);
    }

    void InputSystem::postRender() {
        saveKeySnapshot();
    }
}
