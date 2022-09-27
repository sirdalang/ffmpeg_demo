#pragma once

#include <string>
#include <map>

#include "ffwrapper.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

enum class MediaInfoType
{
    INDEX,
    ID,
    CODEC_NAME,
    CODEC_LONG_NAME,
    PROFILE,
    CODEC_TYPE,
    WIDTH,
    HEIGHT,
    CODED_WIDTH,
    CODED_HEIGHT,
    HAS_B_FRAME,
    PIX_FMT,
    LEVEL,
    COLOR_RANGE,
    COLOR_SPACE,
    COLOR_TRANSFER,
    COLOR_PRIMARIES,
    CHROMA_LOCATION,
    SAMPLE_FMT,
    SAMPLE_RATE,
    BITS_PER_SAMPLE,
    R_FRAME_RATE,
    AVG_FRAME_RATE,
    TIME_BASE,
    START_PTS,
    DURATION_TS,
    BIT_RATE,
    BITS_PER_RAW_SAMPLE,
    NB_FRAMES,
};

typedef int StreamIndex;
typedef std::list<std::pair<MediaInfoType,std::string>> StreamInfo;
typedef std::list<std::pair<StreamIndex,StreamInfo>> MediaInfo;

class FFProbeDemo
{
public:
    FFProbeDemo (std::string const& filename);
    int init();
    int showStreams();
    void getInfo(std::shared_ptr<MediaInfo> &info);
    static const char *nameOf(MediaInfoType type);
private:
    int openFile();
    void getStreamInfo(MediaInfo &info);

    void addStreamInfo(StreamInfo& ref, MediaInfoType type, const char *value);
    void addStreamInfo(StreamInfo& ref, MediaInfoType type, int value);
    void addStreamInfo(StreamInfo& ref, MediaInfoType type, int64_t value);
    void addStreamInfo(StreamInfo& ref, MediaInfoType type, const AVRational &r);
private:
    std::string filename__;
    std::shared_ptr<InputFile> input_file__;
    std::shared_ptr<MediaInfo> info__;
};


NAMESPACE_FFMPEG_DEMO_END