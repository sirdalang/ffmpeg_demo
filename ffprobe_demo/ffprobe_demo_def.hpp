#pragma once

#include "log.hpp"

#include "ffwrapper.hpp"

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