#version 300 es
precision highp float;
in vec2 vTexCoord;
layout(location = 0) out vec4 outColor;
uniform sampler2D uTexture;
uniform vec2 uTouchXY;
uniform vec2 uTexSize;
uniform float uTime;
uniform float uBoundary;

void main() {
    float ratio = uTexSize.y / uTexSize.x;
    vec2 texCoord = vTexCoord * vec2(1.0, ratio);
    vec2 touchXY = uTouchXY * vec2(1.0, ratio);
    float dis = distance(texCoord, touchXY);
    if ((uTime - uBoundary > 0.0) && (dis <= uTime + uBoundary) && (dis >= uTime - uBoundary)) {
        float diff = dis - uTime;
        float moveDis = 20.0 * diff * (diff - uBoundary) * (diff + uBoundary);
        vec2 unitDirecVec = normalize(texCoord - touchXY);
        texCoord = texCoord + unitDirecVec * moveDis;
    }
    texCoord = texCoord / vec2(1.0, ratio);
    outColor = texture(uTexture, texCoord);
}