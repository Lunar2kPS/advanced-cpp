#pragma once

#include "glmutility.h"
#include "interfaces/IGameLoopSystem.h"

namespace carlos {
    class CameraSystem : public IGameLoopSystem {
        private:
            //NOTE: mat4() is all zeroes.
            //      mat4(1) is the identity matrix, which is equivalent to:
            //      mat4(
            //          1, 0, 0, 0,
            //          0, 1, 0, 0,
            //          0, 0, 1, 0,
            //          0, 0, 0, 1
            //      )
            mat4 proj = mat4(1);
            mat4 view = mat4(1);
            int previousWidth = -1;
            int previousHeight = -1;
            int pxScale = 4;
        public:
            CameraSystem() { }
            ~CameraSystem() { }

            int getOrder() { return -1100; }
            
            mat4 getMVP(mat4 model) {
                //NOTE: The multiplication appears as P V M because GLM adheres to column-major ordering in memory layout,
                //  as opposed to row-major ordering. OpenGL uses column-major, so when setting uniforms,
                //  because GLM adheres to the same conventions as OpenGL, no transposition is necessary.
                return proj * view * model;
            }

            void render();
    };
}
