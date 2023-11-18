#pragma once

#include <string>

#include "openglutility.h"

using std::string;

namespace carlos {
    /// @brief Contains 2D, pixel-based image data.
    class Texture {
        private:
            GLuint id;
            int width;
            int height;
            unsigned char* imageData;

            Texture(GLuint id, int width, int height, unsigned char* imageData) :
                id(id),
                width(width),
                height(height),
                imageData(imageData) { }
        public:
            /// @brief Loads a texture from an image file.
            /// @param filePath The file path of the image file.
            /// @return A pointer to the newly-loaded Texture, or nullptr if it was unable to load.
            static Texture* loadFrom(const string& filePath);
            ~Texture();

            GLuint getId() { return id; }
            int getWidth() { return width; }
            int getHeight() { return height; }
            unsigned char* getImageData() { return imageData; }
    };
}
