#include <iostream>
#include "libav_demuxer.h"
#include "elementary_stream.h"

namespace {
    int ReadPacket(void* opaque, uint8_t* buffer, int size) {
        ElementaryStream* stream = reinterpret_cast<ElementaryStream*>(opaque);
        return stream->Read(buffer, size);
    }
}

extern AVInputFormat ff_h264_demuxer;

LibavDemuxer::LibavDemuxer()
    : stream_(0), io_context_(0) {
    av_register_input_format(&ff_h264_demuxer);
    format_context_ = avformat_alloc_context();
}

LibavDemuxer::~LibavDemuxer() {
    av_free(io_context_);
    avformat_free_context(format_context_);
}

bool LibavDemuxer::Initialise(ElementaryStream* stream) {
    stream_ = stream;

    // Register a custom IO handler to use our input source.
    std::clog << "demuxer: registering custom IO handler for libav\n";
    const int buffersize = 4096;
    io_buffer_ = (unsigned char *)av_malloc(buffersize +
            FF_INPUT_BUFFER_PADDING_SIZE);
    io_context_ = avio_alloc_context(io_buffer_, buffersize, 0, stream_,
            &::ReadPacket, 0, 0);

    if(!io_context_) {
        std::cerr << "demuxer: avio_alloc_context failed\n";
        return false;
    }
    format_context_->pb = io_context_;

    std::clog << "demuxer: opening input source\n";
    int result = avformat_open_input(&format_context_, "", NULL, NULL);
    if(result < 0) {
        char errbuf[128];
        const char *errbuf_ptr = errbuf;

        if(av_strerror(result, errbuf, sizeof(errbuf)) < 0)
            errbuf_ptr = strerror(AVUNERROR(result));
        
        std::cerr << "ffmpeg: avformat_open_input failed ("
            << errbuf_ptr << ")\n";
        return false;
    }
    return true;
}

bool LibavDemuxer::ReadFrame(AVPacket& packet) {
    if(av_read_frame(format_context_, &packet) < 0) {
        std::clog << "av_read_frame failed\n";
        packet.data = 0;
        packet.size = 0;
        return false;
    }

    // Make sure libav doesn't free the packet before we've finished with
    // it.
    av_dup_packet(&packet);

    return true;
}

