#include "ffprobe_demo.hpp"

#include "log.hpp"

#define STR_BUF_SIZE    (64)
#define UNKNOWN         "unknown"
#define NUM_UNKNOWN     (-1)

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
    char mainkey[STR_BUF_SIZE];
    char buffer[STR_BUF_SIZE];
    const char *keyname = nullptr;

    const AVCodecDescriptor *cd = nullptr;
    AVCodecParameters *par = nullptr;
    AVCodecContext *dec_ctx = nullptr;
    const char *cstr = nullptr;

    auto &ifile = input_file__;

    index = 0;
    for (auto it = ifile->streams.begin(); it != ifile->streams.end(); ++it)
    {
        StreamInfo stinfo;

        par = it->st->codecpar;
        dec_ctx = it->dec_ctx->get();
        
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

#if 1
        cstr = avcodec_profile_name(par->codec_id, par->profile);
        addStreamInfo(stinfo, "profile", cstr ? cstr : UNKNOWN);
#else    
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
#endif

        cstr = av_get_media_type_string(par->codec_type);
        addStreamInfo(stinfo, "codec_type", cstr ? cstr : UNKNOWN);

        switch (par->codec_type)
        {
            case AVMEDIA_TYPE_VIDEO:
            {
                addStreamInfo(stinfo, "width", par->width);
                addStreamInfo(stinfo, "height", par->height);
                addStreamInfo(stinfo, "coded_width", dec_ctx->coded_width);
                addStreamInfo(stinfo, "coded_height", dec_ctx->coded_height);
                addStreamInfo(stinfo, "has_b_frames", par->video_delay);

                cstr = av_get_pix_fmt_name((AVPixelFormat)par->format);
                addStreamInfo(stinfo, "pix_fmt", cstr ? cstr : UNKNOWN);
                addStreamInfo(stinfo, "level", par->level);

                cstr = av_color_range_name(par->color_range);
                addStreamInfo(stinfo, "color_range", cstr ? cstr : UNKNOWN);

                cstr = av_color_space_name(par->color_space);
                addStreamInfo(stinfo, "color_space", cstr ? cstr : UNKNOWN);

                cstr = av_color_transfer_name(par->color_trc);
                addStreamInfo(stinfo, "color_transfer", cstr ? cstr : UNKNOWN);

                cstr = av_color_primaries_name(par->color_primaries);
                addStreamInfo(stinfo, "color_primaries", cstr ? cstr : UNKNOWN);

                cstr = av_chroma_location_name(par->chroma_location);
                addStreamInfo(stinfo, "chroma_location", cstr ? cstr : UNKNOWN);
                break;
            }
            case AVMEDIA_TYPE_AUDIO:
            {
                cstr = av_get_sample_fmt_name((AVSampleFormat)par->format);
                addStreamInfo(stinfo, "sample_fmt", cstr ? cstr : UNKNOWN);

                addStreamInfo(stinfo, "sample_rate", par->sample_rate);
                addStreamInfo(stinfo, "channels", par->ch_layout.nb_channels);
                addStreamInfo(stinfo, "bits_per_sample", av_get_bits_per_sample(par->codec_id));

                break;
            }
        }

        addStreamInfo(stinfo, "r_frame_rate", it->st->r_frame_rate);
        addStreamInfo(stinfo, "avg_frame_rate", it->st->avg_frame_rate);
        addStreamInfo(stinfo, "time_base", it->st->time_base);
        addStreamInfo(stinfo, "start_pts", it->st->start_time);
        addStreamInfo(stinfo, "duration_ts", it->st->duration);

        addStreamInfo(stinfo, "bit_rate", 
            par->bit_rate > 0 ? par->bit_rate : NUM_UNKNOWN);
        addStreamInfo(stinfo, "bits_per_raw_sample", 
            dec_ctx->bits_per_raw_sample > 0 ?
            dec_ctx->bits_per_raw_sample : NUM_UNKNOWN);
        addStreamInfo(stinfo, "nb_frames", it->st->nb_frames);

        snprintf (mainkey, sizeof(mainkey), "stream_index_%d", index);
        info.push_back(std::make_pair(std::string(mainkey), stinfo));

        ++index;
    }

    return ;
}

void FFProbeDemo::addStreamInfo(StreamInfo& ref, 
    const char *key, const char *value)
{
    ref.push_back(std::make_pair(
        std::string(key), std::string(value)));
}

void FFProbeDemo::addStreamInfo(StreamInfo& ref, 
    const char *key, int value)
{
    char cs_value[STR_BUF_SIZE];
    snprintf (cs_value, sizeof(cs_value), "%d", value);
    ref.push_back(std::make_pair(
        std::string(key), std::string(cs_value)));
}

void FFProbeDemo::addStreamInfo(StreamInfo& ref, 
    const char *key, int64_t value)
{
    char cs_value[STR_BUF_SIZE];
    snprintf (cs_value, sizeof(cs_value), "%ld", value);
    ref.push_back(std::make_pair(
        std::string(key), std::string(cs_value)));
}

void FFProbeDemo::addStreamInfo(StreamInfo &ref, 
    const char *key, const AVRational &r)
{
    char cs_value[STR_BUF_SIZE];
    snprintf (cs_value, sizeof(cs_value), "%d/%d", r.num, r.den);
    ref.push_back(std::make_pair(
            std::string(key), std::string(cs_value)));
}

NAMESPACE_FFMPEG_DEMO_END