#pragma once

#include "openglutility.h"

#include "rendering/Material.h"
#include "rendering/Mesh.h"

namespace carlos {
    class MeshRenderer {
        Mesh& mesh;
        Material& material;

        public:
            void render();
    };
}
