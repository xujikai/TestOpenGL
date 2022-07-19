//
// Created by Administrator on 2022/5/26.
//

#ifndef MOTIONVERSE_RECORDERDEF_H
#define MOTIONVERSE_RECORDERDEF_H

#define RECORDER_TYPE_SINGLE_VIDEO  0 //仅录制视频
#define RECORDER_TYPE_SINGLE_AUDIO  1 //仅录制音频
#define RECORDER_TYPE_AV            2 //同时录制音频和视频,打包成 MP4 文件

#define DEFAULT_SAMPLE_RATE    44100
#define DEFAULT_CHANNEL_LAYOUT AV_CH_LAYOUT_STEREO

#define TEXTURE_NUM 3

#include "ImageDef.h"

typedef void (*OnRenderFrameCallback)(void *, NativeImage *);

#endif //MOTIONVERSE_RECORDERDEF_H
