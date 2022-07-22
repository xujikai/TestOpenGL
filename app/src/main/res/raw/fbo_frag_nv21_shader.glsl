precision mediump float;
varying vec2 vTextureCoordinates;
uniform sampler2D uYTexture;
uniform sampler2D uUvTexture;
void main() {
    vec3 yuv;
    yuv.x = texture2D(uYTexture, vTextureCoordinates).r;
    yuv.y = texture2D(uUvTexture, vTextureCoordinates).a - 0.5;
    yuv.z = texture2D(uUvTexture, vTextureCoordinates).r - 0.5;
    highp vec3 rgb = mat3(
        1, 1, 1,
        0, -0.344, 1.770,
        1.403, -0.714, 0
    ) * yuv;
    gl_FragColor = vec4(rgb, 1);
}