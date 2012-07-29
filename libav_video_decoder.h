#ifndef LIBAV_VIDEO_DECODER_H
#define LIBAV_VIDEO_DECODER_H

#include <boost/function.hpp>

class ElementaryStream;
struct AVFrame;
struct AVCodecContext;

typedef boost::function<void(AVFrame*)> ReadPictureCallback;

class LibavVideoDecoder {
public:
    LibavVideoDecoder() : es_(0), codec_(0), codec_context_(0) {}

    bool Initialise(ElementaryStream* es);

    void ReadFrame(ReadPictureCallback& callback);

private:
    ElementaryStream* es_;
    AVCodecContext* context_;
    AVCodec* codec_;
};

#endif // LIBAV_VIDEO_DECODER_H

