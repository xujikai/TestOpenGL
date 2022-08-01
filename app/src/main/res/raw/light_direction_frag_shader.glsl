#version 300 es
precision mediump float;
layout(location = 0) out vec4 fragColor;
in vec3 vPosition;
in vec2 vTextureCoordinates;
in vec3 vNormal;
uniform sampler2D uImageTexture;
uniform vec3 uLightDirection;
uniform vec3 uViewPosition;
uniform vec3 uLightColor;
uniform vec3 uObjectColor;

float myPow(float x, int r) {
    float ret = 1.0;
    for (int i = 0; i < r; i = i + 1) {
        ret = ret * x;
    }
    return ret;
}

void main() {
    vec3 imageTexColor = texture(uImageTexture, vTextureCoordinates).rgb;

    // 环境光
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * uLightColor;

    // 漫反射光
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(-uLightDirection);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    // 镜面光
    float specularStrength = 2.0;
    vec3 viewDir = normalize(uViewPosition - vPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = myPow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * uLightColor;

    vec3 finalColor = (ambient + diffuse + specular) * imageTexColor;
    fragColor = vec4(finalColor, 1.0);
}