#include "libav_video_decoder.h"
#include "elementary_stream.h"

extern "C"
{
    #include <libavcodec/avcodec.h>
}

bool LibavVideoDecoder::Initialise(LibavDemuxer* demuxer) {
    avcodec_register_all();

    // TODO: Make this configurable
    codec_ = avcodec_find_decoder(CODEC_ID_H264);
    if(!codec_) {
        std::cerr << "libav error: avcodec_find_decoder\n";
        return false;
    }
    
    context_ = avcodec_alloc_context3(codec_);
    if(!context_) {
        std::cerr << "libav error: could not allocate libav context\n";
        return false;
    }

    if(avcodec_open2(context_, codec_, NULL) < 0) {
        std::cerr << "libav error: avcodec_open\n";
        return false;
    }

    demuxer_ = demuxer;

    return true;
}

bool LibavVideoDecoder::ReadFrame(ReadPictureCallback& callback) {
    // FIXME: This should run on a seperate thread.
    AVPacket pkt;
    if(!demuxer_->ReadFrame(pkt)) {
        return false;
    }

    while(pkt.size) {
        AVFrame* picture = avcodec_alloc_frame();
        int got_picture;
        int len = avcodec_decode_video2(context_, picture, &got_picture, &pkt);
        if(len < 0) {
            std::cerr << "Error while decoding frame\n";
            return false;
        }

        if(got_picture) {
            callback(picture);
        }

        pkt.size -= len;
        pkt.data += len;
        //av_free_packet(&pkt);
    }
    return true;
}


