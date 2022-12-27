#version 300 es
precision mediump float;
layout(location = 0) out vec4 fragColor0;
layout(location = 1) out vec4 fragColor1;
layout(location = 2) out vec4 fragColor2;
layout(location = 3) out vec4 fragColor3;
in vec2 vTextureCoord;
uniform sampler2D uTexture;
void main() {
    vec4 textureColor = texture(uTexture, vTextureCoord);
    fragColor0 = textureColor;
    fragColor1 = vec4(textureColor.r, 0.0, 0.0, 1.0);
    fragColor2 = vec4(0.0, textureColor.g, 0.0, 1.0);
    fragColor3 = vec4(0.0, 0.0, textureColor.b, 1.0);
}