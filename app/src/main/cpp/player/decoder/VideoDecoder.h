//
// Created by Administrator on 2023/3/29.
//

#ifndef TESTOPENGL_VIDEODECODER_H
#define TESTOPENGL_VIDEODECODER_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include <libavutil/time.h>
#include <libavcodec/jni.h>
}
#include <string>
#include "../../util/LogUtil.h"
#include "../../util/ImageDef.h"

using namespace std;

class VideoDecoder {
public:
    void InitDecoder();

    void UnInitDecoder();

    void OnDecoderReady();

    void OnDecoderDone();

    int DecodeOnePacket();

    void OnFrameAvailable(AVFrame *pFrame);

private:
    AVFormatContext *pFormatCtx = nullptr;
    AVCodec *pCodec = nullptr;
    AVCodecContext *pCodecCtx = nullptr;
    AVPacket *pPacket = nullptr;
    AVFrame *pFrame = nullptr;

    const AVPixelFormat DST_PIX_FMT = AV_PIX_FMT_RGBA;
    AVFrame *pDstFrame = nullptr;
    uint8_t *pDstFrameBuf = nullptr;
    SwsContext *pSwsCtx = nullptr;

    char mVideoUrl[1024] = {0};
    int mStreamIndex = -1;
    long mDuration = 0L; // 视频总时长 ms

    int mVideoWidth = 0;
    int mVideoHeight = 0;
    int mDstWidth = 0;
    int mDstHeight = 0;
};


#endif //TESTOPENGL_VIDEODECODER_H
