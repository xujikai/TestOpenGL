precision mediump float;
varying vec2 vTextureCoordinates;
uniform sampler2D uTexture;
void main() {
    float offset = 0.005;
    vec4 color = texture2D(uTexture, vTextureCoordinates) * 0.11111;
    color += texture2D(uTexture, vec2(vTextureCoordinates.x - offset, vTextureCoordinates.y)) * 0.11111;
    color += texture2D(uTexture, vec2(vTextureCoordinates.x + offset, vTextureCoordinates.y)) * 0.11111;
    color += texture2D(uTexture, vec2(vTextureCoordinates.x - offset * 2.0, vTextureCoordinates.y)) * 0.11111;
    color += texture2D(uTexture, vec2(vTextureCoordinates.x + offset * 2.0, vTextureCoordinates.y)) * 0.11111;
    color += texture2D(uTexture, vec2(vTextureCoordinates.x - offset * 3.0, vTextureCoordinates.y)) * 0.11111;
    color += texture2D(uTexture, vec2(vTextureCoordinates.x + offset * 3.0, vTextureCoordinates.y)) * 0.11111;
    color += texture2D(uTexture, vec2(vTextureCoordinates.x - offset * 4.0, vTextureCoordinates.y)) * 0.11111;
    color += texture2D(uTexture, vec2(vTextureCoordinates.x + offset * 4.0, vTextureCoordinates.y)) * 0.11111;
    gl_FragColor = color;
}