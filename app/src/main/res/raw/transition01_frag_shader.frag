#version 300 es
precision mediump float;
in vec2 vTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform float uOffset;
uniform vec2 uTexSize;
const float zoom_quickness = 0.8;

vec2 zoom(vec2 uv, float amount) {
    return 0.5 + (uv - 0.5) * (1.0 - amount);
}
vec4 transition(vec2 uv) {
    float nQuick = clamp(zoom_quickness, 0.2, 1.0);
    return mix(
            texture(uTexture0, zoom(uv, smoothstep(0.0, nQuick, uOffset))),
            texture(uTexture1, uv),
            smoothstep(nQuick - 0.2, 1.0, uOffset)
    );
}
void main() {
    outColor = transition(vTexCoord);
}