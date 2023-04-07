//
// Created by Administrator on 2023/3/29.
//

#include "VideoDecoder.h"

void VideoDecoder::InitDecoder() {
    int ret = -1;
    do {
        pFormatCtx = avformat_alloc_context();
        avformat_open_input(&pFormatCtx, mVideoUrl, nullptr, nullptr);
        for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
            if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                mStreamIndex = i;
                break;
            }
        }
        if (mStreamIndex == -1) {
            LOGCATE("VideoDecoder::InitDecoder find video stream failed");
            break;
        }

        AVCodecParameters *pCodecPar = pFormatCtx->streams[mStreamIndex]->codecpar;

        pCodec = avcodec_find_decoder(pCodecPar->codec_id);
        if (pCodec == nullptr) {
            LOGCATE("VideoDecoder::InitDecoder find video codec failed");
            break;
        }

        pCodecCtx = avcodec_alloc_context3(pCodec);
        avcodec_parameters_to_context(pCodecCtx, pCodecPar);

        ret = avcodec_open2(pCodecCtx, pCodec, nullptr);
        if (ret < 0) {
            LOGCATE("VideoDecoder::InitDecoder open video codec failed");
            break;
        }

        mDuration = pFormatCtx->duration / AV_TIME_BASE * 1000; // us 转 ms
        pPacket = av_packet_alloc();
        pFrame = av_frame_alloc();
    } while (false);
}

void VideoDecoder::UnInitDecoder() {
    if (pFrame != nullptr) {
        av_frame_free(&pFrame);
        pFrame = nullptr;
    }
    if (pPacket != nullptr) {
        av_packet_free(&pPacket);
        pPacket = nullptr;
    }
    if (pCodecCtx != nullptr) {
        avcodec_close(pCodecCtx);
        avcodec_free_context(&pCodecCtx);
        pCodecCtx = nullptr;
        pCodec = nullptr;
    }
    if (pFormatCtx != nullptr) {
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
}

void VideoDecoder::OnDecoderReady() {
    mVideoWidth = pCodecCtx->width;
    mVideoHeight = pCodecCtx->height;
    mDstWidth = mVideoWidth;
    mDstHeight = mVideoHeight;

    pDstFrame = av_frame_alloc();
    int dstBufSize = av_image_get_buffer_size(DST_PIX_FMT, mDstWidth, mDstHeight, 1);
    pDstFrameBuf = static_cast<uint8_t *>(av_malloc(dstBufSize * sizeof(uint8_t)));
    av_image_fill_arrays(pDstFrame->data, pDstFrame->linesize,
                         pDstFrameBuf, DST_PIX_FMT, mDstWidth, mDstHeight, 1);

    pSwsCtx = sws_getContext(
            mVideoWidth, mVideoHeight, pCodecCtx->pix_fmt,
            mDstWidth, mDstHeight, DST_PIX_FMT,
            SWS_FAST_BILINEAR, nullptr, nullptr, nullptr
    );
}

void VideoDecoder::OnDecoderDone() {
    if (pDstFrame) {
        av_frame_free(&pDstFrame);
        pDstFrame = nullptr;
    }
    if (pDstFrameBuf) {
        av_free(pDstFrameBuf);
        pDstFrameBuf = nullptr;
    }
    if (pSwsCtx) {
        sws_freeContext(pSwsCtx);
        pSwsCtx = nullptr;
    }
}

int VideoDecoder::DecodeOnePacket() {
    int ret = av_read_frame(pFormatCtx, pPacket);
    while (ret == 0) {
        if (pPacket->stream_index == mStreamIndex) {
            if (avcodec_send_packet(pCodecCtx, pPacket) == AVERROR_EOF) {
                ret = -1;
                break;
            }
            int frameCount = 0;
            while (avcodec_receive_frame(pCodecCtx, pFrame) == 0) {
                OnFrameAvailable(pFrame);
                ++frameCount;
            }
            if (frameCount > 0) {
                ret = 0;
                break;
            }
        }
        av_packet_unref(pPacket);
        ret = av_read_frame(pFormatCtx, pPacket);
    }
    av_packet_unref(pPacket);
    return ret;
}

void VideoDecoder::OnFrameAvailable(AVFrame *pFrame) {
    if (pFrame != nullptr) {
        NativeImage image;

    }
}