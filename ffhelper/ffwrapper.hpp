#pragma once

#include <stdint.h>
#include <memory>
#include <list>

#ifdef __cplusplus
extern "C" 
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
}
#endif 

#include "pub.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

class CAVCodecContext
{
public:
    CAVCodecContext(const AVCodec* codec);
    ~CAVCodecContext();

    AVCodecContext *get();
    AVCodecContext *codec_ctx{nullptr};
private:
    CAVCodecContext(CAVCodecContext&) = delete;
    CAVCodecContext& operator=(CAVCodecContext&) = delete;
    AVCodecContext *codec_ctx_backup__{nullptr};
};

class CAVFormatContext
{
public:
    enum ALLOC_SRC
    {
        SRC_NONE,
        SRC_AVFORMAT_ALLOC_CONTEXT,
        SRC_AVFORMAT_OPEN_INPUT,
    };

    CAVFormatContext();
    ~CAVFormatContext();

    AVFormatContext *get();
    AVFormatContext *fmt_ctx{nullptr};
    ALLOC_SRC alloc_src{SRC_NONE};
private:
    CAVFormatContext(CAVFormatContext&) = delete;
    CAVFormatContext& operator=(CAVFormatContext&) = delete;

    AVFormatContext *fmt_ctx_backup__{nullptr};
};





class InputStream 
{
public:
    // AVStream *st;
    AVStream *st{nullptr};

    // AVCodecContext *dec_ctx;
    std::shared_ptr<CAVCodecContext> dec_ctx;
};

class InputFile 
{
public:
    // AVFormatContext *fmt_ctx;
    std::shared_ptr<CAVFormatContext> fmt_ctx;

    // InputStream *streams;
    std::list<InputStream> streams;
};

int ffwrapper_open_file(const std::string &file, std::shared_ptr<InputFile> &input_file);

NAMESPACE_FFMPEG_DEMO_END