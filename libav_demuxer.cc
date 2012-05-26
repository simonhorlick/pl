#include "libav_demuxer.h"
#include "libav_stream.h"

#include <iostream>

extern "C"
{
    #include <libavformat/avformat.h>
}

LibavDemuxer::LibavDemuxer() {
    av_register_all();
    format_context_ = avformat_alloc_context();
    source_ = 0;
    io_context_ = 0;
}

LibavDemuxer::~LibavDemuxer() {
    av_free(io_context_);
    avformat_free_context(format_context_);
}

namespace
{
    int ReadPacket(void* opaque, uint8_t* buffer, int size)
    {
        LibavDemuxer* demuxer = reinterpret_cast<LibavDemuxer*>(opaque);
        return demuxer->Read(buffer, size);
    }
}

bool LibavDemuxer::Initialise(DataSource* source) {
    source_ = source;

    // Create custom libav function that reads from source_.
    std::clog << "demuxer: registering custom IO handler for libav\n";
    const int buffersize = 4096;
    io_buffer_ = (unsigned char *)av_malloc(buffersize +
            FF_INPUT_BUFFER_PADDING_SIZE);
    io_context_ = avio_alloc_context(io_buffer_, buffersize, 0, this,
            &::ReadPacket, 0, 0);

    if(!io_context_) {
        std::cerr << "demuxer: avio_alloc_context failed\n";
        return false;
    }
    format_context_->pb = io_context_;

    std::clog << "demuxer: opening input source\n";
    int result = avformat_open_input(&format_context_, "", NULL, NULL);
    if(result < 0)
    {
        char errbuf[128];
        const char *errbuf_ptr = errbuf;

        if(av_strerror(result, errbuf, sizeof(errbuf)) < 0)
            errbuf_ptr = strerror(AVUNERROR(result));
        
        std::cerr << "ffmpeg: avformat_open_input failed ("
            << errbuf_ptr << ")\n";
        return false;
    }

    streams_ = FindStreams();
    
    return true;
}

std::vector<LibavStream*> LibavDemuxer::FindStreams()
{
    std::vector<LibavStream*> streams;

    //std::clog << "demuxer: find stream info\n";
    if(avformat_find_stream_info(format_context_, 0) < 0)
    {
        std::cerr << "ffmpeg error: av_find_stream_info\n";
        return streams;
    }

    streams.resize(format_context_->nb_streams);

    for(unsigned int i = 0; i < format_context_->nb_streams; i++)
    {
        //std::clog << "demuxer: stream " << i << "\n";
        AVCodecContext* codec_context = format_context_->streams[i]->codec;

        if(codec_context->codec_type == AVMEDIA_TYPE_VIDEO ||
            codec_context->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            streams[i] = new LibavStream(this, format_context_->streams[i]);
        }
    }
    return streams;
}

int LibavDemuxer::Read(uint8_t* buffer, int size) {
    return source_->Read(buffer, size);
}

void LibavDemuxer::PostDemuxTask() {

}

void LibavDemuxer::Demux() {

}

