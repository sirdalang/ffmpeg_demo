#pragma once

#include <string>

#include "ffwrapper.hpp"
#include "ffplay_demo_helper.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

class FFPlayDemo
{
public:
    enum class CtlType
    {
        PLAY,
        PAUSE,
        STOP,
    };
    enum class State
    {
        PLAY,
        PAUSE,
        STOP,
    };

    typedef int(*cb_push_packet)(const std::shared_ptr<WrapAVPacket> packet);
    typedef int(*cb_push_frame)(const std::shared_ptr<WrapAVFrame> packet);

    class CallBacks
    {
    public:
        cb_push_packet cb_push_packet_{nullptr};
        cb_push_frame cb_push_frame_{nullptr};
    };

    class PlayContext;
    
public:
    FFPlayDemo(const std::string &filename, const CallBacks &cb);
    int init();
    int exec();

    int control(CtlType type);
private:
    int openFile();
    int readFile();
    int toState(State state);
    int pushPacket(const std::shared_ptr<WrapAVPacket> packet);
    int decodeFrame(const std::shared_ptr<WrapAVPacket> packet);
private:
    std::string filename_;
    std::shared_ptr<CallBacks> callbacks_;
    std::shared_ptr<InputFile> input_file_;

    std::shared_ptr<FFPlayDemoState> state_pause_;
    std::shared_ptr<FFPlayDemoState> state_playing_;
    std::shared_ptr<FFPlayDemoState> state_stopped_;
    std::shared_ptr<FFPlayDemoState> state_;

    std::shared_ptr<PlayContext> play_ctx_;

    /* friend class */
    friend class FFPlayDemoPauseState;
    friend class FFPlayDemoPlayingState;
    friend class FFPlayDemoStoppedState;
};

NAMESPACE_FFMPEG_DEMO_END