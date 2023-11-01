#pragma once

#include "openglutility.h"
#include "components/Component.h"

#include "rendering/Material.h"
#include "rendering/Mesh.h"

namespace carlos {
    class MeshRenderer : public Component {
        Mesh* mesh;
        Material* material;

        public:
            MeshRenderer() { }
            void render();
    };
}
