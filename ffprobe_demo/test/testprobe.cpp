#include <gtest/gtest.h>

#include <memory>

#include "ffprobe_demo.hpp"

namespace 
{

const std::string video_file_name = "/home/test/video/demo.MOV";

TEST(TestProbe, BasicTest)
{
    ffdemo::FFProbeDemo probedemo(video_file_name);

    EXPECT_EQ(0, probedemo.init());
    EXPECT_EQ(0, probedemo.showStreams());

    std::shared_ptr<ffdemo::MediaInfo> info;
    probedemo.getInfo(info);

    EXPECT_NE(nullptr, info);

    for (auto it = info->cbegin(); it != info->cend(); ++it)
    {
        printf("stream:%d\n", it->first);
        for (auto it_str = it->second.cbegin(); 
            it_str != it->second.cend(); ++it_str)
        {
            printf(" %s:%s\n", 
                ffdemo::FFProbeDemo::nameOf(it_str->first),
                it_str->second.c_str());
        }
    }
}

}