#include "stream.h"
#include "libav_demuxer.h"

extern "C" {
    #include <libavformat/avformat.h>
}

Stream::Stream(LibavDemuxer* demuxer, AVStream* stream) {

}

