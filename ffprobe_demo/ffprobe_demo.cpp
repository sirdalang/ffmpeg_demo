#include "ffprobe_demo.hpp"



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

int FFProbeDemo::showStreams()
{
    std::shared_ptr<InputFile> &ifile = input_file__;

    for (auto it = ifile->streams.begin(); it != ifile->streams.end(); ++it)
    {
        AVCodecParameters *par = nullptr;
        AVStream *stream = it->st;
        AVCodecContext *dec_ctx = nullptr;
        const AVCodecDescriptor *cd;

        par = stream->codecpar;
        dec_ctx = it->dec_ctx->get();
        
        if ((cd = avcodec_descriptor_get(par->codec_id))) 
        {
            xinfo("codec_name:%s\n", cd->name);
            xinfo("codec_long_name:%s\n",
                        cd->long_name ? cd->long_name : "unknown");
        } 
        else 
        {
            xinfo("codec_name:%s\n", "unknown");
            xinfo("codec_long_name:%s\n", "unknown");
        }
    }

    return 0;
}

int FFProbeDemo::getContext(std::shared_ptr<InputFile> &input_file)
{
    input_file = input_file__;
    input_file__.reset();

    return 0;
}