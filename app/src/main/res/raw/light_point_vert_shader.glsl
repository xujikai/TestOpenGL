#version 300 es
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTextureCoordinates;
layout(location = 2) in vec3 aNormal;
out vec3 vPosition;
out vec2 vTextureCoordinates;
out vec3 vNormal;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
void main() {
    vTextureCoordinates = aTextureCoordinates;
    vPosition = vec3(uModelMatrix * vec4(aPosition, 1.0));
    vNormal = mat3(transpose(inverse(uModelMatrix))) * aNormal;
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
}