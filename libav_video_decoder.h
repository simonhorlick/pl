#ifndef LIBAV_VIDEO_DECODER_H
#define LIBAV_VIDEO_DECODER_H

class ElementaryStream;

class LibavVideoDecoder {
public:
    LibavVideoDecoder() : es_(0) {}

    bool Initialise(ElementaryStream* es);

    void GetFrame();

private:
    ElementaryStream* es_;
};

#endif // LIBAV_VIDEO_DECODER_H

