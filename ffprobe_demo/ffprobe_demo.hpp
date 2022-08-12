#pragma once

#include <string>

#include "ffwrapper.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

class FFProbeDemo
{
public:
    FFProbeDemo (std::string const& filename);
    int init();
    int showStreams();
private:
    int openFile();
private:
    std::string filename__;
    std::shared_ptr<InputFile> input_file__;
};


NAMESPACE_FFMPEG_DEMO_END