#include <gtest/gtest.h>
#include <list>
#include "reclock.h"
#include "frame.h"

extern "C" {
    #include <libavcodec/avcodec.h>
}

class ReclockTest : public testing::Test {
    void PushSinglePixelFrame(uint8_t colour[3]) {
        AVFrame* frame = new AVFrame;
        avpicture_alloc((AVPicture*)frame, PIX_FMT_YUV420P, 1, 1);
        for(int i=0; i<3; i++)
            frame->data[i][0] = colour[i];
        frames.push_back(AVFrameSharedPtr(frame, AVFrameDeleter()));
    }

protected:
    std::list<AVFrameSharedPtr> frames;
};

// Ensure frames are untouched
TEST_F(ReclockTest, OneToOne) {
    FAIL() << "Not implemented";
}

// If there are twice as many source frames as there are target frames, then
// ensure no frames are blended, as there is one source frame at the time of
// each target frame.
TEST_F(ReclockTest, DoubleNumberOfSourceFrames) {
    FAIL() << "Not implemented";
}

// Pick the closest source frames to the target frame time and interpolate
// between them.
TEST_F(ReclockTest, ManySourceFrames) {
    FAIL() << "Not implemented";
}

// If there are half as many source frames as target frames, then target frames
// lying in between two source frames should be interpolated evenly between the
// two source frames.
TEST_F(ReclockTest, HalfNumberOfSourceFrames) {
    FAIL() << "Not implemented";
}

