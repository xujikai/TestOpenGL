#version 300 es
precision mediump float;
in vec2 vTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform float uOffset;
const vec2 direction = vec2(0.0, 1.0);

vec4 getFromColor(vec2 uv) {
    return texture(uTexture0, uv);
}
vec4 getToColor(vec2 uv) {
    return texture(uTexture1, uv);
}
vec4 transition(vec2 uv) {
    vec2 p = uv + uOffset * sign(direction);
    vec2 f = fract(p);
    return mix(
            getToColor(f),
            getFromColor(f),
            step(0.0, p.y) * step(p.y, 1.0) * step(0.0, p.x) * step(p.x, 1.0)
    );
}
void main() {
    outColor = transition(vTexCoord);
}