#include "ffprobe_demo.hpp"

USING_NAMESPACE_FFMPEG_DEMO;

int probe_test(const char *char_filename)
{
    std::string filename(char_filename);
    FFProbeDemo demo(filename);

    demo.init();
    demo.showStreams();

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf ("Usage: %s <file>\n", argv[0]);
        return 0;
    }

    probe_test(argv[1]);

    return 0;
}

