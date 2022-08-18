#pragma once

#include <string>
#include <map>

#include "ffwrapper.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

class FFProbeDemo
{
public:
    typedef std::list<std::pair<std::string,std::string>> StreamInfo;
    typedef std::list<std::pair<std::string,StreamInfo>> MediaInfo;

    FFProbeDemo (std::string const& filename);
    int init();
    int showStreams();
    void getInfo(std::shared_ptr<MediaInfo> &info);
private:
    int openFile();
    void getStreamInfo(MediaInfo &info);

    void addStreamInfo(StreamInfo& ref, const char *key, const char *value);
    void addStreamInfo(StreamInfo& ref, const char *key, int value);
private:
    std::string filename__;
    std::shared_ptr<InputFile> input_file__;
    std::shared_ptr<MediaInfo> info__;
};


NAMESPACE_FFMPEG_DEMO_END