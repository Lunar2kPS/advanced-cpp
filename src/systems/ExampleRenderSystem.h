#pragma once

#include "openglutility.h"
#include "interfaces/IGameLoopSystem.h"
#include "rendering/Mesh.h"
#include "rendering/Material.h"

using std::string;

namespace carlos {
    class ExampleRenderSystem : public IGameLoopSystem {
        private:
            bool isAbleToDraw = true;
            Mesh* mesh;
            Shader* shader;
            Material* material;
            Texture* mainTexture;
        public:
            ExampleRenderSystem();
            ~ExampleRenderSystem();
            void render();
    };
}
