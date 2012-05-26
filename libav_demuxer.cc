#include "libav_demuxer.h"

extern "C"
{
    #include <libavformat/avformat.h>
}

LibavDemuxer::LibavDemuxer() {
    av_register_all();
}

LibavDemuxer::~LibavDemuxer() {

}

bool LibavDemuxer::Initialise(DataSource* source) {
    source_ = source;

    // Create custom libav function that reads from source_.


    return false;
}

