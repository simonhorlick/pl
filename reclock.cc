#include "reclock.h"
#include "frame.h"

extern "C" {
    #include <libavformat/avformat.h>
}

// Expects dest to be allocated with at least the size of max(a,b).
void lerp(AVFrame* dest, AVFrame* a, AVFrame* b, int num, int den) {
    // XXX: This will probably fail if the picture size changes in the
    // middle of the stream. That will never happen though, right?
    assert(a->width == b->width && a->height == b->height);
    if(den != 0) {
        int afactor = den-num;
        int bfactor = num;
        for(int y=0; y<dest->height; y++) {
            for(int x=0; x<dest->width; x++) {
                int p = (a->data[0][x+a->linesize[0]*y]*afactor) +
                        (b->data[0][x+b->linesize[0]*y]*bfactor);
                p /= den;
                dest->data[0][x+dest->linesize[0]*y] = (uint8_t)p;
            }
        }
        for(int y=0; y<dest->height/2; y++) {
            for(int x=0; x<dest->width/2; x++) {
                int p1 = (a->data[1][x+a->linesize[1]*y]*afactor) +
                         (b->data[1][x+b->linesize[1]*y]*bfactor);
                int p2 = (a->data[2][x+a->linesize[2]*y]*afactor) +
                         (b->data[2][x+b->linesize[2]*y]*bfactor);
                p1 /= den;
                p2 /= den;
                dest->data[1][x+dest->linesize[1]*y] = (uint8_t)p1;
                dest->data[2][x+dest->linesize[2]*y] = (uint8_t)p2;
            }
        }
    }
}

void Reclock::IncrementTimestamp() {
    // FIXME: This will incur rounding errors pretty quickly
    next += ptsTicksPerSecond_ / targetRate_;
}

void Reclock::Update() {
    // Frames are assumed to be Add()ed in presentation order.
    // Target frames will be output in display order
    static const int64_t invalid_timestamp = -1;
    if(next == invalid_timestamp) {
        next = source.front()->pts;
    }

    // Possible cases:
    //  - Display timestamp is the same as one in our list
    //    => Display this frame and remove all frames in source with lower timestamps
    //  - Display timestamp lies between two source frames
    //    => Return an interpolated frame
    //  - Display timestamp is greater than any in our list
    //    => Wait for more source frames.
    //  - Display timestamp is before any in our list
    //    => Display the first source frame.

    // Front of source contains the _oldest_ frame.

    if(next < source.front()->pts) {
        // We should never have a target timestamp before the oldest frame,
        // in this case the target timestamp should be set to the timestamp of
        // the oldest frame above. Of course, this could happen if bad
        // timestamps are supplied.
        assert(false);
    }

    // Find if this timestamp lies either on a source frame or between two
    // source frames.
    for(std::vector<AVFrameSharedPtr>::iterator it = source.begin(), prev = source.begin();
            it != source.end(); ++it) {
        if(next == (*it)->pts) {
            // erase any frames before this pts (not this one though, we may
            // need to reuse it, on the next frame for example.
            // TODO: There's probably a function for this already.
            cb_(*it);
            IncrementTimestamp();
        } else if(next < (*it)->pts) {
            // Interpolate between this and previous
            assert(it != prev);

            AVFrame* interpolated = new AVFrame;
            avpicture_alloc((AVPicture*)interpolated, (PixelFormat)(*it)->format, (*it)->width, (*it)->height);
            lerp(interpolated, prev->get(), it->get(), sourceRate_, targetRate_);

            // TODO: I don't know why I only just noticed this, but why are we
            // even bothering to do this in software? This is exactly what the
            // hardware is there for..

            cb_(AVFrameSharedPtr(interpolated, AVFrameDeleter()));
            IncrementTimestamp();
        }
    }

    // Otherwise the target timestamp is greater than that of all frames we
    // have seen so far. In this case we should wait for more frames.
}

