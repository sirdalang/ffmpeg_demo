#include "ffprobe_demo.hpp"

#ifdef __cplusplus
extern "C" 
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#endif 

FFProbeDemo::FFProbeDemo (std::string const& filename)
{
    filename__ = filename;

    input_file__ = std::make_shared<InputFile>();
}

int FFProbeDemo::init()
{
    const char *filename = filename__.c_str();
    
    if (openFile() < 0)
    {
        xerror("open file %s failed\n", filename);
        return -1;
    }

    return 0;
}

int FFProbeDemo::openFile()
{
    unsigned int i = 0;
    int ret = 0;
    bool fail_flag = false;

    AVDictionary *format_opts = nullptr;
    AVFormatContext *fmt_ctx = nullptr;
    AVInputFormat *iformat = nullptr;

    std::shared_ptr<InputFile> &ifile = input_file__;
    const char *filename = filename__.c_str();

    ifile->fmt_ctx = std::make_shared<CAVFormatContext>();

    if ((ret = avformat_open_input(&ifile->fmt_ctx->fmt_ctx__, filename,
            iformat, &format_opts)) < 0)
    {
        xerror("open input failed\n");
        return ret;
    }

    av_dump_format(fmt_ctx, 0, filename, 0);

    for (i = 0; i < fmt_ctx->nb_streams; ++i)
    {
        InputStream istream;

        AVStream *stream = fmt_ctx->streams[i];
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

        if (avcodec_open2(istream.dec_ctx->codec_ctx__, codec, nullptr) < 0)
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