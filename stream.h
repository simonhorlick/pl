#ifndef STREAM_H
#define STREAM_H

class LibavDemuxer;
struct AVStream;

class Stream {
public:
    Stream(LibavDemuxer* demuxer, AVStream* stream);

};

#endif // STREAM_H

