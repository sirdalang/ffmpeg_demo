#include "ffprobe_demo.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf ("Usage: %s <file>\n", argv[0]);
        return 0;
    }

    std::string filename(argv[1]);
    FFProbeDemo demo(filename);

    demo.init();

    return 0;
}