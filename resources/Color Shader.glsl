#shader vertex
#version 330 core
// #version 300 es

layout (location = 0) in vec2 localPosition;
layout (location = 1) in vec2 meshUV;
out vec2 uv;

void main() {
    gl_Position = vec4(localPosition, 0, 1);
    uv = meshUV;
};

#shader fragment
#version 330 core

in vec2 uv;
layout (location = 0) out vec4 fragmentOutput;

uniform vec4 color;
uniform sampler2D mainTexture;

void main() {
    fragmentOutput = color * texture(mainTexture, uv);
};
