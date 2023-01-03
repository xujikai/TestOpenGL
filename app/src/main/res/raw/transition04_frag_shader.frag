#version 300 es
precision mediump float;
in vec2 vTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform float uOffset;
const float count = 10.0;
const float smoothness = 0.5;

vec4 getFromColor(vec2 uv) {
    return texture(uTexture0, uv);
}
vec4 getToColor(vec2 uv) {
    return texture(uTexture1, uv);
}
vec4 transition(vec2 uv) {
    float pr = smoothstep(-smoothness, 0.0, uv.x - uOffset * (1.0 + smoothness));
    float s = step(pr, fract(count * uv.x));
    return mix(getFromColor(uv), getToColor(uv), s);
}
void main() {
    outColor = transition(vTexCoord);
}