#shader vertex
#version AUTO
#if defined(GL_ES)
    precision mediump float;
#endif

layout (location = 0) in vec2 localPosition;
layout (location = 1) in vec2 meshUV;

out vec2 uv;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(localPosition, 0, 1);
    uv = meshUV;
}

#shader fragment
#version AUTO
#if defined(GL_ES)
    precision mediump float;
#endif

in vec2 uv;
layout (location = 0) out vec4 fragmentOutput;

uniform vec4 color;
uniform sampler2D mainTexture;

void main() {
    fragmentOutput = color * texture(mainTexture, uv);
}
