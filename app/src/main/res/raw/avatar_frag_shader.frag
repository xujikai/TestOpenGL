#version 300 es
precision highp float;
in vec2 vTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D uTexture;
uniform vec2 uTexSize;
uniform float uNeedRotate;
uniform float uRotateAngle;

vec2 rotate(float radius, float angle, vec2 texSize, vec2 texCoord) {
    vec2 newTexCoord = texCoord;
    vec2 center = vec2(texSize.x / 2.0, texSize.y / 2.0);
    vec2 tc = texCoord * texSize;
    tc -= center;
    float dist = length(tc);
    if (dist < radius) {
        float percent = (radius - dist) / radius;
        float theta = percent * percent * angle * 8.0;
        float s = sin(theta);
        float c = cos(theta);
        tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
        tc += center;
        newTexCoord = tc / texSize;
    }
    return newTexCoord;
}

void main() {
    vec2 texCoord = vTexCoord;
    if (uNeedRotate > 0.0) {
        texCoord = rotate(0.5 * uTexSize.x, uRotateAngle, uTexSize, vTexCoord);
    }
    outColor = texture(uTexture, texCoord);
    if (outColor.a < 0.6) discard;
}