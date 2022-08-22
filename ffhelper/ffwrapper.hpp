#pragma once

#include <stdint.h>
#include <memory>
#include <list>
#include <string>

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

class UniqueObj
{
public:
    UniqueObj() = default;
    ~UniqueObj() = default;
    UniqueObj(UniqueObj&) = delete;
    // UniqueObj(UniqueObj&&) = delete;
    UniqueObj& operator=(UniqueObj&) = delete;
    // UniqueObj& operator=(UniqueObj&&) = delete;
};

class WrapAVCodecContext : public UniqueObj
{
public:
    WrapAVCodecContext(const AVCodec* codec);
    ~WrapAVCodecContext();

    AVCodecContext *get();
    AVCodecContext *codec_ctx{nullptr};
private:
    AVCodecContext *codec_ctx_backup__{nullptr};
};

class WrapAVFormatContext : public UniqueObj
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

class WrapAVPacket : public UniqueObj
{
public:
    WrapAVPacket();
    ~WrapAVPacket();

    AVPacket *get();
private:
    AVPacket *av_packet__{nullptr};
    AVPacket *av_packet_backup__{nullptr};
};

class WrapAVFrame : public UniqueObj
{
public:
    WrapAVFrame();
    ~WrapAVFrame();
    AVFrame *get();
private:
    AVFrame *av_frame__{nullptr};
    AVFrame *av_frame_backup__{nullptr};
};

class InputStream : public UniqueObj
{
public:
    InputStream() = default;

    // AVStream *st;
    AVStream *st{nullptr};  // a pointer to avformatcontext

    // AVCodecContext *dec_ctx;
    std::shared_ptr<WrapAVCodecContext> dec_ctx;
};

class InputFile : public UniqueObj
{
public:
    // AVFormatContext *fmt_ctx;
    std::shared_ptr<WrapAVFormatContext> fmt_ctx;

    // InputStream *streams;
    std::list<std::shared_ptr<InputStream>> streams;
};

int ffwrapper_open_file(const std::string &file, std::shared_ptr<InputFile> &input_file);

NAMESPACE_FFMPEG_DEMO_END