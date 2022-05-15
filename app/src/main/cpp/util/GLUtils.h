/**
 *
 * Created by 公众号：字节流动 on 2021/3/12.
 * https://github.com/githubhaohao/NDK_OpenGLES_3_0
 * 最新文章首发于公众号：字节流动，有疑问或者技术交流可以添加微信 Byte-Flow ,领取视频教程, 拉你进技术交流群
 *
 * */

#ifndef _BYTE_FLOW_GL_UTILS_H_
#define _BYTE_FLOW_GL_UTILS_H_

#include <GLES3/gl3.h>
#include <string>

#define SHADER_TO_STRING(s) #s

#define MATH_PI 3.1415926535897932384626433832802

class GLUtils {
public:
    static GLuint LoadShader(GLenum shaderType, const char *pSource);

    static GLuint CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource,
                                GLuint &vertexShaderHandle,
                                GLuint &fragShaderHandle);

    static GLuint CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource);

    static GLuint CreateProgramWithFeedback(
            const char *pVertexShaderSource,
            const char *pFragShaderSource,
            GLuint &vertexShaderHandle,
            GLuint &fragShaderHandle,
            const GLchar **varying,
            int varyingCount);

    static void DeleteProgram(GLuint &program);

    static void CheckGLError(const char *pGLOperation);

};

#endif // _BYTE_FLOW_GL_UTILS_H_