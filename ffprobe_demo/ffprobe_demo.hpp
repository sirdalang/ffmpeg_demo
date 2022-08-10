#pragma once

#include <string>

#include "ffprobe_demo_def.hpp"

class FFProbeDemo
{
public:
    FFProbeDemo (std::string const& filename);
    int init();
private:
    int open_file();
private:
    std::string filename__;
    std::shared_ptr<InputFile> input_file__;
};