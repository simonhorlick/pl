#include <gtest/gtest.h>
#include <list>
#include <boost/bind.hpp>
#include "reclock.h"
#include "frame.h"

extern "C" {
    #include <libavcodec/avcodec.h>
}

class ReclockTest : public testing::Test {
public:
    AVFrameSharedPtr MakeSinglePixelFrame(uint8_t colour[3]) {
        AVFrame* frame = new AVFrame;
        avpicture_alloc((AVPicture*)frame, PIX_FMT_YUV420P, 1, 1);
        for(int i=0; i<3; i++)
            frame->data[i][0] = colour[i];
        return AVFrameSharedPtr(frame, AVFrameDeleter());
    }

    void OnFrame(const AVFrameSharedPtr& frame) {
        std::clog << "Got frame" << std::endl;
        targetFrames.push_back(frame);
    }

protected:
    std::list<AVFrameSharedPtr> targetFrames;
};

// Ensure frames are untouched
TEST_F(ReclockTest, OneToOne) {
    uint8_t A[3] = {0xff, 0xff, 0xff};
    uint8_t B[3] = {0x0, 0x0, 0x0};

    FrameCallback cb(boost::bind(&ReclockTest::OnFrame, this, _1));

    Reclock r;
    r.Initialise(cb, 25, 25);

    std::list<AVFrameSharedPtr> sourceFrames;
    sourceFrames.push_back(MakeSinglePixelFrame(A));
    sourceFrames.push_back(MakeSinglePixelFrame(B));
    sourceFrames.push_back(MakeSinglePixelFrame(A));
    sourceFrames.push_back(MakeSinglePixelFrame(B));

    for(std::list<AVFrameSharedPtr>::iterator it = sourceFrames.begin();
            it != sourceFrames.end(); ++it) {
        r.Add(*it);
    }

    EXPECT_EQ(targetFrames, sourceFrames);
}

// If there are twice as many source frames as there are target frames, then
// ensure no frames are blended, as there is one source frame at the time of
// each target frame.
TEST_F(ReclockTest, DoubleNumberOfSourceFrames) {
    uint8_t A[3] = {0xff, 0xff, 0xff};
    uint8_t B[3] = {0x0, 0x0, 0x0};

    FrameCallback cb(boost::bind(&ReclockTest::OnFrame, this, _1));

    Reclock r;
    r.Initialise(cb, 50, 25);

    AVFrameSharedPtr frames[4];
    frames[0] = MakeSinglePixelFrame(A);
    frames[1] = MakeSinglePixelFrame(B);
    frames[2] = MakeSinglePixelFrame(A);
    frames[3] = MakeSinglePixelFrame(B);

    std::list<AVFrameSharedPtr> sourceFrames;
    sourceFrames.push_back(frames[0]);
    sourceFrames.push_back(frames[1]);
    sourceFrames.push_back(frames[2]);
    sourceFrames.push_back(frames[3]);

    std::list<AVFrameSharedPtr> expectedTargetFrames;
    expectedTargetFrames.push_back(frames[0]);
    expectedTargetFrames.push_back(frames[2]);

    for(std::list<AVFrameSharedPtr>::iterator it = sourceFrames.begin();
            it != sourceFrames.end(); ++it) {
        r.Add(*it);
    }

    EXPECT_EQ(expectedTargetFrames, targetFrames);
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

