#pragma once

#include <string>

#include "openglutility.h"

using std::string;

namespace carlos {
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
            static Texture* loadFrom(const string& filePath);
            ~Texture();

            GLuint getId() { return id; }
            int getWidth() { return width; }
            int getHeight() { return height; }
            unsigned char* getImageData() { return imageData; }
    };
}
