#pragma once

#include <string>

#include "ffwrapper.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

class FFProbeDemo
{
public:
    typedef std::pair<std::string,std::string> PairText;
    typedef std::pair<std::string, std::list<PairText>> TripleText;
    typedef std::list<TripleText> MediaInfo;

    FFProbeDemo (std::string const& filename);
    int init();
    int showStreams();
    void getInfo(std::shared_ptr<MediaInfo> &info);
private:
    int openFile();
    void getStreamInfo(MediaInfo &info);
private:
    std::string filename__;
    std::shared_ptr<InputFile> input_file__;
    std::shared_ptr<MediaInfo> info__;
};


NAMESPACE_FFMPEG_DEMO_END