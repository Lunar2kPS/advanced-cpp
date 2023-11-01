#include "MeshRenderer.h"

namespace carlos {
    void MeshRenderer::render() {
        mesh->bind();
        mesh->renderDirect();
        // mesh->unbind();
    }
}
