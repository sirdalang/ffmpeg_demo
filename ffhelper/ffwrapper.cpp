#include "ffwrapper.hpp"

#include "log.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

CAVCodecContext::CAVCodecContext(const AVCodec* codec)
{
    codec_ctx = avcodec_alloc_context3(codec);
    if (nullptr == codec_ctx)
    {
        xerror("alloc codec context failed\n");
    }

    codec_ctx_backup__ = codec_ctx;
}

CAVCodecContext::~CAVCodecContext()
{
    if (codec_ctx_backup__ != codec_ctx)
    {
        xerror("pointer changed\n");
    }

    if (nullptr != codec_ctx)
    {
        avcodec_free_context(& codec_ctx);
    }
}

AVCodecContext *CAVCodecContext::get()
{
    return codec_ctx;
}

CAVFormatContext::CAVFormatContext()
{
    fmt_ctx = avformat_alloc_context();
    if (nullptr == fmt_ctx)
    {
        xerror("alloc format context failed\n");
    }
    else
    {
        alloc_src = SRC_AVFORMAT_ALLOC_CONTEXT;
    }
    fmt_ctx_backup__ = fmt_ctx;
}

CAVFormatContext::~CAVFormatContext()
{
    if (fmt_ctx_backup__ != fmt_ctx)
    {
        xerror("pointer changed\n");
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

AVFormatContext *CAVFormatContext::get()
{
    return fmt_ctx;
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

    ifile->fmt_ctx = std::make_shared<CAVFormatContext>();

    if ((ret = avformat_open_input(&ifile->fmt_ctx->fmt_ctx, filename,
            iformat, &format_opts)) < 0)
    {
        xerror("open input failed\n");
        return ret;
    }
    else
    {
        ifile->fmt_ctx->alloc_src = CAVFormatContext::SRC_AVFORMAT_OPEN_INPUT;
    }

    av_dump_format(ifile->fmt_ctx->get(), 0, filename, 0);

    for (i = 0; i < ifile->fmt_ctx->get()->nb_streams; ++i)
    {
        InputStream istream;

        AVStream *stream = ifile->fmt_ctx->get()->streams[i];
        const AVCodec *codec = nullptr;

        istream.st = stream;

        if (AV_CODEC_ID_PROBE == stream->codecpar->codec_id)
        {
            xerror("Failed to probe codec for input stream %d\n", 
                stream->index);
            continue;
        }

        codec = avcodec_find_decoder(stream->codecpar->codec_id);
        if (nullptr == codec)
        {
            xerror("Unsupported codec with id %d for input stream %d\n",
                stream->codecpar->codec_id, stream->index);
            continue;
        }

        istream.dec_ctx = std::make_shared<CAVCodecContext>(codec);

        if (avcodec_open2(istream.dec_ctx->get(), codec, nullptr) < 0)
        {
            xerror ("Open codec for input stream %d failed\n",
                stream->index);
            fail_flag = true;
            break;
        }

        ifile->streams.push_back(istream);

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

