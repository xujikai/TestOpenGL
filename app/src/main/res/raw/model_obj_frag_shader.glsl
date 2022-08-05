#version 300 es
precision mediump float;

out vec4 fragColor;

in vec2 vTexCoord;
in vec3 vAmbient;
in vec3 vDiffuse;
in vec3 vSpecular;

uniform sampler2D uTextureDiffuse1;

void main() {
    vec3 imageTexColor = texture(uTextureDiffuse1, vTexCoord).rgb;
    vec3 finalColor = (vAmbient + vDiffuse + vSpecular) * imageTexColor;
    fragColor = vec4(finalColor, 1.0);
}