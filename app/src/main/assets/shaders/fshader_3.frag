#version 300 es
precision mediump float;
in vec2 vTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D uTextureY;
uniform sampler2D uTextureU;
uniform sampler2D uTextureV;
vec2 texSize = vec2(1280, 720);

void main() {
    float radius = 200.0;
    float angle = 0.8;
    vec2 center = vec2(texSize.x / 2.0, texSize.y / 2.0);
    vec2 tc = vTexCoord * texSize;
    tc -= center;
    float dist = length(tc);
    if (dist < radius) {
        float percent = (radius - dist) / radius;
        float theta = percent * percent * angle * 8.0;
        float s = sin(theta);
        float c = cos(theta);
        tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
    }
    tc += center;
    float y, u, v, r, g, b;
    y = texture(uTextureY, tc / texSize).r;
    u = texture(uTextureU, tc / texSize).r;
    v = texture(uTextureV, tc / texSize).r;
    u = u - 0.5;
    v = v - 0.5;
    r = y + 1.403 * v;
    g = y - 0.344 * u - 0.714 * v;
    b = y + 1.770 * u;
    outColor = vec4(r, g, b, 1.0);
}