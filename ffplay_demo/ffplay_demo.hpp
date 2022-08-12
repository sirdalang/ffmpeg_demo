#pragma once

#include <string>

#include "ffwrapper.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

class FFPlayDemo
{
public:
    enum CtlType
    {
        PLAY,
        PAUSE,
        QUIT,
    };

    FFPlayDemo(const std::string &filename);
    int init();
    int exec();

    int control(CtlType type);
private:
    int openFile();
private:
    std::string filename__;
    std::shared_ptr<InputFile> input_file__;

    bool pause__{false};
    bool quit__{false};
};

NAMESPACE_FFMPEG_DEMO_END