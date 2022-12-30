#version 300 es
precision mediump float;
in vec2 vTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D uTexture;
uniform vec3 uTextColor;

void main() {
    vec4 color = vec4(1.0, 1.0, 1.0, texture(uTexture, vTexCoord).r);
    outColor = vec4(uTextColor, 1.0) * color;
}