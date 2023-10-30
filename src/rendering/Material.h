#pragma once

#include <string>
#include <unordered_map>

#include "openglutility.h"
#include "includers/glm.h"
#include "rendering/Shader.h"
#include "rendering/Texture.h"

using std::unordered_map;
using std::string;
using std::pair;
using std::move; //NOTE: May require #include <utility> on its own.

namespace carlos {
    class Material {
        private:
            template <typename V>
            struct UniformValue {
                string propertyName;
                GLint propertyId;
                V value;
            };

            Shader& shader;
        
            unordered_map<string, UniformValue<float>> floatProperties = { };
            unordered_map<string, UniformValue<vec4>> vectorProperties = { };
            unordered_map<string, UniformValue<mat4>> matrixProperties = { };
            unordered_map<string, UniformValue<Texture*>> textureProperties = { };
        public:
            Material(Shader& shader) : shader(shader) { }

            void setFloat(const string& name, float value);
            void setVector(const string& name, vec4 value);
            void setMatrix(const string& name, mat4 value);
            void setTexture(const string& name, Texture* value);

            void setColor(const string& name, vec4 value) { setVector(name, value); }
            void setColor(const string& name, float r, float g, float b, float a) { setVector(name, vec4(r, g, b, a)); }

            void bind();
    };
}
