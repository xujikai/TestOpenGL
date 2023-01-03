//
// Created by Administrator on 2023/1/3.
//

#include "Transition04Sample.h"

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

Transition04Sample::~Transition04Sample() {
    for (int i = 0; i < RENDER_IMG_NUM; ++i) {
        NativeImageUtil::FreeNativeImage(&mRenderImages[i]);
    }
}

void Transition04Sample::Init() {
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
        precision mediump float;
        in vec2 vTexCoord;
        layout(location = 0) out vec4 outColor;
        uniform sampler2D uTexture0;
        uniform sampler2D uTexture1;
        uniform float uOffset;
        const float count = 10.0;
        const float smoothness = 0.5;

        vec4 getFromColor(vec2 uv) {
            return texture(uTexture0, uv);
        }
        vec4 getToColor(vec2 uv) {
            return texture(uTexture1, uv);
        }
        vec4 transition(vec2 uv) {
            float pr = smoothstep(-smoothness, 0.0, uv.x - uOffset * (1.0 + smoothness));
            float s = step(pr, fract(count * uv.x));
            return mix(getFromColor(uv), getToColor(uv), s);
        }
        void main() {
            outColor = transition(vTexCoord);
        }
    )";

    mProgramObj = GLUtils::CreateProgram(vShaderStr.data(), fShaderStr.data());

    uMvpMatrixLoc = glGetUniformLocation(mProgramObj, "uMvpMatrix");
    uTexture0Loc = glGetUniformLocation(mProgramObj, "uTexture0");
    uTexture1Loc = glGetUniformLocation(mProgramObj, "uTexture1");
    uOffsetLoc = glGetUniformLocation(mProgramObj, "uOffset");
    uTexSizeLoc = glGetUniformLocation(mProgramObj, "uTexSize");

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

    for (int i = 0; i < RENDER_IMG_NUM; ++i) {
        glGenTextures(1, &mImageTexIds[i]);
        glBindTexture(GL_TEXTURE_2D, mImageTexIds[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }
}

void Transition04Sample::Draw(int screenW, int screenH) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(mProgramObj);
    glBindVertexArray(mVaoId);

    UpdateMVPMatrix(mMvpMatrix, mAngleX, mAngleY, (float) screenW / (float) screenH);
    glUniformMatrix4fv(uMvpMatrixLoc, 1, GL_FALSE, &mMvpMatrix[0][0]);

    for (int i = 0; i < RENDER_IMG_NUM; ++i) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mImageTexIds[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImages[i].width, mRenderImages[i].height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, mRenderImages[i].ppPlane[0]);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }
    mFrameIndex++;
    if (mFrameIndex % RENDER_LOOP_NUM == 0) {
        mLoopNum++;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageTexIds[mLoopNum % RENDER_IMG_NUM]);
    glUniform1i(uTexture0Loc, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mImageTexIds[(mLoopNum + 1) % RENDER_IMG_NUM]);
    glUniform1i(uTexture1Loc, 1);

    float offset = (mFrameIndex % RENDER_LOOP_NUM) * 1.0f / RENDER_LOOP_NUM;
    glUniform2fv(uTexSizeLoc, 1, &glm::vec2(mRenderImages[0].width, mRenderImages[1].height)[0]);
    glUniform1f(uOffsetLoc, offset);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void Transition04Sample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        glDeleteBuffers(3, mVboIds);
        glDeleteVertexArrays(1, &mVaoId);
        glDeleteTextures(RENDER_IMG_NUM, mImageTexIds);
        mProgramObj = GL_NONE;
    }
}

void Transition04Sample::SetImageDataWithIndex(int index, NativeImage *pImage) {
    if (pImage) {
        mRenderImages[index].width = pImage->width;
        mRenderImages[index].height = pImage->height;
        mRenderImages[index].format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &mRenderImages[index]);
    }
}

void Transition04Sample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio) {
    LOGCATE("Transition04Sample::UpdateMVPMatrix angleX = %d, angleY = %d, ratio = %f", angleX, angleY, ratio);
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
    Model = glm::scale(Model, glm::vec3(1.0f, 1.0f, 1.0f));
    Model = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    Model = glm::rotate(Model, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));

    mvpMatrix = Projection * View * Model;
}