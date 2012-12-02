#include "reclock.h"

extern "C" {
    #include <libavformat/avformat.h>
}

void lerp(AVFrame* dest, AVFrame* a, AVFrame* b, int num, int den) {
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

void reclock(std::list<AVFrame*>& source, std::list<AVFrame*>& target, int sourcebase, int targetbase) {
    int nn = 0;
    AVFrame* p = source[0];
    AVFrame* n = source[nn];
    // FIXME: Use libav functions for this, just need to allocate a frame
    AVFrame* interp = make_frame(p->width, p->height);
    int origframes = source.size();
    int newframes = (origframes*targetbase)/sourcebase;

    // FIXME: This is really shit. Want to just take two frames from source, output
    // a frame then possibly remove frames from source.
    for(int i=0; i<newframes; i++) {
        int dts = (i*1000)/targetbase;
        if(n->pts < dts) {
            n = source[++nn];
            p = source[nn-1];
        }
        assert(n->pts >= dts);
        assert(p->pts <= dts);
        int num = dts - p->pts;
        int den = n->pts - p->pts; //(timebase)
        lerp(interp, p, n, num, den);
        target.push_back(interp);
    }
}

