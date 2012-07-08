#include "libav_stream.h"
#include "libav_demuxer.h"

extern "C" {
    #include <libavformat/avformat.h>
}

LibavStream::LibavStream(LibavDemuxer* demuxer, AVStream* stream) {
    stream_ = stream;
    demuxer_ = demuxer;
}

AVCodecContext* LibavStream::GetAVContext() {
    return stream_->codec;
}

