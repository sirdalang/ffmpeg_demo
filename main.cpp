#include <thread>
#include <chrono>

#include "ffprobe_demo.hpp"
#include "ffplay_demo.hpp"

USING_NAMESPACE_FFMPEG_DEMO

int probe_test(const char *char_filename)
{
    std::shared_ptr<FFProbeDemo::MediaInfo> mediainfo;

    std::string filename(char_filename);
    FFProbeDemo demo(filename);

    demo.init();
    demo.getInfo(mediainfo);

    for (auto it = mediainfo->cbegin(); 
        it != mediainfo->cend(); 
        ++it)
    {
        printf ("name:%s\n", it->first.c_str());
        for (auto it_sub = it->second.cbegin();
            it_sub != it->second.cend();
            ++it_sub)
        {
            printf ("  %s:%s\n", 
                it_sub->first.c_str(),
                it_sub->second.c_str());
        }
    }

    return 0;
}

int packet_callback(const AVPacket *packet)
{
    printf ("packet: size=%d\n",
        packet->size);
    return 0;
}

int play_test(const char *filename)
{
    std::string str_filename(filename);
    FFPlayDemo::CallBacks cbs;
    cbs.cb_push_packet_ = packet_callback;

    FFPlayDemo player(str_filename, cbs);

    if (player.init() < 0)
    {
        printf ("init failed\n");
        return -1;
    }

    player.control(FFPlayDemo::CtlType::PLAY);

    while (true)
    {
        player.exec();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf ("Usage: %s <file>\n", argv[0]);
        return 0;
    }

    // probe_test(argv[1]);
    play_test(argv[1]);

    return 0;
}

