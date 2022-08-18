#include "ffprobe_demo.hpp"

#include "log.hpp"

#define UNKNOWN "unknown"

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
                        cd->long_name ? cd->long_name : UNKNOWN);
        } 
        else 
        {
            xinfo("codec_name:%s\n", UNKNOWN);
            xinfo("codec_long_name:%s\n", UNKNOWN);
        }
    }

    return 0;
}

void FFProbeDemo::getInfo(std::shared_ptr<MediaInfo> &info)
{
    info__.reset();
    info__ = std::make_shared<MediaInfo>();

    getStreamInfo(*info__);
    info = info__;
    info__.reset();
}

void FFProbeDemo::getStreamInfo(MediaInfo &info)
{
    int index = 0;
    char mainkey[256];
    const char *keyname = nullptr;

    const AVCodecDescriptor *cd;
    AVCodecParameters *par;
    const char *cstr = nullptr;

    auto &ifile = input_file__;

    index = 0;
    for (auto it = ifile->streams.begin(); it != ifile->streams.end(); ++it)
    {
        StreamInfo stinfo;

        par = it->st->codecpar;
        
        addStreamInfo(stinfo, "index", it->st->index);
        addStreamInfo(stinfo, "id", it->st->id);

        if (cd = avcodec_descriptor_get(par->codec_id)) 
        {
            addStreamInfo(stinfo, "codec_name", cd->name);
            addStreamInfo(stinfo, "codec_long_name", 
                cd->long_name ? cd->long_name : UNKNOWN);
        }
        else 
        {
            addStreamInfo(stinfo, "codec_name", UNKNOWN);
            addStreamInfo(stinfo, "codec_long_name", UNKNOWN);
        }

        keyname = "profile";
        if (cstr = avcodec_profile_name(par->codec_id, par->profile))
        {
            addStreamInfo(stinfo, keyname, cstr);
        }
        else
        {
            if (par->profile != FF_PROFILE_UNKNOWN)
            {
                addStreamInfo(stinfo, keyname, par->profile);
            }
            else
            {
                addStreamInfo(stinfo, keyname, UNKNOWN);
            }
        }

        keyname = "codec_type";
        cstr = av_get_media_type_string(par->codec_type);
        if (cstr)
        {
            addStreamInfo(stinfo, keyname, cstr);
        }
        else
        {
            addStreamInfo(stinfo, keyname, UNKNOWN);
        }

        switch (par->codec_type)
        {
            case AVMEDIA_TYPE_VIDEO:
            {
                addStreamInfo (stinfo, "width", par->width);
                break;
            }
        }

        snprintf (mainkey, sizeof(mainkey), "stream_index_%d", index);
        info.push_back(std::make_pair(std::string(mainkey), stinfo));

        ++index;
    }

    return ;
}

void FFProbeDemo::addStreamInfo(StreamInfo& ref, 
    const char *key, const char *value)
{
    std::string str_key(key);
    std::string str_value(value);

    ref.push_back(std::make_pair(str_key, str_value));
}

void FFProbeDemo::addStreamInfo(StreamInfo& ref, 
    const char *key, int value)
{
    char cs_value[64];
    snprintf (cs_value, sizeof(cs_value), "%d", value);

    std::string str_key(key);
    std::string str_value(cs_value);

    ref.push_back(std::make_pair(str_key, str_value));
}

NAMESPACE_FFMPEG_DEMO_END