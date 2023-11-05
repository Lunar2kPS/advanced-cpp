#pragma once

#include <iostream>
#include <vector>
#include <sstream>

#include "ServiceLocator.h"
#include "interfaces/IGameLoopSystem.h"
#include "interfaces/IInputSystem.h"
#include "components/MeshRenderer.h"
#include "components/Transform.h"
#include "GameObject.h"

using std::cout;
using std::endl;
using std::vector;
using std::stringstream;

namespace carlos {
    class GameObjectTester : public IGameLoopSystem {
        private:
            Mesh* mesh;
            Shader* shader;
            Material* material;
            Texture* mainTexture;

            void createRenderingObjects() {
                int indexCount = 6;
                int vertexCount = 4;
                int vertexAttributeDimensionCount = 2 + 2; //xy positions, uv texcoords
                int vertexValueCount = vertexCount * vertexAttributeDimensionCount;

                mesh = new Mesh(
                    vertexCount,
                    vertexCount * vertexAttributeDimensionCount,
                    new float[vertexValueCount] {
                        16,   0,  1,  0,
                        0,   0,  0,  0,
                        16,  16,  1,  1,
                        0,  16,  0,  1
                    },
                    indexCount,
                    new GLuint[indexCount] {
                        0, 1, 2,
                        3, 2, 1
                    },
                    2,
                    vector<int> {
                        2, 2
                    }
                );

                shader = Shader::compileFrom("resources/Unlit Texture.glsl");
                material = new Material(*shader);
                material->setColor("color", 1, 1, 1, 1);

                mainTexture = Texture::loadFrom("resources/Grass Tile.png");
                material->setTexture("mainTexture", mainTexture);
            }
        
        public:
            GameObjectTester() {
                SceneSystem* scenes = DefaultServiceLocator::getSystem<SceneSystem>();
                if (scenes == nullptr)
                    return;

                createRenderingObjects();

                stringstream ss = { };
                for (int y = 0; y < 4; y++) {
                    for (int x = 0; x < 10; x++) {
                        ss << "G (" << x << ", " << y << ")";
                        GameObject* current = new GameObject(ss.str());
                        scenes->add(current);

                        ss.str(""); //NOTE: Sets the entire contents back to the empty string
                        ss.clear(); //NOTE: Only clears error flags

                        Transform* transform = current->addComponent<Transform>();
                        transform->setLocalPosition(vec2(16 * x, 16 * y));
                        MeshRenderer* renderer = current->addComponent<MeshRenderer>();

                        renderer->setMesh(mesh);
                        renderer->setMaterial(material);
                    }
                }
            }

            ~GameObjectTester() {
                delete mesh;
                delete shader;
                delete material;
                delete mainTexture;
            }

            void update() {
                IInputSystem* input = DefaultServiceLocator::getSystem<IInputSystem>();
                if (input != nullptr) {
                    //INSTEAD OF THIS:
                    // if (glfwGetKey(static_cast<GLFWwindow*>(windows[0]->getPtr()), GLFW_KEY_0) == GLFW_PRESS)
                    //     cout << "0 is down!" << endl;
                    // if (glfwGetKey(static_cast<GLFWwindow*>(windows[0]->getPtr()), GLFW_KEY_KP_0) == GLFW_PRESS)
                    //     cout << "Numpad 0 is down!" << endl;

                    //NOW I CAN DO THIS!!
                    if (input->getKeyDown(Key::ALPHA0))
                        cout << "0 is down!" << endl;
                    if (input->getKeyDown(Key::NUMPAD0))
                        cout << "Numpad 0 is down!" << endl;
                }
            }
    };
}
