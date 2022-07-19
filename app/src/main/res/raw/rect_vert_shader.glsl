attribute vec4 aPosition;
uniform mat4 uProjectionMatrix;

void main() {
    gl_Position = uProjectionMatrix * aPosition;
}
