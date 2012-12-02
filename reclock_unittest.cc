#include <gtest/gtest.h>
#include <list>
#include "reclock.h"

extern "C" {
    #include <libavcodec/avcodec.h>
}

class ReclockTest : public testing::Test {

protected:
    std::list<AVFrame*> frames;
};

TEST_F(ReclockTest, Test1) {
    // calculate timestamps for 1fps source
    int time = 50;
    int frame = 0;
    for(std::list<AVFrame*>::iterator it = frames.begin();
            it != frames.end(); ++it, ++frame) {
        (*it)->pts = (1000*frame)/time;
    }

    // Assume 1fps and reclock to 60fps
    reclock(frames, time, 60);
}

