#version 300 es
precision mediump float;
in vec2 vTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D uTextureY;
uniform sampler2D uTextureU;
uniform sampler2D uTextureV;

vec4 YuvToRgb(vec2 uv) {
    float y, u, v, r, g, b;
    y = texture(uTextureY, uv).r;
    u = texture(uTextureU, uv).r;
    v = texture(uTextureV, uv).r;
    u = u - 0.5;
    v = v - 0.5;
    r = y + 1.403 * v;
    g = y - 0.344 * u - 0.714 * v;
    b = y + 1.770 * u;
    return vec4(r, g, b, 1.0);
}
void main() {
    vec2 newTexCoord = vTexCoord;
    if (newTexCoord.x < 0.5) {
        newTexCoord.x = newTexCoord.x * 2.0;
    } else {
        newTexCoord.x = (newTexCoord.x - 0.5) * 2.0;
    }
    if (newTexCoord.y < 0.5) {
        newTexCoord.y = newTexCoord.y * 2.0;
    } else {
        newTexCoord.y = (newTexCoord.y - 0.5) * 2.0;
    }
    outColor = YuvToRgb(newTexCoord);
}