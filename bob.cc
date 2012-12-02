#include "bob.h"
#include <list>

extern "C" {
    #include <libavformat/avformat.h>
}

AVFrame* make_frame(int width, int height) {
    AVFrame* output = avcodec_alloc_frame();
    output->width = width;
    output->height = height;
    output->data[0] = (uint8_t*)av_malloc(width * height);
    output->data[1] = (uint8_t*)av_malloc((width/2) * (height/2));
    output->data[2] = (uint8_t*)av_malloc((width/2) * (height/2));
    output->linesize[0] = width;
    output->linesize[1] = width/2;
    output->linesize[2] = width/2;
    return output;
}

void bob(AVFrame* f, std::list<AVFrame*>& frames) {
    // tff bob
    for(int field=1; field>=0; field--) {
        AVFrame* output = make_frame(f->width, f->height);
        // take even rows of source and double them up on the target
        for(int y=1; y<f->height; y+=2) {
            for(int x=0; x<f->width; x++) {
                output->data[0][x+output->linesize[0]*(y-1)] = f->data[0][x+f->linesize[0]*(y-field)];
                output->data[0][x+output->linesize[0]*(y)] = f->data[0][x+f->linesize[0]*(y-field)];
            }
        }
        for(int y=1; y<f->height/2; y+=2) {
            for(int x=0; x<f->width/2; x++) {
                output->data[1][x+output->linesize[1]*(y-1)] = f->data[1][x+f->linesize[1]*(y-field)];
                output->data[1][x+output->linesize[1]*(y)] = f->data[1][x+f->linesize[1]*(y-field)];
                output->data[2][x+output->linesize[2]*(y-1)] = f->data[2][x+f->linesize[2]*(y-field)];
                output->data[2][x+output->linesize[2]*(y)] = f->data[2][x+f->linesize[2]*(y-field)];
            }
        }
        frames.push_back(output);
    }
}

