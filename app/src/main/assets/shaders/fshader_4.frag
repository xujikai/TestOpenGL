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
    float circleRadius = float(0.5);
    float minZoom = 0.4;
    float maxZoom = 0.6;
    vec2 center = vec2(texSize.x / 2.0, texSize.y / 2.0);
    vec2 uv = vTexCoord;
    uv.x *= (texSize.x / texSize.y);
    vec2 realCenter = vec2(0.0, 0.0);
    realCenter.x = (center.x / texSize.x) * (texSize.x / texSize.y);
    realCenter.y = center.y / texSize.y;
    float maxX = realCenter.x + circleRadius;
    float minX = realCenter.x - circleRadius;
    float maxY = realCenter.y + circleRadius;
    float minY = realCenter.y - circleRadius;
    if (uv.x > minX && uv.x < maxX && uv.y > minY && uv.y < maxY) {
        float relX = uv.x - realCenter.x;
        float relY = uv.y - realCenter.y;
        float ang = atan(relY, relX);
        float dist = sqrt(relX * relX + relY * relY);
        if (dist <= circleRadius) {
            float newRad = dist * ((maxZoom * dist / circleRadius) + minZoom);
            float newX = realCenter.x + cos(ang) * newRad;
            newX *= (texSize.y / texSize.x);
            float newY = realCenter.y + sin(ang) * newRad;
            outColor = YuvToRgb(vec2(newX, newY));
        } else {
            outColor = YuvToRgb(vTexCoord);
        }
    } else {
        outColor = YuvToRgb(vTexCoord);
    }
}