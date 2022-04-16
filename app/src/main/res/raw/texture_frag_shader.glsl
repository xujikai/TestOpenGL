precision mediump float;

varying vec2 vTextureCoordinates;
uniform sampler2D uTextureUnit;

void main() {
//    gl_FragColor = vec4(1f, 1f, 1f, 1f);
    gl_FragColor = texture2D(uTextureUnit, vTextureCoordinates);
}
