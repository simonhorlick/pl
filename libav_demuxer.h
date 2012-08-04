#ifndef __LIBAV_DEMUXER_H__
#define __LIBAV_DEMUXER_H__

#include "elementary_stream.h"

extern "C"
{
    #include <libavformat/avformat.h>
}

class LibavDemuxer {
public:
    LibavDemuxer();
    ~LibavDemuxer();
    bool Initialise(ElementaryStream* stream);

    void ReadFrame(AVPacket& packet);

private:
    ElementaryStream* stream_;
    AVFormatContext* format_context_;
    AVIOContext* io_context_;
    unsigned char* io_buffer_;
};

#endif // __LIBAV_DEMUXER_H__

