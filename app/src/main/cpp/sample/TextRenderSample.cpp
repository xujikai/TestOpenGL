//
// Created by Administrator on 2022/12/30.
//

#include "TextRenderSample.h"

static const wchar_t BYTE_FLOW[] = L"来源于微信公众号字节流动，欢迎关注。";

void TextRenderSample::Init() {
    std::string vShaderStr = R"(
        #version 300 es
        layout(location = 0) in vec4 aPosition;
        uniform mat4 uMvpMatrix;
        out vec2 vTexCoord;

        void main() {
            gl_Position = uMvpMatrix * vec4(aPosition.xy, 0.0, 1.0);
            vTexCoord = aPosition.zw;
        }
    )";
    std::string fShaderStr = R"(
        #version 300 es
        precision mediump float;
        in vec2 vTexCoord;
        layout(location = 0) out vec4 outColor;
        uniform sampler2D uTexture;
        uniform vec3 uTextColor;

        void main() {
            vec4 color = vec4(1.0, 1.0, 1.0, texture(uTexture, vTexCoord).r);
            outColor = vec4(uTextColor, 1.0) * color;
        }
    )";

    mProgramObj = GLUtils::CreateProgram(vShaderStr.data(), fShaderStr.data());

    uMvpMatrixLoc = glGetUniformLocation(mProgramObj, "uMvpMatrix");
    uTextureLoc = glGetUniformLocation(mProgramObj, "uTexture");
    uTextColorLoc = glGetUniformLocation(mProgramObj, "uTextColor");

    glGenBuffers(1, &mVboId);
    glBindBuffer(GL_ARRAY_BUFFER, mVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);

    glBindBuffer(GL_ARRAY_BUFFER, mVboId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindVertexArray(GL_NONE);

    LoadFacesByASCII();
    LoadFacesByUnicode(BYTE_FLOW, sizeof(BYTE_FLOW) / sizeof(BYTE_FLOW[0]) - 1);
}

void TextRenderSample::Draw(int screenW, int screenH) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    UpdateMVPMatrix(mMvpMatrix, mAngleX, mAngleY, (float) screenW / (float) screenH);
    glUniformMatrix4fv(uMvpMatrixLoc, 1, GL_FALSE, &mMvpMatrix[0][0]);

    glm::vec2 viewport = glm::vec2(screenW, screenH);
    RenderText("Hello Jams", -0.9f, 0.2f, 1.0f, glm::vec3(0.8f, 0.1f, 0.1f), viewport);
    RenderText("Welcome to my house", -0.9f, 0.0f, 2.0f, glm::vec3(0.2f, 0.4f, 0.7f), viewport);
    RenderText("Quick see see book", -0.9f, -0.2f, 1.5f, glm::vec3(0.2f, 0.8f, 0.2f), viewport);
    RenderText(BYTE_FLOW, sizeof(BYTE_FLOW) / sizeof(BYTE_FLOW[0]) - 1, -0.9f, -0.4f, 1.0f,
               glm::vec3(0.7f, 0.4f, 0.2f), viewport);
}

void TextRenderSample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        glDeleteBuffers(1, &mVboId);
        glDeleteVertexArrays(1, &mVaoId);
        std::map<GLint, Character>::const_iterator iter;
        for (iter = mCharacterMap.begin(); iter != mCharacterMap.end(); iter++) {
            glDeleteTextures(1, &mCharacterMap[iter->first].texId);
        }
        mProgramObj = GL_NONE;
    }
}

void TextRenderSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio) {
    LOGCATE("TextRenderSample::UpdateMVPMatrix angleX = %d, angleY = %d, ratio = %f", angleX, angleY, ratio);
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

void TextRenderSample::LoadFacesByASCII() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        LOGCATE("TextRenderSample::LoadFacesByASCII FREETYPE: Could not init FreeType Library");
    }

    FT_Face face;
    std::string path = "/sdcard/Android/data/com.app.testopengl/files/Download/fonts/Antonio-Regular.ttf";
    if (FT_New_Face(ft, path.data(), 0, &face)) {
        LOGCATE("TextRenderSample::LoadFacesByASCII FREETYPE: Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, 0, 96);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            LOGCATE("TextRenderSample::LoadFacesByASCII FREETYTPE: Failed to load Glyph");
            continue;
        }
        GLuint texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                     0, GL_LUMINANCE, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
                texId, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<GLuint>(face->glyph->advance.x)
        };
        mCharacterMap.insert(std::pair<GLint, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderSample::LoadFacesByUnicode(const wchar_t *text, int size) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        LOGCATE("TextRenderSample::LoadFacesByUnicode FREETYPE: Could not init FreeType Library");
    }

    FT_Face face;
    std::string path = "/sdcard/Android/data/com.app.testopengl/files/Download/fonts/msyh.ttc";
    if (FT_New_Face(ft, path.data(), 0, &face)) {
        LOGCATE("TextRenderSample::LoadFacesByUnicode FREETYPE: Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, 96, 96);
    FT_Select_Charmap(face, ft_encoding_unicode);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (int i = 0; i < size; i++) {
        if (FT_Load_Glyph(face, FT_Get_Char_Index(face, text[i]), FT_LOAD_DEFAULT)) {
            LOGCATE("TextRenderSample::LoadFacesByUnicode FREETYTPE: Failed to load Glyph");
            continue;
        }

        FT_Glyph glyph;
        FT_Get_Glyph(face->glyph, &glyph);
        FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
        auto bitmapGlyph = (FT_BitmapGlyph) glyph;
        FT_Bitmap& bitmap = bitmapGlyph->bitmap;

        GLuint texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, bitmap.width, bitmap.rows, 0,
                     GL_LUMINANCE, GL_UNSIGNED_BYTE, bitmap.buffer);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
                texId, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<GLuint>((glyph->advance.x / 65536) << 6)
        };
        mCharacterMap.insert(std::pair<GLint, Character>(text[i], character));
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderSample::RenderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale,
                                  const glm::vec3& color, const glm::vec2& viewport) {
    glUseProgram(mProgramObj);
    glBindVertexArray(mVaoId);

    glUniform3fv(uTextColorLoc, 1, &color[0]);

    std::string::const_iterator c;
    x *= viewport.x;
    y *= viewport.y;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = mCharacterMap[*c];

        GLfloat xPos = x + ch.bearing.x * scale;
        GLfloat yPos = y - (ch.size.y - ch.bearing.y) * scale;
        xPos /= viewport.x;
        yPos /= viewport.y;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;
        w /= viewport.x;
        h /= viewport.y;

        GLfloat vertices[6][4] = {
                {xPos, yPos + h,    0.0f, 0.0f},
                {xPos, yPos,        0.0f, 1.0f},
                {xPos + w, yPos,    1.0f, 1.0f},
                {xPos, yPos + h,    0.0f, 0.0f},
                {xPos + w, yPos,    1.0f, 1.0f},
                {xPos + w, yPos + h, 1.0f, 0.0f}
        };
        glBindBuffer(GL_ARRAY_BUFFER, mVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.texId);
        glUniform1i(uTextureLoc, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(GL_NONE);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void TextRenderSample::RenderText(const wchar_t *text, int textLen, GLfloat x, GLfloat y, GLfloat scale,
                             const glm::vec3 &color, const glm::vec2 &viewport) {
    glUseProgram(mProgramObj);
    glBindVertexArray(mVaoId);

    glUniform3fv(uTextColorLoc, 1, &color[0]);

    x *= viewport.x;
    y *= viewport.y;
    for (int i = 0; i < textLen; i++) {
        Character ch = mCharacterMap[text[i]];

        GLfloat xPos = x + ch.bearing.x * scale;
        GLfloat yPos = y - (ch.size.y - ch.bearing.y) * scale;
        xPos /= viewport.x;
        yPos /= viewport.y;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;
        w /= viewport.x;
        h /= viewport.y;

        GLfloat vertices[6][4] = {
                {xPos, yPos + h,    0.0f, 0.0f},
                {xPos, yPos,        0.0f, 1.0f},
                {xPos + w, yPos,    1.0f, 1.0f},
                {xPos, yPos + h,    0.0f, 0.0f},
                {xPos + w, yPos,    1.0f, 1.0f},
                {xPos + w, yPos + h, 1.0f, 0.0f}
        };
        glBindBuffer(GL_ARRAY_BUFFER, mVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.texId);
        glUniform1i(uTextureLoc, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(GL_NONE);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}
