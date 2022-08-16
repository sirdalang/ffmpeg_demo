# ffmpeg_demo
Some ffmpeg demos. May be useful!

## build ffmpeg

ubuntu/x64
../ffmpeg/configure --prefix=$(pwd)/output  --disable-x86asm

sudo apt-get install ffmpeg libavformat-dev libavcodec-dev libavutil-dev libswscale-dev libswresample-dev -y

## valgrind

使用 valgrind 检查内存泄漏的时候，存在一些误报，举例如下：  

```C
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

int fun()
{
        AVFormatContext *fmt_ctx = NULL;

        avformat_open_input(&fmt_ctx, NULL, NULL, NULL);
        avformat_close_input(&fmt_ctx);

        return 0;
}

int main()
{
        return 0;
}
```

以上程序只是简单的链接了 avformat 库，在使用 valgrind 检查泄漏的时候，会出现如下信息：  

```bash
==56783== 184 bytes in 1 blocks are possibly lost in loss record 226 of 241
==56783==    at 0x483DFAF: realloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==56783==    by 0x78F2F3F: g_realloc (in /usr/lib/x86_64-linux-gnu/libglib-2.0.so.0.6400.6)
==56783==    by 0x786F063: ??? (in /usr/lib/x86_64-linux-gnu/libgobject-2.0.so.0.6400.6)
==56783==    by 0x7873324: g_type_register_static (in /usr/lib/x86_64-linux-gnu/libgobject-2.0.so.0.6400.6)
==56783==    by 0x785AD12: g_param_type_register_static (in /usr/lib/x86_64-linux-gnu/libgobject-2.0.so.0.6400.6)
==56783==    by 0x785D7EA: ??? (in /usr/lib/x86_64-linux-gnu/libgobject-2.0.so.0.6400.6)
==56783==    by 0x78480CB: ??? (in /usr/lib/x86_64-linux-gnu/libgobject-2.0.so.0.6400.6)
==56783==    by 0x4011B99: call_init.part.0 (dl-init.c:72)
==56783==    by 0x4011CA0: call_init (dl-init.c:30)
==56783==    by 0x4011CA0: _dl_init (dl-init.c:119)
==56783==    by 0x4001139: ??? (in /usr/lib/x86_64-linux-gnu/ld-2.31.so)
```

所有的项都是由 _dl_init 引起的，这应该是 libavforamt 库内部的一些链接执行函数的内存使用，可以忽略。  