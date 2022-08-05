#version 300 es
precision mediump float;

out vec4 fragColor;

in vec3 vAmbient;
in vec3 vDiffuse;
in vec3 vSpecular;

void main() {
    vec3 objectColor = vec3(0.6, 0.6, 0.6);
    vec3 finalColor = (vAmbient + vDiffuse + vSpecular) * objectColor;
    fragColor = vec4(finalColor, 1.0);
}