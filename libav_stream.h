#ifndef LIBAV_STREAM_H
#define LIBAV_STREAM_H

#include <boost/function.hpp>

class LibavDemuxer;
struct AVStream;

typedef boost::function<void (uint8_t*,int)> ReadBufferCallback;

class LibavStream {
public:
    LibavStream(LibavDemuxer* demuxer, AVStream* stream);

    void ReadFrame(ReadBufferCallback& callback);
};

#endif // LIBAV_STREAM_H

