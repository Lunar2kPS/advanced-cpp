#include "rendering/Texture.h"

//TODO: Understand FULLY why I had to move this #include into Texture.cpp...
//      Why didn't it work with #pragma once in Texture.h?
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace carlos {
    Texture* Texture::loadFrom(const string& filePath) {
        stbi_set_flip_vertically_on_load(1);

        int width;
        int height;
        int channelCount;
        const char* filePathCStr = filePath.c_str();
        unsigned char* imageData = stbi_load(filePathCStr, &width, &height, &channelCount, STBI_rgb_alpha);

        if (imageData == nullptr)
            return nullptr;

        GLuint texId;
        GLCALL(glGenTextures(1, &texId));

        return new Texture(texId, width, height, imageData);
    }

    Texture::~Texture() {
        GLCALL(glDeleteTextures(1, &id));
        stbi_image_free(imageData);
    }
}
