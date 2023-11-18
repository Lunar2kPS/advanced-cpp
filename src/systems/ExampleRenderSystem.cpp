#include "systems/ExampleRenderSystem.h"

#include <iostream>
#include <string>

#include "glmutility.h"
#include "ServiceLocator.h"
#include "interfaces/IWindowSystem.h"
#include "systems/CameraSystem.h"

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

    void ExampleRenderSystem::render() {
        if (isAbleToDraw) {
            CameraSystem* camera = DefaultServiceLocator::getSystem<CameraSystem>();
            mat4 model = mat4(1);
            mat4 mvp = camera->getMVP(model);
            material->setMatrix("mvp", mvp);

            mesh->bind();
            material->bind();
            mesh->renderDirect();

            for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                    material->setMatrix("mvp", mvp);
                    material->bind();
                    mesh->renderDirect();
                }
            }

            if (glHadPreviousErrors()) {
                cout << "Encountered an error! Stopping drawing..." << endl;
                isAbleToDraw = false;
            }
        }
    }
}
