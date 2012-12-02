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
    // TODO: Implement
}

