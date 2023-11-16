#pragma once

#include "openglutility.h"
#include "glmutility.h"
#include "components/Component.h"

#include "rendering/Material.h"
#include "rendering/Mesh.h"

namespace carlos {
    /// @brief A component that causes the GameObject to be rendered with a given Mesh and Material. For 2D rendering, a quad Mesh is typically used.
    class MeshRenderer : public Component {
        Mesh* mesh;
        Material* material;

        public:
            MeshRenderer() { }
            
            Mesh* getMesh() { return mesh; }
            Material* getMaterial() { return material; }
            void setMesh(Mesh* mesh) { this->mesh = mesh; }
            void setMaterial(Material* material) { this->material = material; }

            void render();
    };
}
