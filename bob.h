#ifndef BOB_H
#define BOB_H

#include <list>
struct AVFrame;

// Take an interlaced frame f and add two frames to queue corresponding to the
// bobbed top and bottom fields of f (this will, of course, double the frame
// rate).
void bob(AVFrame* f, std::list<AVFrame*>& frames);

#endif // BOB_H

