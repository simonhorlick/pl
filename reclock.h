#ifndef RECLOCK_H
#define RECLOCK_H

#include <vector>
#include "frame.h"

// Take frames to be played at a source rate and output frames at
// target rate. The target frames that lie at a point in time between two
// source frames will be blended using a simple linear interpolation based on
// the time offsets from the source frame(s).

class Reclock { 
public:
    // The callback cb will be called upon completion of a target frame. The
    // rates sourceRate and targetRate are specified as frames-per-second, i.e.
    // 50 for 50fps and 25 for 25fps.
    bool Initialise(const FrameCallback& cb, int64_t ptsTicksPerSecond, int sourceRate, int targetRate) {
        cb_ = cb;
        sourceRate_ = sourceRate;
        targetRate_ = targetRate;
        ptsTicksPerSecond_ = ptsTicksPerSecond;
        next = -1;
        return true;
    }

    void Add(const AVFrameSharedPtr& frame) {
        source.push_back(frame);
        Update();
    }

private:
    // Determines whether we can return a completed frame yet and computes one.
    void Update();
    void IncrementTimestamp();

    int sourceRate_;
    int targetRate_;
    int64_t ptsTicksPerSecond_;

    int64_t next; // Timestamp of the next target frame

    std::vector<AVFrameSharedPtr> source;
    FrameCallback cb_;
};

#endif // RECLOCK_H

