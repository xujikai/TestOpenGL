#version 300 es
layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec2 aTexCoord;
uniform mat4 uMvpMatrix;
in vec2 vTexCoord;

void main() {
    gl_Position = uMvpMatrix * aPosition;
    vTexCoord = aTexCoord;
}