#include "ffplay_demo.hpp"

#include "log.hpp"

NAMESPACE_FFMPEG_DEMO_BEGIN

FFPlayDemo::FFPlayDemo(const std::string &filename)
{
    filename__ = filename;
}

int FFPlayDemo::init()
{
    const char *filename = filename__.c_str();
    
    if (openFile() < 0)
    {
        xerror("open file %s failed\n", filename);
        return -1;
    }

    return 0;
}

int FFPlayDemo::openFile()
{
    int ret = ffwrapper_open_file(filename__, input_file__);

    return ret;
}

int FFPlayDemo::control(CtlType type)
{
    switch (type)
    {
        case PLAY:
        {
            pause__ = false;
            break;
        }
        case PAUSE:
        {
            pause__ = true;
            break;
        }
        case QUIT:
        {
            quit__ = true;
            break;
        }
    }
}

NAMESPACE_FFMPEG_DEMO_END