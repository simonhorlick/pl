#ifndef RECLOCK_H
#define RECLOCK_H

#include <list>
struct AVFrame;

// Take frames to be played at sourcebase rate and output frames onto target at
// target rate. The target frames that lie at a point in time between two
// source frames will be blended using a simple linear interpolation based on
// the time offsets from the source frame(s).
void reclock(std::list<AVFrame*>& source, int sourcebase, int targetbase);

#endif // RECLOCK_H

