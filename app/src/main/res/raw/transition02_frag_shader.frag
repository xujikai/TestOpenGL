#version 300 es
precision mediump float;
in vec2 vTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform vec2 uTexSize;
uniform float uOffset;
const int bars = 20;
const float amplitude = 2.0;
const float noise = 0.1;
const float frequency = 0.5;
const float dripScale = 0.5;

float rand(int num) {
    return fract(mod(float(num) * 67123.313, 12.0) * sin(float(num) * 10.3) * cos(float(num)));
}
float wave(int num) {
    float fn = float(num) * frequency * 0.1 * float(bars);
    return cos(fn * 0.5) * cos(fn * 0.13) * sin((fn + 10.0) * 0.3) / 2.0 + 0.5;
}
float drip(int num) {
    return sin(float(num) / float(bars - 1) * 3.141592) * dripScale;
}
float pos(int num) {
    return (noise == 0.0 ? wave(num) : mix(wave(num), rand(num), noise)) + (dripScale == 0.0 ? 0.0 : drip(num));
}
vec4 getFromColor(vec2 uv) {
    return texture(uTexture0, uv);
}
vec4 getToColor(vec2 uv) {
    return texture(uTexture1, uv);
}
vec4 transition(vec2 uv) {
    int bar = int(uv.x * float(bars));
    float scale = 1.0 + pos(bar) * amplitude;
    float phase = uOffset * scale;
    float posY = uv.y / vec2(1.0).y;
    vec2 newUv;
    vec4 color;
    if (phase + posY < 1.0) {
        newUv = vec2(uv.x, uv.y + mix(0.0, vec2(1.0).y, phase) / vec2(1.0).xy);
        color = getFromColor(newUv);
    } else {
        newUv = uv.xy / vec2(1.0).xy;
        color = getToColor(newUv);
    }
    return color;
}
void main() {
    outColor = transition(vTexCoord);
}