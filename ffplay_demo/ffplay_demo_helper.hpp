
#pragma once

#include "pub.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

class FFPlayDemo;

class FFPlayDemoState
{
public:
    FFPlayDemoState(FFPlayDemo *player);
    virtual ~FFPlayDemoState() = default;

    virtual int exec() = 0;
    // virtual int pause() = 0;
    // virtual int play() = 0;
    // virtual int stop() = 0;

    FFPlayDemo *player_;
};

class FFPlayDemoPauseState : public FFPlayDemoState
{
public:
    FFPlayDemoPauseState(FFPlayDemo *player);
    int exec() override;
    // int pause() override;
    // int play() override;
    // int stop() override;
};

class FFPlayDemoPlayingState : public FFPlayDemoState
{
public:
    FFPlayDemoPlayingState(FFPlayDemo *player);
    int exec() override;
    // int pause() override;
    // int play() override;
    // int stop() override;
};

class FFPlayDemoStoppedState : public FFPlayDemoState
{
public:
    FFPlayDemoStoppedState(FFPlayDemo *player);
    int exec() override;
    // int pause() override;
    // int play() override;
    // int stop() override;
};

NAMESPACE_FFMPEG_DEMO_END