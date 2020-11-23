precision mediump float;
attribute vec4 aPosition;
uniform mat4 uMatrix;
attribute vec2 aCoordinate;
varying vec2 vCoordinate;

void main() {
    gl_Position = aPosition * uMatrix;
    vCoordinate = aCoordinate;
}
