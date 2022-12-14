#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#if 0
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

int packet_callback(std::shared_ptr<WrapAVPacket> av_packet)
{
    AVPacket *packet = av_packet->get();
    printf ("packet: size=%d, stream=%d\n",
        packet->size,
        packet->stream_index);
    return 0;
}

int frame_callback(const std::shared_ptr<WrapAVFrame> av_frame)
{
    AVFrame *frame = av_frame->get();
    printf ("frame: pkt_size=%d, w,h=%d,%d\n",
        frame->pkt_size,
        frame->width, frame->height);
    return 0;
}

int play_test(const char *filename)
{
    std::string str_filename(filename);
    FFPlayDemo::CallBacks cbs;
    cbs.cb_push_packet_ = packet_callback;
    cbs.cb_push_frame_ = frame_callback;

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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int sdli_test()
{
    SDLI sdli;

    sdli.init();

    sdli.exec();

    return 0;
}
#endif 

int main(int argc, char *argv[])
{
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    return 0;
}

