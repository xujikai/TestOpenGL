#version 300 es
precision mediump float;
in vec2 vTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D uTextureY;
uniform sampler2D uTextureU;
uniform sampler2D uTextureV;
vec2 texSize = vec2(1280, 720);

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
    vec2 pos = vTexCoord.xy;
    vec2 onePixel = vec2(1, 1) / texSize;
    vec4 color = vec4(0);
    mat3 edgeDetectionKernel = mat3(
        -1, -1, -1,
        -1, 8, -1,
        -1, -1, -1
    );
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            vec2 samplePos = pos + vec2(i - 1 , j - 1) * onePixel;
            vec4 sampleColor = YuvToRgb(samplePos);
            sampleColor *= edgeDetectionKernel[i][j];
            color += sampleColor;
        }
    }
    outColor = vec4(color.rgb, 1.0);
}
