//
// Created by Administrator on 2022/12/29.
//

#include "WaterRippleSample.h"

static GLfloat mVertexArr[] = {
    -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f
};

static GLfloat mTexCoordArr[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
};

static GLushort mIndexArr[] = {0, 1, 2, 0, 2, 3};

WaterRippleSample::~WaterRippleSample() {
    NativeImageUtil::FreeNativeImage(&mRenderImage);
}

void WaterRippleSample::Init() {
    static std::string vShaderStr = R"(
        #version 300 es
        layout(location = 0) in vec4 aPosition;
        layout(location = 1) in vec2 aTexCoord;
        uniform mat4 uMvpMatrix;
        out vec2 vTexCoord;

        void main() {
            gl_Position = uMvpMatrix * aPosition;
            vTexCoord = aTexCoord;
        }
    )";
    static std::string fShaderStr = R"(
        #version 300 es
        precision highp float;
        in vec2 vTexCoord;
        layout(location = 0) out vec4 outColor;
        uniform sampler2D uTexture;
        uniform vec2 uTouchXY;
        uniform vec2 uTexSize;
        uniform float uTime;
        uniform float uBoundary;

        void main() {
            float ratio = uTexSize.y / uTexSize.x;
            vec2 texCoord = vTexCoord * vec2(1.0, ratio);
            vec2 touchXY = uTouchXY * vec2(1.0, ratio);
            float dis = distance(texCoord, touchXY);
            if ((uTime - uBoundary > 0.0) && (dis <= uTime + uBoundary) && (dis >= uTime - uBoundary)) {
                float diff = dis - uTime;
                float moveDis = 20.0 * diff * (diff - uBoundary) * (diff + uBoundary);
                vec2 unitDirecVec = normalize(texCoord - touchXY);
                texCoord = texCoord + unitDirecVec * moveDis;
            }
            texCoord = texCoord / vec2(1.0, ratio);
            outColor = texture(uTexture, texCoord);
        }
    )";
    // float moveDis = 20.0 * diff * (diff - uBoundary) * (diff + uBoundary);
    // float moveDis = (1.0 - pow(abs(20.0 * diff), 4.8)) * diff;
    // float moveDis = -pow(8.0 * diff, 3.0);

    mProgramObj = GLUtils::CreateProgram(vShaderStr.data(), fShaderStr.data());

    uMvpMatrixLoc = glGetUniformLocation(mProgramObj, "uMvpMatrix");
    uTextureLoc = glGetUniformLocation(mProgramObj, "uTexture");
    uTouchXYLoc = glGetUniformLocation(mProgramObj, "uTouchXY");
    uTexSizeLoc = glGetUniformLocation(mProgramObj, "uTexSize");
    uTimeLoc = glGetUniformLocation(mProgramObj, "uTime");
    uBoundaryLoc = glGetUniformLocation(mProgramObj, "uBoundary");

    glGenBuffers(3, mVboIds);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexArr), mVertexArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mTexCoordArr), mTexCoordArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndexArr), mIndexArr, GL_STATIC_DRAW);

    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
    glBindVertexArray(GL_NONE);

    glGenTextures(1, &mImageTexId);
    glBindTexture(GL_TEXTURE_2D, mImageTexId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void WaterRippleSample::Draw(int screenW, int screenH) {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgramObj);
    glBindVertexArray(mVaoId);

    UpdateMVPMatrix(mMvpMatrix, mAngleX, mAngleY, (float) screenW / (float) screenH);
    glUniformMatrix4fv(uMvpMatrixLoc, 1, GL_FALSE, &mMvpMatrix[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, mImageTexId);
    glUniform1i(uTextureLoc, 0);

    glUniform2fv(uTouchXYLoc, 1, &mTouchXY[0]);
    glUniform2fv(uTexSizeLoc, 1, &glm::vec2(mRenderImage.width, mRenderImage.height)[0]);
    auto time = static_cast<float>(fmod(mFrameIndex, 150) / 120);
    glUniform1f(uTimeLoc, time);
    glUniform1f(uBoundaryLoc, 0.1f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    mFrameIndex++;
}

void WaterRippleSample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        glDeleteBuffers(3, mVboIds);
        glDeleteVertexArrays(1, &mVaoId);
        glDeleteTextures(1, &mImageTexId);
        mProgramObj = GL_NONE;
    }
}

void WaterRippleSample::SetImageData(NativeImage *pImage) {
    if (pImage) {
        mRenderImage.width = pImage->width;
        mRenderImage.height = pImage->height;
        mRenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &mRenderImage);
    }
}

void WaterRippleSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio) {
    LOGCATE("WaterRippleSample::UpdateMVPMatrix angleX = %d, angleY = %d, ratio = %f", angleX, angleY, ratio);
    angleX = angleX % 360;
    angleY = angleY % 360;

    //转化为弧度角
    float radiansX = static_cast<float>(MATH_PI / 180.0f * angleX);
    float radiansY = static_cast<float>(MATH_PI / 180.0f * angleY);

    // Projection matrix
    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    //glm::mat4 Projection = glm::frustum(-ratio, ratio, -1.0f, 1.0f, 4.0f, 100.0f);
    //glm::mat4 Projection = glm::perspective(45.0f,ratio, 0.1f,100.f);

    // View matrix
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 4), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(mScaleX, mScaleY, 1.0f));
    Model = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    Model = glm::rotate(Model, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));

    mvpMatrix = Projection * View * Model;
}
