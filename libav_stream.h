#ifndef LIBAV_STREAM_H
#define LIBAV_STREAM_H

#include <boost/function.hpp>

class LibavDemuxer;
struct AVStream;
struct AVCodecContext;

class LibavStream {
public:
    LibavStream(LibavDemuxer* demuxer, AVStream* stream);

    AVCodecContext* GetAVContext();

    AVPacket* ReadFrame();
private:
    AVStream* stream_;
    LibavDemuxer* demuxer_;
};

#endif // LIBAV_STREAM_H

