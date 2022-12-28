#version 300 es
precision highp float;
layout(location = 0) out vec4 outColor;
in vec2 vTexCoord;
uniform sampler2D uTexture;

void main() {
    vec4 sampler0, sampler1, sampler2, sampler3;
    float blurStep = 0.16;
    float step = blurStep / 100.0;
    sampler0 = texture(uTexture, vec2(vTexCoord.x - step, vTexCoord.y - step));
    sampler1 = texture(uTexture, vec2(vTexCoord.x + step, vTexCoord.y + step));
    sampler2 = texture(uTexture, vec2(vTexCoord.x + step, vTexCoord.y - step));
    sampler3 = texture(uTexture, vec2(vTexCoord.x - step, vTexCoord.y + step));
    outColor = (sampler0 + sampler1 + sampler2 + sampler3) / 4.0;
    if (outColor.a > 0.05) {
        outColor += vec4(0.2, 0.2, 0.2, 0.0);
    } else {
        discard;
    }
}