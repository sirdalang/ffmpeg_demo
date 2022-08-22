#include "ffplay_demo.hpp"

#include "log.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

class FFPlayDemo::PlayContext
{
public:
    bool config_enable_decode{true};
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

    return ret;
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
    // if (packet->)
    return 0;
}

NAMESPACE_FFMPEG_DEMO_END