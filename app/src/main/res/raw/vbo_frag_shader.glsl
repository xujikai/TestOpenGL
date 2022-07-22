#version 300 es
precision mediump float;
out vec4 fragColor;
in vec2 vTextureCoordinates;
uniform sampler2D uTexture;
void main() {
    fragColor = texture(uTexture, vTextureCoordinates);
}