#include "ffplay_demo_helper.hpp"

#include "ffwrapper.hpp"
#include "ffplay_demo.hpp"  // !!! re-include

#include "log.hpp"

USING_NAMESPACE_FFMPEG_DEMO

FFPlayDemoState::FFPlayDemoState(FFPlayDemo *player)
{
    player_ = player;
}

FFPlayDemoPauseState::FFPlayDemoPauseState(FFPlayDemo *player)
    : FFPlayDemoState(player)
{
}

int FFPlayDemoPauseState::exec()
{
    xinfo("paused\n");
    return 0;
}

FFPlayDemoPlayingState::FFPlayDemoPlayingState(FFPlayDemo *player)
    : FFPlayDemoState(player)
{
}

int FFPlayDemoPlayingState::exec()
{
    int ret = player_->readFile();
    if (ret > 0)
    {
        xinfo("read...\n");
    }
    else if (0 == ret)
    {
        xinfo("read end, change to pause state\n");
        player_->toState(FFPlayDemo::State::PAUSE);
    }
    else
    {
        xinfo("read failed, change to stopped state\n");
        player_->toState(FFPlayDemo::State::STOP);
    }

    return ret;
}

FFPlayDemoStoppedState::FFPlayDemoStoppedState(FFPlayDemo *player)
    : FFPlayDemoState(player)
{
}

int FFPlayDemoStoppedState::exec()
{
    xinfo("stoped\n");
    return 0;
}