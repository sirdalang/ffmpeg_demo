#include "ffprobe_demo.hpp"

#include "log.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

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
    int ret = ffwrapper_open_file(filename__, input_file__);

    return ret;
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

NAMESPACE_FFMPEG_DEMO_END