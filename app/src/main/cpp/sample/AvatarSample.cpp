//
// Created by Administrator on 2022/12/28.
//

#include "AvatarSample.h"

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

static GLushort mIndexArr[] = {
        0, 1, 2,
        0, 2, 3
};

AvatarSample::~AvatarSample() {
    for (auto & mRenderImage : mRenderImages) {
        NativeImageUtil::FreeNativeImage(&mRenderImage);
    }
}

void AvatarSample::Init() {
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
        uniform vec2 uTexSize;
        uniform float uNeedRotate;
        uniform float uRotateAngle;

        vec2 rotate(float radius, float angle, vec2 texSize, vec2 texCoord) {
            vec2 newTexCoord = texCoord;
            vec2 center = vec2(texSize.x / 2.0, texSize.y / 2.0);
            vec2 tc = texCoord * texSize;
            tc -= center;
            float dist = length(tc);
            if (dist < radius) {
                float percent = (radius - dist) / radius;
                float theta = percent * percent * angle * 8.0;
                float s = sin(theta);
                float c = cos(theta);
                tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
                tc += center;
                newTexCoord = tc / texSize;
            }
            return newTexCoord;
        }

        void main() {
            vec2 texCoord = vTexCoord;
            if (uNeedRotate > 0.0) {
                texCoord = rotate(0.5 * uTexSize.x, uRotateAngle, uTexSize, vTexCoord);
            }
            outColor = texture(uTexture, texCoord);
            if (outColor.a < 0.6) discard;
        }
    )";
    static std::string fBlurShaderStr = R"(
        #version 300 es
        precision highp float;
        layout(location = 0) out vec4 outColor;
        in vec2 vTexCoord;
        uniform sampler2D uTexture;

        void main() {
            vec4 sampler0, sampler1, sampler2, sampler3;
            float blurStep = 0.16;
            float step = blurStep / 100.0;
            sampler0 = texture(uTexture, vec2(vTexCoord.x - step, vTexCoord.y - step));
            sampler1 = texture(uTexture, vec2(vTexCoord.x + step, vTexCoord.y + step));
            sampler2 = texture(uTexture, vec2(vTexCoord.x + step, vTexCoord.y - step));
            sampler3 = texture(uTexture, vec2(vTexCoord.x - step, vTexCoord.y + step));
            outColor = (sampler0 + sampler1 + sampler2 + sampler3) / 4.0;
            if (outColor.a > 0.05) {
                outColor += vec4(0.2, 0.2, 0.2, 0.0);
            } else {
                discard;
            }
        }
    )";

    mProgramObj = GLUtils::CreateProgram(vShaderStr.data(), fShaderStr.data());
    mBlurProgramObj = GLUtils::CreateProgram(vShaderStr.data(), fBlurShaderStr.data());

    uMvpMatrixLoc = glGetUniformLocation(mProgramObj, "uMvpMatrix");
    uTextureLoc = glGetUniformLocation(mProgramObj, "uTexture");
    uTexSizeLoc = glGetUniformLocation(mProgramObj, "uTexSize");
    uNeedRotateLoc = glGetUniformLocation(mProgramObj, "uNeedRotate");
    uRotateAngleLoc = glGetUniformLocation(mProgramObj, "uRotateAngle");

    uBlurMvpMatrixLoc = glGetUniformLocation(mBlurProgramObj, "uMvpMatrix");
    uBlurTextureLoc = glGetUniformLocation(mBlurProgramObj, "uTexture");

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

    glGenTextures(RENDER_IMG_NUM, mImageTexIds);
    for (int i = 0; i < RENDER_IMG_NUM; ++i) {
        glBindTexture(GL_TEXTURE_2D, mImageTexIds[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }
}

void AvatarSample::Draw(int screenW, int screenH) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(mProgramObj);
    glBindVertexArray(mVaoId);

    // 背景层
    float dScaleLevel = mFrameIndex % 200 * 1.0f / 1000 + 0.0001f;
    float scaleLevel = static_cast<float>(1.0f + dScaleLevel * pow(-1, mFrameIndex / 200));
    scaleLevel = scaleLevel < 1.0 ? scaleLevel + 0.2f : scaleLevel;
    mScaleX = mScaleY = scaleLevel + 0.4f;
//    mScaleX = mScaleY = 1.0f;
    UpdateMVPMatrix(mMvpMatrix, mAngleX, mAngleY, mTransX / 2, mTransY / 2, (float) screenW / (float) screenH);
    glUniformMatrix4fv(uMvpMatrixLoc, 1, GL_FALSE, &mMvpMatrix[0][0]);

    glUniform2fv(uTexSizeLoc, 1, &glm::vec2(mRenderImages[0].width, mRenderImages[0].height)[0]);
    glUniform1f(uNeedRotateLoc, 1.0f);
    glUniform1f(uRotateAngleLoc, mTransX * 1.5f);

    for (int i = 0; i < RENDER_IMG_NUM; ++i) {
        glBindTexture(GL_TEXTURE_2D, mImageTexIds[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImages[i].width, mRenderImages[i].height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, mRenderImages[i].ppPlane[0]);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageTexIds[0]);
    glUniform1i(uTextureLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    // 人物层
    scaleLevel = static_cast<float>(1.0f + dScaleLevel * pow(-1, mFrameIndex / 200 + 1));
    scaleLevel = scaleLevel < 1.0 ? scaleLevel + 0.2f : scaleLevel;
    mScaleX = mScaleY = scaleLevel + 0.4f;
    UpdateMVPMatrix(mMvpMatrix, mAngleX, mAngleY, mTransX * 1.2f, mTransY * 1.2f, (float) screenW / (float) screenH);
    glUniformMatrix4fv(uMvpMatrixLoc, 1, GL_FALSE, &mMvpMatrix[0][0]);

    glUniform2fv(uTexSizeLoc, 1, &glm::vec2(mRenderImages[1].width, mRenderImages[1].height)[0]);
    glUniform1f(uNeedRotateLoc, 0.0f);
    glUniform1f(uRotateAngleLoc, mTransX / 20);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mImageTexIds[1]);
    glUniform1i(uTextureLoc, 1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    // 前景层
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(mBlurProgramObj);

    scaleLevel = static_cast<float>(1.0f + dScaleLevel * pow(-1, mFrameIndex / 200));
    scaleLevel = scaleLevel < 1.0 ? scaleLevel + 0.2f : scaleLevel;
    mScaleY = mScaleX = scaleLevel + 0.8f;
    UpdateMVPMatrix(mMvpMatrix, mAngleX, mAngleY, mTransX * 2.5f, mTransY * 2.5f, (float) screenW / (float) screenH);
    glUniformMatrix4fv(uBlurMvpMatrixLoc, 1, GL_FALSE, &mMvpMatrix[0][0]);

//    glActiveTexture(GL_TEXTURE2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageTexIds[2]);
//    glUniform1i(uBlurTextureLoc, 2);
    glUniform1i(uBlurTextureLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    glDisable(GL_BLEND);

    mFrameIndex++;
}

void AvatarSample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        glDeleteBuffers(3, mVboIds);
        glDeleteVertexArrays(1, &mVaoId);
        glDeleteTextures(RENDER_IMG_NUM, mImageTexIds);
        mProgramObj = GL_NONE;
    }
    if (mBlurProgramObj) {
        glDeleteProgram(mBlurProgramObj);
        mBlurProgramObj = GL_NONE;
    }
}

void AvatarSample::SetImageDataWithIndex(int index, NativeImage *pImage) {
    if (pImage) {
        mRenderImages[index].width = pImage->width;
        mRenderImages[index].height = pImage->height;
        mRenderImages[index].format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &mRenderImages[index]);
    }
}

void AvatarSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float transX,
                                   float transY, float ratio) {
    LOGCATE("AvatarSample::UpdateMVPMatrix angleX = %d, angleY = %d, ratio = %f", angleX, angleY, ratio);
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
    Model = glm::translate(Model, glm::vec3(transX, transY, 0.0f));

    mvpMatrix = Projection * View * Model;
}
