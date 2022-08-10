#include "ffwrapper.hpp"

#include "log.hpp"

CAVCodecContext::CAVCodecContext(const AVCodec* codec)
{
    codec_ctx__ = avcodec_alloc_context3(codec);
    if (nullptr == codec_ctx__)
    {
        xerror("alloc codec context failed\n");
    }

    codec_ctx_backup__ = codec_ctx__;
}

CAVCodecContext::~CAVCodecContext()
{
    if (codec_ctx_backup__ != codec_ctx__)
    {
        xerror("pointer changed\n");
    }

    if (nullptr != codec_ctx__)
    {
        avcodec_free_context(& codec_ctx__);
    }
}

CAVFormatContext::CAVFormatContext()
{
    fmt_ctx__ = avformat_alloc_context();
    if (nullptr == fmt_ctx__)
    {
        xerror("alloc format context failed\n");
    }
    fmt_ctx_backup__ = fmt_ctx__;
}

CAVFormatContext::~CAVFormatContext()
{
    if (fmt_ctx_backup__ != fmt_ctx__)
    {
        xerror("pointer changed\n");
    }

    if (nullptr != fmt_ctx__)
    {
        avformat_free_context(fmt_ctx__);
        fmt_ctx__ = nullptr;
    }
}