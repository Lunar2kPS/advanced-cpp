#include "MeshRenderer.h"

#include "glmutility.h"
#include "ServiceLocator.h"
#include "interfaces/IWindowSystem.h"
#include "GameObject.h"
#include "components/Transform.h"

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
