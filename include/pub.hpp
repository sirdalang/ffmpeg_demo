#pragma once

#include <assert.h>

#define NAMESPACE_FFMPEG_DEMO_BEGIN \
    namespace ffdemo {

#define NAMESPACE_FFMPEG_DEMO_END \
    }

#define USING_NAMESPACE_FFMPEG_DEMO \
    using namespace ffdemo;

#define FF_DEMO_ASSERT(expr) \
    assert(expr)

