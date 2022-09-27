#include "ffwrapper.hpp"

#include "log.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

WrapAVCodecContext::WrapAVCodecContext(const AVCodec* codec)
{
    codec_ctx = avcodec_alloc_context3(codec);
    if (nullptr == codec_ctx)
    {
        xerror("alloc codec context failed");
    }

    codec_ctx_backup__ = codec_ctx;
}

WrapAVCodecContext::~WrapAVCodecContext()
{
    if (codec_ctx_backup__ != codec_ctx)
    {
        xerror("pointer changed");
    }

    if (nullptr != codec_ctx)
    {
        avcodec_free_context(& codec_ctx);
    }
}

AVCodecContext *WrapAVCodecContext::get()
{
    return codec_ctx;
}

WrapAVFormatContext::WrapAVFormatContext()
{
    fmt_ctx = avformat_alloc_context();
    if (nullptr == fmt_ctx)
    {
        xerror("alloc format context failed");
    }
    else
    {
        alloc_src = SRC_AVFORMAT_ALLOC_CONTEXT;
    }
    fmt_ctx_backup__ = fmt_ctx;
}

WrapAVFormatContext::~WrapAVFormatContext()
{
    if (fmt_ctx_backup__ != fmt_ctx)
    {
        xerror("pointer changed");
    }

    if (nullptr != fmt_ctx)
    {
        if (SRC_AVFORMAT_ALLOC_CONTEXT == alloc_src)
        {
            avformat_free_context(fmt_ctx);
            fmt_ctx = nullptr;
        }
        else if (SRC_AVFORMAT_OPEN_INPUT == alloc_src)
        {
            avformat_close_input(&fmt_ctx);
            fmt_ctx = nullptr;
        }
    }
}

AVFormatContext *WrapAVFormatContext::get()
{
    return fmt_ctx;
}

WrapAVPacket::WrapAVPacket()
{
    av_packet__ = av_packet_alloc();
    if (nullptr == av_packet__)
    {
        xerror ("alloc packet failed");
    }
    av_packet_backup__ = av_packet__;
}

WrapAVPacket::~WrapAVPacket()
{
    if (av_packet__ != av_packet_backup__)
    {
        xerror("pointer changed");
    }

    if (nullptr != av_packet__)
    {
        av_packet_free(&av_packet__);
        av_packet__ = nullptr;
        av_packet_backup__ = nullptr;
    }
}

AVPacket *WrapAVPacket::get()
{
    return av_packet__;
}

WrapAVFrame::WrapAVFrame()
{
    av_frame__ = av_frame_alloc();
    if (nullptr == av_frame__)
    {
        xerror("alloc frame failed");
    }
    av_frame_backup__ = av_frame__;
}

WrapAVFrame::~WrapAVFrame()
{
    if (av_frame__ != av_frame_backup__)
    {
        xerror("pointer changed");
    }

    if (nullptr != av_frame__)
    {
        av_frame_free(&av_frame__);
        av_frame__ = nullptr;
        av_frame_backup__ = nullptr;
    }
}

AVFrame *WrapAVFrame::get()
{
    return av_frame__;
}

int ffwrapper_open_file(const std::string &file, std::shared_ptr<InputFile> &input_file)
{
    unsigned int i = 0;
    int ret = 0;
    bool fail_flag = false;

    AVDictionary *format_opts = nullptr;
    AVInputFormat *iformat = nullptr;

    std::shared_ptr<InputFile> &ifile = input_file;
    const char *filename = file.c_str();

    ifile->fmt_ctx = std::make_shared<WrapAVFormatContext>();

    if ((ret = avformat_open_input(&ifile->fmt_ctx->fmt_ctx, filename,
            iformat, &format_opts)) < 0)
    {
        xerror("open input failed");
        return ret;
    }
    else
    {
        ifile->fmt_ctx->alloc_src = WrapAVFormatContext::SRC_AVFORMAT_OPEN_INPUT;
    }

    if ((ret = avformat_find_stream_info(ifile->fmt_ctx->get(), nullptr)) < 0)
    {
        xerror ("find stream info failed");
        return ret;
    }

    // av_dump_format(ifile->fmt_ctx->get(), 0, filename, 0);

    for (i = 0; i < ifile->fmt_ctx->get()->nb_streams; ++i)
    {
        std::shared_ptr<InputStream> is;

        is = std::make_shared<InputStream>();

        AVStream *stream = ifile->fmt_ctx->get()->streams[i];
        const AVCodec *codec = nullptr;

        is->st = stream;

        if (AV_CODEC_ID_PROBE == stream->codecpar->codec_id)
        {
            xerror("Failed to probe codec for input stream %d", 
                stream->index);
            continue;
        }

        codec = avcodec_find_decoder(stream->codecpar->codec_id);
        if (nullptr == codec)
        {
            xerror("Unsupported codec with id %d for input stream %d",
                stream->codecpar->codec_id, stream->index);
            continue;
        }

        is->dec_ctx = std::make_shared<WrapAVCodecContext>(codec);

        if (avcodec_parameters_to_context(is->dec_ctx->get(),
                stream->codecpar) < 0)
        {
            xerror("avcodec_parameters_to_context failed");
            break;
        }

        if (avcodec_open2(is->dec_ctx->get(), codec, nullptr) < 0)
        {
            xerror ("Open codec for input stream %d failed",
                stream->index);
            fail_flag = true;
            break;
        }

        ifile->streams.push_back(is);

        continue;
    }

    if (fail_flag)
    {
        ifile.reset();
        return -1;
    }

    return 0;
}

NAMESPACE_FFMPEG_DEMO_END

