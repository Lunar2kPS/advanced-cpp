#include "rendering/Material.h"

namespace carlos {
    void Material::setFloat(const string& name, float value) {
        GLCALL(GLint propertyId = glGetUniformLocation(shader.getId(), name.c_str()));
        floatProperties[name] = UniformValue<float> {
            name,
            propertyId,
            value
        };
    }

    void Material::setVector(const string& name, vec4 value) {
        GLCALL(GLint propertyId = glGetUniformLocation(shader.getId(), name.c_str()));
        vectorProperties[name] = UniformValue<vec4> {
            name,
            propertyId,
            value
        };
    }

    void Material::setMatrix(const string& name, mat4 value) {
        GLCALL(GLint propertyId = glGetUniformLocation(shader.getId(), name.c_str()));
        matrixProperties[name] = UniformValue<mat4> {
            name,
            propertyId,
            value
        };
    }

    void Material::setTexture(const string& name, Texture* value) {
        //TODO: Learn about copying, moving, and unordered_maps' inability to move references (&)...
        //  gotta use Texture* instead of Texture& here or else weird error about "deleted function" ???
        GLCALL(GLint propertyId = glGetUniformLocation(shader.getId(), name.c_str()));
        textureProperties[name] = UniformValue<Texture*> {
            name,
            propertyId,
            value
        };
    }

    void Material::bind() {
        shader.bind();

        for (pair<const string, UniformValue<float>> pair : floatProperties) {
            GLCALL(glUniform1f(pair.second.propertyId, pair.second.value));
        }

        for (pair<const string, UniformValue<vec4>> pair : vectorProperties) {
            vec4 value = pair.second.value;
            GLCALL(glUniform4f(pair.second.propertyId, value.x, value.y, value.z, value.w));
        }

        for (pair<const string, UniformValue<mat4>> pair : matrixProperties) {
            GLCALL(glUniformMatrix4fv(pair.second.propertyId, 1, false, &pair.second.value[0][0]));
        }

        int index = 0;
        for (pair<const string, UniformValue<Texture*>> pair : textureProperties) {
            Texture* tex = pair.second.value;
            GLuint texId = tex->getId();

            //TODO: Handle tex's nullptr case

            GLCALL(glActiveTexture(GL_TEXTURE0 + index));
            GLCALL(glBindTexture(GL_TEXTURE_2D, texId));

            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

            GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->getWidth(), tex->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->getImageData()));
            GLCALL(glUniform1i(pair.second.propertyId, index));
            index++;
        }
    }
}
