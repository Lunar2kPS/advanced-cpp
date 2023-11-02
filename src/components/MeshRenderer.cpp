#include "MeshRenderer.h"

namespace carlos {
    void MeshRenderer::render() {
        if (mesh == nullptr || material == nullptr)
            return;
        material->bind();
        mesh->bind();
        mesh->renderDirect();
        // mesh->unbind();
    }
}
