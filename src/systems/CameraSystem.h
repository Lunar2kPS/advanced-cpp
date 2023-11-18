#pragma once

#include "glmutility.h"
#include "interfaces/IGameLoopSystem.h"

namespace carlos {
    /// @brief Represents one camera, for the purposes of the MVP matrix in rendering. In other words, the transform matrix given by this system is currently the main transform that defines how the entire game engine renders, for now, for simplicity.
    /// @note - The projection (P) matrix used is an orthographic matrix, based on the program's window width and height (in pixels). In other words, at a given pixel scale (1x, 2x, 3x, 4x, etc.), the size of the window in pixels determines how large the camera's viewing volume is.
    /// @note - The default pixel scale is 4, which lets us render pixel art at a larger scale for ease of the player.
    /// @note - The P (proj) matrix is updated during the render callback.
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
            int pixelScale = DEFAULT_PIXEL_SCALE;
        public:
            static const int DEFAULT_PIXEL_SCALE = 4;

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
