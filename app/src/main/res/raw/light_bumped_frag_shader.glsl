#version 300 es
precision mediump float;

layout(location = 0) out vec4 fragColor;

in vec3 vPosition;
in vec2 vTextureCoordinates;
in vec3 vTangentLightPosition;
in vec3 vTangentViewPosition;
in vec3 vTangentPosition;

uniform sampler2D uImageTexture;
uniform sampler2D uNormalTexture;
uniform vec3 uLightColor;

float myPow(float x, int r) {
    float ret = 1.0;
    for (int i = 0; i < r; i++) {
        ret = ret * x;
    }
    return ret;
}

void main() {
    vec3 normalTex = texture(uNormalTexture, vTextureCoordinates).rgb;
    normalTex = normalize(normalTex * 2.0 - 1.0);
    vec3 imageTexColor = texture(uImageTexture, vTextureCoordinates).rgb;

    // 环境光
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * uLightColor;

    // 漫反射光
    vec3 normal = normalize(normalTex);
    vec3 lightDir = normalize(vTangentLightPosition - vTangentPosition);
    float diffuseTheta = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseTheta * uLightColor;

    // 镜面光
    float specularStrength = 2.0;
    vec3 viewDir = normalize(vTangentViewPosition - vTangentPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularTheta = myPow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * specularTheta * uLightColor;

    vec3 finalColor = (ambient + diffuse + specular) * imageTexColor;
    fragColor = vec4(finalColor, 1.0);

}