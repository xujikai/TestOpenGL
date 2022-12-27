#version 300 es
layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec2 aTextureCoord;
uniform mat4 uMvpMatrix;
out vec2 vTextureCoord;
void main() {
    gl_Position = uMvpMatrix * aPosition;
    vTextureCoord = aTextureCoord;
}