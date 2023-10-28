#shader vertex
#version AUTO
#if defined(GL_ES)
    precision mediump float;
#endif

layout (location = 0) in vec2 localPosition;

void main() {
    gl_Position = vec4(localPosition, 0, 1);
}

#shader fragment
#version AUTO
#if defined(GL_ES)
    precision mediump float;
#endif

layout (location = 0) out vec4 fragmentOutput;

uniform vec4 color;

void main() {
    fragmentOutput = color;
}
