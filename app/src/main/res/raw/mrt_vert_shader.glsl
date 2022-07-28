#version 300 es
layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec2 aTextureCoordinates;
out vec2 vTextureCoordinates;
void main() {
    vTextureCoordinates = aTextureCoordinates;
    gl_Position = aPosition;
}