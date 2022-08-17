//
// Created by Administrator on 2022/8/16.
//

#include "BeatHeartSample.h"

GLfloat mVertexArr[] = {
        -1.0f,  1.0f, 0.0f,  // Position 0
        -1.0f, -1.0f, 0.0f,  // Position 1
        1.0f,  -1.0f, 0.0f,  // Position 2
        1.0f,   1.0f, 0.0f,  // Position 3
};

GLushort mIndexArr[] = { 0, 1, 2, 0, 2, 3 };

void BeatHeartSample::Init() {
    static std::string vShaderStr = R"(
        #version 300 es
        layout(location = 0) in vec4 aPosition;
        uniform mat4 uMvpMatrix;
        void main() {
            gl_Position = uMvpMatrix * aPosition;
        }
    )";
    static std::string fShaderStr = R"(
        #version 300 es
        precision highp float;
        layout(location = 0) out vec4 fragColor;
        uniform float uTime;
        uniform vec2 uScreenSize;
        void main() {
            vec2 fragCoord = gl_FragCoord.xy;
            vec2 centerDir = (2.0 * fragCoord - uScreenSize.xy) / min(uScreenSize.y, uScreenSize.x);
            // 背景颜色
            vec2 uv = fragCoord / uScreenSize;
            vec3 bcol = vec3(1.0, 0.8, 0.8) * (1.0 - 0.38 * length(centerDir));
            // 动画
            float tt = uTime;
            float ss = pow(tt, 0.2) * 0.5 + 0.5;
            ss = 1.0 + ss * 0.5 * sin(tt * 6.2831 * 3.0 + centerDir.y * 0.5) * exp(-tt * 4.0);
            centerDir *= vec2(0.5, 1.5) + ss * vec2(0.5, -0.5);
            // 形状
            #if 0
            centerDir *= 0.8;
            centerDir.y = -0.1 - centerDir.y * 1.2 + abs(centerDir.x) * (1.0 - abs(centerDir.x));
            float r = length(centerDir);
            float d = 0.5;
            #else
            centerDir.y -= 0.25;
            float a = atan(centerDir.x, centerDir.y) / 3.141592653;
            float r = length(centerDir);
            float h = abs(a);
            float d = (13.0 * h - 22.0 * h * h + 10.0 * h * h * h) / (6.0 - 5.0 * h);
            #endif
            // 颜色
            float s = 0.75 + 0.75 * centerDir.x;
            s *= 1.0 - 0.4 * r;
            s = 0.3 + 0.7 * s;
            s *= 0.5 + 0.5 * pow(1.0 - clamp(r / d, 0.0, 1.0), 0.1);
            vec3 hcol = vec3(1.0, 0.5 * r, 0.3) * s;
            vec3 col = mix(bcol, hcol, smoothstep(-0.06, 0.06, d - r));
            fragColor = vec4(col, 1.0);
        }
    )";

    mProgramObj = GLUtils::CreateProgram(vShaderStr.data(), fShaderStr.data());

    aPositionLoc = 0;
    uMvpMatrixLoc = glGetUniformLocation(mProgramObj, "uMvpMatrix");
    uTimeLoc = glGetUniformLocation(mProgramObj, "uTime");
    uScreenSizeLoc = glGetUniformLocation(mProgramObj, "uScreenSize");

    glGenBuffers(2, mVboIds);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexArr), mVertexArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndexArr), mIndexArr, GL_STATIC_DRAW);

    glGenVertexArrays(1, mVaoIds);

    glBindVertexArray(mVboIds[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glEnableVertexAttribArray(aPositionLoc);
    glVertexAttribPointer(aPositionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[1]);
    glBindVertexArray(GL_NONE);
}

void BeatHeartSample::Draw(int screenW, int screenH) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgramObj);
    glBindVertexArray(mVaoIds[0]);
    UpdateMVPMatrix(mMvpMatrix, mRotateX, mRotateY, (float) screenW / (float) screenH);
    glUniformMatrix4fv(uMvpMatrixLoc, 1, false, &mMvpMatrix[0][0]);
    float time = static_cast<float>(fmod(GetSysCurrentTime(), 2000) / 2000);
    glUniform1f(uTimeLoc, time);
    glUniform2f(uScreenSizeLoc, (float)screenW, (float)screenH);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    glBindVertexArray(GL_NONE);
}

void BeatHeartSample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        mProgramObj = GL_NONE;
    }
    if (mVboIds[0]) {
        glDeleteBuffers(2, mVboIds);
    }
    if (mVaoIds[0]) {
        glDeleteVertexArrays(1, mVaoIds);
    }
}

void BeatHeartSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio) {
    angleX = angleX % 360;
    angleY = angleY % 360;

    float radiansX = static_cast<float>(MATH_PI / 180.0f * angleX);
    float radiansY = static_cast<float>(MATH_PI / 180.0f * angleY);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
    );

    glm::mat4 projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 10.0f);

    mMvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
}
