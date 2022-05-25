//
// Created by 徐极凯 on 2022/5/21.
//

#ifndef TESTOPENGL_MEDIACAMERARENDER_H
#define TESTOPENGL_MEDIACAMERARENDER_H

#include <thread>
#include "../util/ImageDef.h"
#include "../util/ByteFlowDef.h"
#include "../util/GLUtils.h"
#include <GLES3/gl3.h>

#define TEXTURE_NUM 3

typedef void (*OnRenderFrameCallback)(void *, NativeImage *);

class MediaCameraRender {
    MediaCameraRender();
    ~MediaCameraRender();
public:
    //初始化预览帧的宽高
    virtual void Init(int videoWidth, int videoHeight, int *dstSize);
    //渲染一帧视频
    virtual void RenderVideoFrame(NativeImage *pImage);
    virtual void UnInit();

    //GLSurfaceView 的三个回调
    virtual void OnSurfaceCreated();
    virtual void OnSurfaceChanged(int width, int height);
    virtual void OnDrawFrame();

    //更新变换矩阵，Camera预览帧需要进行旋转
    virtual void UpdateMVPMatrix(int angleX, int angleY, float scaleX, float scaleY);
    virtual void UpdateMVPMatrix(TransformMatrix * pTransformMatrix);

    void SetRenderCallback(void *ctx, OnRenderFrameCallback callback) {
        m_CallbackContext = ctx;
        m_RenderFrameCallback = callback;
    }

    virtual bool CreateFrameBufferObj();
    void GetRenderFrameFromFBO();

    static MediaCameraRender *GetInstance();
    static void DestroyInstance();

private:
    static MediaCameraRender* m_pSample;
    static std::mutex m_Mutex;

    int m_SurfaceWidth;
    int m_SurfaceHeight;

    GLuint m_ProgramObj = GL_NONE;
    GLuint m_FboProgramObj = GL_NONE;
    GLuint m_TextureIds[TEXTURE_NUM];
    GLuint m_VaoId = GL_NONE;
    GLuint m_VboIds[3];
    GLuint m_SrcFboId = GL_NONE;
    GLuint m_SrcFboTextureId = GL_NONE;
    GLuint m_DstFboId = GL_NONE;
    GLuint m_DstFboTextureId = GL_NONE;
    NativeImage m_RenderImage;

    glm::mat4 m_MVPMatrix;
    TransformMatrix m_TransformMatrix;

    OnRenderFrameCallback m_RenderFrameCallback = nullptr;
    void *m_CallbackContext = nullptr;
};


#endif //TESTOPENGL_MEDIACAMERARENDER_H
