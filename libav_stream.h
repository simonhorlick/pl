#ifndef LIBAV_STREAM_H
#define LIBAV_STREAM_H

class LibavDemuxer;
struct AVStream;

class LibavStream {
public:
    LibavStream(LibavDemuxer* demuxer, AVStream* stream);

};

#endif // LIBAV_STREAM_H

