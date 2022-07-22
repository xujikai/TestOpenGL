precision mediump float;
varying vec2 vTextureCoordinates;
uniform sampler2D uTexture;
void main() {
    vec4 color = texture2D(uTexture, vTextureCoordinates);
    color.b = 0.5;
    gl_FragColor = color;
}