#include "bob.h"
#include "frame.h"
#include <list>

extern "C" {
    #include <libavformat/avformat.h>
}

// TODO: Fix the number of allocations
void bob(const AVFrameSharedPtr& input, std::list<AVFrameSharedPtr>& frames) {
    const AVFrame* f = input.get();
    const int w = input->width;
    const int h = input->height;

    // tff bob
    for(int field=1; field>=0; field--) {
        AVFrame* output = new AVFrame;
        output->width = w;
        output->height = h;
        avpicture_alloc((AVPicture*)output, (PixelFormat)f->format, w, h);

        // take even rows of source and double them up on the target
        for(int y=1; y<h; y+=2) {
            for(int x=0; x<w; x++) {
                output->data[0][x+output->linesize[0]*(y-1)] = f->data[0][x+f->linesize[0]*(y-field)];
                output->data[0][x+output->linesize[0]*(y)] = f->data[0][x+f->linesize[0]*(y-field)];
            }
        }
        for(int y=1; y<h/2; y+=2) {
            for(int x=0; x<w/2; x++) {
                output->data[1][x+output->linesize[1]*(y-1)] = f->data[1][x+f->linesize[1]*(y-field)];
                output->data[1][x+output->linesize[1]*(y)] = f->data[1][x+f->linesize[1]*(y-field)];
                output->data[2][x+output->linesize[2]*(y-1)] = f->data[2][x+f->linesize[2]*(y-field)];
                output->data[2][x+output->linesize[2]*(y)] = f->data[2][x+f->linesize[2]*(y-field)];
            }
        }
        frames.push_back(AVFrameSharedPtr(output,AVFrameDeleter()));
    }
}

