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
        InputStream* is = it->get();

        AVCodecParameters *par = nullptr;
        AVStream *stream = is->st;
        AVCodecContext *dec_ctx = nullptr;
        const AVCodecDescriptor *cd;

        par = stream->codecpar;
        dec_ctx = is->dec_ctx->get();
        
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

const char *FFProbeDemo::nameOf(MediaInfoType type)
{
    const char *name = "null";
    switch (type)
    {    
        case MediaInfoType::INDEX:
        {
            name = "index";
            break;
        }
        case MediaInfoType::ID:
        {
            name = "id";
            break;
        }
        case MediaInfoType::CODEC_NAME:
        {
            name = "codec_name";
            break;
        }
        case MediaInfoType::CODEC_LONG_NAME:
        {
            name = "codec_long_name";
            break;
        }
        case MediaInfoType::PROFILE:
        {
            name = "profile";
            break;
        }
        case MediaInfoType::CODEC_TYPE:
        {
            name = "codec_type";
            break;
        }
        case MediaInfoType::WIDTH:
        {
            name = "width";
            break;
        }
        case MediaInfoType::HEIGHT:
        {
            name = "height";
            break;
        }
        case MediaInfoType::CODED_WIDTH:
        {
            name = "coded_width";
            break;
        }
        case MediaInfoType::CODED_HEIGHT:
        {
            name = "coded_height";
            break;
        }
        case MediaInfoType::HAS_B_FRAME:
        {
            name = "has_b_frame";
            break;
        }
        case MediaInfoType::PIX_FMT:
        {
            name = "has_b_frames";
            break;
        }
        case MediaInfoType::LEVEL:
        {
            name = "level";
            break;
        }
        case MediaInfoType::COLOR_RANGE:
        {
            name = "color_range";
            break;
        }
        case MediaInfoType::COLOR_SPACE:
        {
            name = "color_space";
            break;
        }
        case MediaInfoType::COLOR_TRANSFER:
        {
            name = "color_transfer";
            break;
        }
        case MediaInfoType::COLOR_PRIMARIES:
        {
            name = "color_primaries";
            break;
        }
        case MediaInfoType::CHROMA_LOCATION:
        {
            name = "chroma_location";
            break;
        }
        case MediaInfoType::SAMPLE_FMT:
        {
            name = "sample_fmt";
            break;
        }
        case MediaInfoType::SAMPLE_RATE:
        {
            name = "sample_rate";
            break;
        }
        case MediaInfoType::BITS_PER_SAMPLE:
        {
            name = "bits_per_sample";
            break;
        }
        case MediaInfoType::R_FRAME_RATE:
        {
            name = "r_frame_rate";
            break;
        }
        case MediaInfoType::AVG_FRAME_RATE:
        {
            name = "avg_frame_rate";
            break;
        }
        case MediaInfoType::TIME_BASE:
        {
            name = "time_base";
            break;
        }
        case MediaInfoType::START_PTS:
        {
            name = "start_pts";
            break;
        }
        case MediaInfoType::DURATION_TS:
        {
            name = "duration_ts";
            break;
        }
        case MediaInfoType::BIT_RATE:
        {
            name = "bit_rate";
            break;
        }
        case MediaInfoType::BITS_PER_RAW_SAMPLE:
        {
            name = "bits_per_raw_sample";
            break;
        }
        case MediaInfoType::NB_FRAMES:
        {
            name = "nb_frames";
            break;
        }
        default:
        {
            break;
        }
    }
    
    return name;
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
        InputStream* is = it->get();

        par = is->st->codecpar;
        dec_ctx = is->dec_ctx->get();
        
        addStreamInfo(stinfo, MediaInfoType::INDEX, is->st->index);
        addStreamInfo(stinfo, MediaInfoType::ID, is->st->id);

        if (cd = avcodec_descriptor_get(par->codec_id))
        {
            addStreamInfo(stinfo, MediaInfoType::CODEC_NAME, cd->name);
            addStreamInfo(stinfo, MediaInfoType::CODEC_LONG_NAME, 
                cd->long_name ? cd->long_name : UNKNOWN);
        }
        else 
        {
            addStreamInfo(stinfo, MediaInfoType::CODEC_NAME, UNKNOWN);
            addStreamInfo(stinfo, MediaInfoType::CODEC_LONG_NAME, UNKNOWN);
        }

#if 1
        cstr = avcodec_profile_name(par->codec_id, par->profile);
        addStreamInfo(stinfo, MediaInfoType::PROFILE, cstr ? cstr : UNKNOWN);
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
        addStreamInfo(stinfo, MediaInfoType::CODEC_TYPE, cstr ? cstr : UNKNOWN);

        switch (par->codec_type)
        {
            case AVMEDIA_TYPE_VIDEO:
            {
                addStreamInfo(stinfo, MediaInfoType::WIDTH, par->width);
                addStreamInfo(stinfo, MediaInfoType::HEIGHT, par->height);
                addStreamInfo(stinfo, MediaInfoType::CODED_WIDTH, dec_ctx->coded_width);
                addStreamInfo(stinfo, MediaInfoType::CODED_HEIGHT, dec_ctx->coded_height);
                addStreamInfo(stinfo, MediaInfoType::HAS_B_FRAME, par->video_delay);

                cstr = av_get_pix_fmt_name((AVPixelFormat)par->format);
                addStreamInfo(stinfo, MediaInfoType::PIX_FMT, cstr ? cstr : UNKNOWN);
                addStreamInfo(stinfo, MediaInfoType::LEVEL, par->level);

                cstr = av_color_range_name(par->color_range);
                addStreamInfo(stinfo, MediaInfoType::COLOR_RANGE, cstr ? cstr : UNKNOWN);

                cstr = av_color_space_name(par->color_space);
                addStreamInfo(stinfo, MediaInfoType::COLOR_SPACE, cstr ? cstr : UNKNOWN);

                cstr = av_color_transfer_name(par->color_trc);
                addStreamInfo(stinfo, MediaInfoType::COLOR_TRANSFER, cstr ? cstr : UNKNOWN);

                cstr = av_color_primaries_name(par->color_primaries);
                addStreamInfo(stinfo, MediaInfoType::COLOR_PRIMARIES, cstr ? cstr : UNKNOWN);

                cstr = av_chroma_location_name(par->chroma_location);
                addStreamInfo(stinfo, MediaInfoType::CHROMA_LOCATION, cstr ? cstr : UNKNOWN);
                break;
            }
            case AVMEDIA_TYPE_AUDIO:
            {
                cstr = av_get_sample_fmt_name((AVSampleFormat)par->format);
                addStreamInfo(stinfo, MediaInfoType::SAMPLE_FMT, cstr ? cstr : UNKNOWN);

                addStreamInfo(stinfo, MediaInfoType::SAMPLE_RATE, par->sample_rate);
                // addStreamInfo(stinfo, "channels", par->ch_layout.nb_channels);
                addStreamInfo(stinfo, MediaInfoType::BITS_PER_SAMPLE, av_get_bits_per_sample(par->codec_id));

                break;
            }
        }

        addStreamInfo(stinfo, MediaInfoType::R_FRAME_RATE, is->st->r_frame_rate);
        addStreamInfo(stinfo, MediaInfoType::AVG_FRAME_RATE, is->st->avg_frame_rate);
        addStreamInfo(stinfo, MediaInfoType::TIME_BASE, is->st->time_base);
        addStreamInfo(stinfo, MediaInfoType::START_PTS, is->st->start_time);
        addStreamInfo(stinfo, MediaInfoType::DURATION_TS, is->st->duration);

        addStreamInfo(stinfo, MediaInfoType::BIT_RATE, 
            par->bit_rate > 0 ? par->bit_rate : NUM_UNKNOWN);
        addStreamInfo(stinfo, MediaInfoType::BITS_PER_RAW_SAMPLE, 
            dec_ctx->bits_per_raw_sample > 0 ?
            dec_ctx->bits_per_raw_sample : NUM_UNKNOWN);
        addStreamInfo(stinfo, MediaInfoType::NB_FRAMES, is->st->nb_frames);

        info.push_back(std::make_pair(index, stinfo));

        ++index;
    }

    return ;
}

void FFProbeDemo::addStreamInfo(StreamInfo& ref, 
    MediaInfoType type, const char *value)
{
    ref.push_back(std::make_pair(
        type, std::string(value)));
}

void FFProbeDemo::addStreamInfo(StreamInfo& ref, 
    MediaInfoType type, int value)
{
    char cs_value[STR_BUF_SIZE];
    snprintf (cs_value, sizeof(cs_value), "%d", value);
    ref.push_back(std::make_pair(
        type, std::string(cs_value)));
}

void FFProbeDemo::addStreamInfo(StreamInfo& ref, 
    MediaInfoType type, int64_t value)
{
    char cs_value[STR_BUF_SIZE];
    snprintf (cs_value, sizeof(cs_value), "%ld", value);
    ref.push_back(std::make_pair(
        type, std::string(cs_value)));
}

void FFProbeDemo::addStreamInfo(StreamInfo &ref, 
    MediaInfoType type, const AVRational &r)
{
    char cs_value[STR_BUF_SIZE];
    snprintf (cs_value, sizeof(cs_value), "%d/%d", r.num, r.den);
    ref.push_back(std::make_pair(
            type, std::string(cs_value)));
}

NAMESPACE_FFMPEG_DEMO_END