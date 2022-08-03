#version 300 es

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTextureCoordinates;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 vPosition;
out vec2 vTextureCoordinates;
out vec3 vTangentLightPosition;
out vec3 vTangentViewPosition;
out vec3 vTangentPosition;

uniform vec3 uLightPosition;
uniform vec3 uViewPosition;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main() {
    vTextureCoordinates = aTextureCoordinates;
    vPosition = vec3(uModelMatrix * vec4(aPosition, 1.0));
    mat3 normalMatrix = transpose(inverse(mat3(uModelMatrix)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    vec3 N = normalize(normalMatrix * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));
    vTangentLightPosition = TBN * uLightPosition;
    vTangentViewPosition = TBN * uViewPosition;
    vTangentPosition = TBN * vPosition;
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
}