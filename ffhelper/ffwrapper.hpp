#pragma once

#include <memory>
#include <list>

#ifdef __cplusplus
extern "C" 
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#endif 

class CAVCodecContext
{
public:
    CAVCodecContext(const AVCodec* codec);
    ~CAVCodecContext();

    AVCodecContext *get();
    AVCodecContext *codec_ctx__{nullptr};
private:
    CAVCodecContext(CAVCodecContext&) = delete;
    CAVCodecContext& operator=(CAVCodecContext&) = delete;
    AVCodecContext *codec_ctx_backup__{nullptr};
};

class CAVFormatContext
{
public:
    CAVFormatContext();
    ~CAVFormatContext();

    AVFormatContext *get();
    AVFormatContext *fmt_ctx__{nullptr};
private:
    CAVFormatContext(CAVFormatContext&) = delete;
    CAVFormatContext& operator=(CAVFormatContext&) = delete;

    AVFormatContext *fmt_ctx_backup__{nullptr};
};
