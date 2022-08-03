#version 300 es
precision mediump float;
layout(location = 0) out vec4 fragColor;
in vec3 vPosition;
in vec2 vTextureCoordinates;
in vec3 vNormal;
uniform sampler2D uImageTexture;
uniform vec3 uLightPosition;
uniform vec3 uViewPosition;
uniform vec3 uLightColor;

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
    vec3 lightDir = normalize(uLightPosition - vPosition);
    float diffuseTheta = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseTheta * uLightColor;

    // 镜面光
    float specularStrength = 2.0;
    vec3 viewDir = normalize(uViewPosition - vPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularTheta = myPow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * specularTheta * uLightColor;

    // 聚光 (柔边)
    vec3 spotPosition = vec3(-2.0, 0.0, 0.0);
    vec3 spotDir = normalize(-spotPosition);
    float spotTheta = dot(lightDir, spotDir);
    float epsilon = 0.15;
    float intensity = clamp((spotTheta - 0.85) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // 光源衰减
    float dist = length(uLightPosition - vPosition);
    float kc = 0.1;
    float kl = 0.1;
    float kq = 0.1;
    float attenuation = 1.0 / (kc + kl * dist + kq * dist * dist);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 finalColor = (ambient + diffuse + specular) * imageTexColor;
    fragColor = vec4(finalColor, 1.0);
}