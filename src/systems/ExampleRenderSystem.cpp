#include "systems/ExampleRenderSystem.h"

#include <iostream>
#include <string>

#include "includers/glm.h"
#include "glm/gtc/matrix_transform.hpp"
#include "ServiceLocator.h"
#include "interfaces/IWindowSystem.h"

using std::cout;
using std::endl;

namespace carlos {
    ExampleRenderSystem::ExampleRenderSystem() {
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

    ExampleRenderSystem::~ExampleRenderSystem() {
        delete mesh;
        delete shader;
        delete material;
        delete mainTexture;
    }

    void ExampleRenderSystem::update() {

    }

    void ExampleRenderSystem::render() {
        if (isAbleToDraw) {
            IWindowSystem* windowing = ServiceLocator::getInstance()->getSystem<IWindowSystem>();
            Window* mainWindow = nullptr;
            if (windowing != nullptr) {
                mainWindow = windowing->getMainWindow();
                if (mainWindow != nullptr) {
                    //NOTE: mat4() is all zeroes.
                    //      mat4(1) is the identity matrix, which is equivalent to:
                    //      mat4(
                    //          1, 0, 0, 0,
                    //          0, 1, 0, 0,
                    //          0, 0, 1, 0,
                    //          0, 0, 0, 1
                    //      )
                    
                    int pxScale = 4;
                    mat4 proj = glm::ortho<float>(0, mainWindow->getWidth() / pxScale, 0, mainWindow->getHeight() / pxScale, -1, 1);
                    mat4 view = mat4(1);
                    mat4 model = mat4(1);
                    
                    //NOTE: The multiplication appears as P V M because GLM adheres to column-major ordering in memory layout,
                    //  as opposed to row-major ordering. OpenGL uses column-major, so when setting uniforms,
                    //  because GLM adheres to the same conventions as OpenGL, no transposition is necessary.
                    mat4 mvp = proj * view * model;
                    material->setMatrix("mvp", mvp);
                }
            }

            mesh->bind();
            material->bind();
            mesh->renderDirect();

            if (mainWindow != nullptr) {
                for (int y = 0; y < 5; y++) {
                    for (int x = 0; x < 5; x++) {
                        int pxScale = 4;
                        mat4 proj = glm::ortho<float>(0, mainWindow->getWidth() / pxScale, 0, mainWindow->getHeight() / pxScale, -1, 1);
                        mat4 view = mat4(1);
                        mat4 model = glm::translate(mat4(1), vec3(16 * x, 16 * y, 0));
                        
                        mat4 mvp = proj * view * model;
                        material->setMatrix("mvp", mvp);
                        material->bind();
                        mesh->renderDirect();
                    }
                }
            }

            if (glHadPreviousErrors()) {
                cout << "Encountered an error! Stopping drawing..." << endl;
                isAbleToDraw = false;
            }
        }
    }
}
