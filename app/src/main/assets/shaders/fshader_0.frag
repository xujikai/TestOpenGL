#version 300 es
precision mediump float;
in vec2 vTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D uTextureY;
uniform sampler2D uTextureU;
uniform sampler2D uTextureV;

void main() {
    float y, u, v, r, g, b;
    y = texture(uTextureY, vTexCoord).r;
    u = texture(uTextureU, vTexCoord).r;
    v = texture(uTextureV, vTexCoord).r;
    u = u - 0.5;
    v = v - 0.5;
    r = y + 1.403 * v;
    g = y - 0.344 * u - 0.714 * v;
    b = y + 1.770 * u;
    outColor = vec4(r, g, b, 1.0);
}