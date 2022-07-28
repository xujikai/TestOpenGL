#version 300 es
precision mediump float;
layout(location = 0) out vec4 fragColor0;
layout(location = 1) out vec4 fragColor1;
layout(location = 2) out vec4 fragColor2;
in vec2 vTextureCoordinates;
uniform sampler2D uTexture;
void main() {
    vec4 color = texture(uTexture, vTextureCoordinates);
    fragColor0 = vec4(1.0, color.g, color.b, color.a);
    fragColor1 = vec4(color.r, 1.0, color.b, color.a);
    fragColor2 = vec4(color.r, color.g, 1.0, color.a);
}