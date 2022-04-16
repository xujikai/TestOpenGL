attribute vec4 aPosition;
attribute vec2 aTextureCoordinates;
varying vec2 vTextureCoordinates;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main() {
    vTextureCoordinates = aTextureCoordinates;
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;
}
