#pragma once

#include <string>

#include "ffprobe_demo_def.hpp"

class FFProbeDemo
{
public:
    FFProbeDemo (std::string const& filename);
    int init();
    int showStreams();
    int getContext(std::shared_ptr<InputFile> &input_file);
private:
    int openFile();
private:
    std::string filename__;
    std::shared_ptr<InputFile> input_file__;
};