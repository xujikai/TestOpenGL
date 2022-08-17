#version 300 es
precision highp float;
layout(location = 0) out vec4 fragColor;
uniform float uTime;
uniform vec2 uScreenSize;
void main() {
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 centerDir = (2.0 * fragCoord - uScreenSize.xy) / min(uScreenSize.y, uScreenSize.x);
    // 背景颜色
    vec2 uv = fragCoord / uScreenSize;
    vec3 bcol = vec3(1.0, 0.8, 0.8) * (1.0 - 0.38 * length(centerDir));
    // 动画
    float tt = uTime;
    float ss = pow(tt, 0.2) * 0.5 + 0.5;
    ss = 1.0 + ss * 0.5 * sin(tt * 6.2831 * 3.0 + centerDir.y * 0.5) * exp(-tt * 4.0);
    centerDir *= vec2(0.5, 1.5) + ss * vec2(0.5, -0.5);
    // 形状
    #if 0
    centerDir *= 0.8;
    centerDir.y = -0.1 - centerDir.y * 1.2 + abs(centerDir.x) * (1.0 - abs(centerDir.x));
    float r = length(centerDir);
    float d = 0.5;
    #else
    centerDir.y -= 0.25;
    float a = atan(centerDir.x, centerDir.y) / 3.141592653;
    float r = length(centerDir);
    float h = abs(a);
    float d = (13.0 * h - 22.0 * h * h + 10.0 * h * h * h) / (6.0 - 5.0 * h);
    #endif
    // 颜色
    float s = 0.75 + 0.75 * centerDir.x;
    s *= 1.0 - 0.4 * r;
    s = 0.3 + 0.7 * s;
    s *= 0.5 + 0.5 * pow(1.0 - clamp(r / d, 0.0, 1.0), 0.1);
    vec3 hcol = vec3(1.0, 0.5 * r, 0.3) * s;
    vec3 col = mix(bcol, hcol, smoothstep(-0.06, 0.06, d - r));
    fragColor = vec4(col, 1.0);
}