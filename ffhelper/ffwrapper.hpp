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

class WrapAVCodecContext
{
public:
    WrapAVCodecContext(const AVCodec* codec);
    ~WrapAVCodecContext();

    AVCodecContext *get();
    AVCodecContext *codec_ctx{nullptr};
private:
    WrapAVCodecContext(WrapAVCodecContext&) = delete;
    WrapAVCodecContext& operator=(WrapAVCodecContext&) = delete;
    AVCodecContext *codec_ctx_backup__{nullptr};
};

class WrapAVFormatContext
{
public:
    enum ALLOC_SRC
    {
        SRC_NONE,
        SRC_AVFORMAT_ALLOC_CONTEXT,
        SRC_AVFORMAT_OPEN_INPUT,
    };

    WrapAVFormatContext();
    ~WrapAVFormatContext();

    AVFormatContext *get();
    AVFormatContext *fmt_ctx{nullptr};
    ALLOC_SRC alloc_src{SRC_NONE};
private:
    WrapAVFormatContext(WrapAVFormatContext&) = delete;
    WrapAVFormatContext& operator=(WrapAVFormatContext&) = delete;

    AVFormatContext *fmt_ctx_backup__{nullptr};
};

class WrapAVPacket
{
public:
    WrapAVPacket();
    ~WrapAVPacket();
    AVPacket *get();
private:
    AVPacket *av_packet__{nullptr};
    AVPacket *av_packet_backup__{nullptr};
};

class WrapAVFrame
{
public:
    WrapAVFrame();
    ~WrapAVFrame();
    AVFrame *get();
private:
    AVFrame *av_frame__{nullptr};
    AVFrame *av_frame_backup__{nullptr};
};



class InputStream 
{
public:
    // AVStream *st;
    AVStream *st{nullptr};

    // AVCodecContext *dec_ctx;
    std::shared_ptr<WrapAVCodecContext> dec_ctx;
};

class InputFile 
{
public:
    // AVFormatContext *fmt_ctx;
    std::shared_ptr<WrapAVFormatContext> fmt_ctx;

    // InputStream *streams;
    std::list<InputStream> streams;
};

int ffwrapper_open_file(const std::string &file, std::shared_ptr<InputFile> &input_file);

NAMESPACE_FFMPEG_DEMO_END