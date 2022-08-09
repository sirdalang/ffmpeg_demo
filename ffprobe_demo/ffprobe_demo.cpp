#include "ffprobe_demo.hpp"

#ifdef __cplusplus
extern "C" 
{
#include <libavformat/avformat.h>
#include <libavformat/version.h>
}
#endif 

FFProbeDemo::FFProbeDemo (std::string const& filename)
{
    filename__ = filename;
}

int FFProbeDemo::init()
{
}

int FFProbeDemo::open_file()
{
    AVFormatContext *fmt_ctx = nullptr;

    fmt_ctx = avformat_alloc_context();
    if (!fmt_ctx)
    {
        
    }
}