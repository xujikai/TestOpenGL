#extension GL_OES_EGL_image_external : require
precision mediump float;

varying vec2 vCoordinate;
uniform samplerExternalOES uTexture;

void main() {
    // 正常效果
//    gl_FragColor = texture2D(uTexture, vCoordinate);
    // 黑白效果
    vec4 color = texture2D(uTexture, vCoordinate);
    float gray = (color.r + color.g + color.b) / 3.0;
    gl_FragColor = vec4(gray, gray, gray, 1.0);
}
