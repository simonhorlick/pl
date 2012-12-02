#ifndef FRAME_H
#define FRAME_H

#include <cassert>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

extern "C" {
    #include <libavcodec/avcodec.h>
}

// A custom deallocator so we can use AVPictures in standard library
// collections.
struct AVFrameDeleter {
    void operator()(AVFrame* p) {
        assert(p);
        avpicture_free((AVPicture*)p);
        delete p;
    }
};

typedef boost::shared_ptr<AVFrame> AVFrameSharedPtr;
typedef boost::function<void(AVFrameSharedPtr)> FrameCallback;

#endif // FRAME_H

