//
// Created by Administrator on 2022/12/30.
//

#ifndef TESTOPENGL_TEXTRENDERSAMPLE_H
#define TESTOPENGL_TEXTRENDERSAMPLE_H

#include "./GLSampleBase.h"
#include <ft2build.h>
#include <freetype/ftglyph.h>
#include <map>
#include <gtc/matrix_transform.hpp>

struct Character {
    GLuint texId;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};

class TextRenderSample : public GLSampleBase {
public:
    void Init() override;

    void Draw(int screenW, int screenH) override;

    void UnInit() override;

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

private:
    void LoadFacesByASCII();
    void LoadFacesByUnicode(const wchar_t* text, int size);

    void RenderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale,
                    const glm::vec3& color, const glm::vec2& viewport);
    void RenderText(const wchar_t* text, int textLen, GLfloat x, GLfloat y, GLfloat scale,
                    const glm::vec3& color, const glm::vec2& viewport);

    GLint uMvpMatrixLoc;
    GLint uTextureLoc;
    GLint uTextColorLoc;

    GLuint mVaoId;
    GLuint mVboId;

    glm::mat4 mMvpMatrix;

    int mAngleX = 0;
    int mAngleY = 0;

    std::map<GLint, Character> mCharacterMap;
};


#endif //TESTOPENGL_TEXTRENDERSAMPLE_H
