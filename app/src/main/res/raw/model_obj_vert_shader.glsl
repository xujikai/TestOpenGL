#version 300 es
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 vTexCoord;
out vec3 vAmbient;
out vec3 vDiffuse;
out vec3 vSpecular;

uniform mat4 uMvpMatrix;
uniform mat4 uModelMatrix;
uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;

void main() {
    vTexCoord = aTexCoord;
    // 转换为世界坐标系中的坐标，后面光照的计算都基于该坐标
    vec3 vPosition = vec3(uModelMatrix * vec4(aPosition, 1.0));
    gl_Position = uMvpMatrix * vec4(aPosition, 1.0);

    // 环境光
    float ambientStrength = 0.25;
    vAmbient = ambientStrength * uLightColor;
    // 漫反射光
    float diffuseStrength = 0.5;
    vec3 normalDir = normalize(vec3(uModelMatrix * vec4(aNormal, 1.0)));
    vec3 lightDir = normalize(uLightPos - vPosition);
    float diffuseTheta = max(dot(normalDir, lightDir), 0.0);
    vDiffuse = diffuseStrength * diffuseTheta * uLightColor;
    // 镜面光
    float specularStrength = 2.0;
    vec3 viewDir = normalize(uViewPos - vPosition);
    vec3 reflectDir = reflect(-lightDir, normalDir);
    float specularTheta = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vSpecular = specularStrength * specularTheta * uLightColor;
}