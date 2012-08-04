#ifndef LIBAV_VIDEO_DECODER_H
#define LIBAV_VIDEO_DECODER_H

#include <boost/function.hpp>
#include "libav_demuxer.h"

class ElementaryStream;
struct AVFrame;
struct AVCodec;
struct AVCodecContext;

typedef boost::function<void(AVFrame*)> ReadPictureCallback;

class LibavVideoDecoder {
public:
    LibavVideoDecoder() : demuxer_(0), context_(0), codec_(0) {}

    bool Initialise(LibavDemuxer* demuxer);

    bool ReadFrame(ReadPictureCallback& callback);

private:
    LibavDemuxer* demuxer_;
    AVCodecContext* context_;
    AVCodec* codec_;
};

#endif // LIBAV_VIDEO_DECODER_H

