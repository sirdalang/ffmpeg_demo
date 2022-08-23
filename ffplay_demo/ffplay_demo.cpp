#include "ffplay_demo.hpp"

#include "log.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

class FFPlayDemo::PlayContext
{
public:
    bool config_enable_decode{true};

    bool config_enable_decode_video{true};
    bool config_enable_decode_audio{true};
};

FFPlayDemo::FFPlayDemo(const std::string &filename, const CallBacks &cb)
{
    filename_ = filename;
    input_file_ = std::make_shared<InputFile>();
    callbacks_ = std::make_shared<CallBacks>(cb);

    play_ctx_ = std::make_shared<PlayContext>();

    state_pause_ = std::make_shared<FFPlayDemoPauseState>(this);
    state_playing_ = std::make_shared<FFPlayDemoPlayingState>(this);
    state_stopped_ = std::make_shared<FFPlayDemoStoppedState>(this);

    state_ = state_stopped_;
}

int FFPlayDemo::init()
{
    const char *filename = filename_.c_str();
    
    if (openFile() < 0)
    {
        xerror("open file %s failed\n", filename);
        return -1;
    }

    return 0;
}

int FFPlayDemo::exec()
{
    return state_->exec();
}

int FFPlayDemo::control(CtlType type)
{
    switch (type)
    {
        case CtlType::PLAY:
        {
            state_ = state_playing_;
            break;
        }
        case CtlType::PAUSE:
        {
            state_ = state_pause_;
            break;
        }
        case CtlType::STOP:
        {
            state_ = state_stopped_;
            break;
        }
        default:
        {
            xerror("not support\n");
            FF_DEMO_ASSERT(false);
            break;
        }
    }

    return 0;
}

int FFPlayDemo::openFile()
{
    int ret = ffwrapper_open_file(filename_, input_file_);

    if (ret < 0)
    {
        xerror("open file failed\n");
    }
    else
    {
        setChannels();
    }

    return ret;
}

/* 配置播放器对应的视频和音频解码通道（源可能有多个通道） */
int FFPlayDemo::setChannels()
{
    int ret = 0;

    for (auto it = input_file_->streams.begin(); it != input_file_->streams.end(); ++it)
    {
        if(!is_video_
            && AVMEDIA_TYPE_VIDEO == it->get()->dec_ctx->get()->codec_type)
        {
            xdebug("chosen video chn: %d\n", it->get()->st->index);
            is_video_ = *it;
        }
        else if (!is_audio_
            && AVMEDIA_TYPE_AUDIO == it->get()->dec_ctx->get()->codec_type)
        {
            xdebug("chosen audio chn: %d\n", it->get()->st->index);
            is_audio_ = *it;
        }
    }

    return 0;
}

/**
 * @brief 
 * 
 * @return int 0 eof -1 error
 */
int FFPlayDemo::readFile()
{
    xinfo("read file\n");

    int ret = 0;

    AVFormatContext *av_fmt_ctx = input_file_->fmt_ctx->get();
    // WrapAVPacket av_packet;
    std::shared_ptr<WrapAVPacket> av_packet = std::make_shared<WrapAVPacket>();

    ret = av_read_frame(av_fmt_ctx, av_packet->get());

    if (0 == ret)
    {
        pushPacket(av_packet);
        if (play_ctx_->config_enable_decode)
        {
            decodeFrame(av_packet);
        }
        return 1;
    }
    else if (AVERROR_EOF == ret)
    {
        return 0;
    }
    else
    {
        return -1;
    }

    return 0;
}

int FFPlayDemo::toState(State state)
{
    switch (state)
    {
        case State::PAUSE:
        {
            state_ = state_pause_;
            break;
        }
        case State::PLAY:
        {
            state_ = state_playing_;
            break;
        }
        case State::STOP:
        {
            state_ = state_stopped_;
            break;
        }
        default:
        {
            xerror("not support\n");
            FF_DEMO_ASSERT(false);
            break;
        }
    }

    return 0;
}

int FFPlayDemo::pushPacket(std::shared_ptr<WrapAVPacket> av_packet)
{
    int ret = 0;

    if (callbacks_->cb_push_packet_)
    {
        ret = callbacks_->cb_push_packet_(av_packet);
    }

    return  ret;
}

int FFPlayDemo::decodeFrame(std::shared_ptr<WrapAVPacket> av_packet)
{
    int ret = 0;

    std::shared_ptr<WrapAVFrame> vframe;
    vframe = std::make_shared<WrapAVFrame>();

    xdebug("packet: stream=%d, size=%d\n",
        av_packet->get()->stream_index,
        av_packet->get()->size);

    /* send packet */
    if (av_packet->get()->stream_index == is_video_->st->index)
    {
        ret = avcodec_send_packet(is_video_->dec_ctx->get(), av_packet->get());
        if (0 == ret)
        {
            xdebug("send packet suc\n");
        }
        else
        {
            xerror("send video packet failed, ret=%d\n", ret);
        }
    }
    else if (av_packet->get()->stream_index == is_audio_->st->index)
    {
        // not decode audio
    }

    /* recv frames */
    while (true)
    {
        ret = avcodec_receive_frame(is_video_->dec_ctx->get(), vframe->get());
        if (0 == ret)
        {
            xdebug("got a frame\n");
            if (callbacks_->cb_push_frame_)
            {
                callbacks_->cb_push_frame_(vframe);
            }
        }
        else
        {
            if (AVERROR(EAGAIN) == ret)
            {
                xdebug("no more frames of ch=%d\n", is_video_->st->index);
                break;
            }
            else
            {
                xerror("receive frame failed, ret=%d\n", ret);
                break;
            }
        }
    }

    return 0;
}

NAMESPACE_FFMPEG_DEMO_END