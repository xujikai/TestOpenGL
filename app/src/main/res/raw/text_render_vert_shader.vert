#version 300 es
layout(location = 0) in vec4 aPosition;
uniform mat4 uMvpMatrix;
out vec2 vTexCoord;

void main() {
    gl_Position = uMvpMatrix * vec4(aPosition.xy, 0.0, 1.0);
    vTexCoord = aPosition.zw;
}