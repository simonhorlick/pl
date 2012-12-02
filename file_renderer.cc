#include "file_renderer.h"
#include "libav_video_decoder.h"
#include <string>
#include <iostream>
#include <fstream>

FileRenderer::FileRenderer() {}

FileRenderer::~FileRenderer() {
    if(f.is_open())
        f.close();
}

void FileRenderer::Initialise(std::string file) {
    f.open(file.c_str(), std::ofstream::binary);
    if(!f.is_open())
        std::cerr << "Could not open output file\n";
}

void FileRenderer::Draw(const AVFrameSharedPtr& picture) {
    std::clog << "drawing frame\n";
    const AVFrame* p = picture.get();
    int width = p->width;
    int height = p->height;
    char* y = reinterpret_cast<char*>(p->data[0]);
    char* u = reinterpret_cast<char*>(p->data[1]);
    char* v = reinterpret_cast<char*>(p->data[2]);

    for(int i=0; i<height; i++, y+=p->linesize[0])
        f.write(y, width);
    for(int i=0; i<height/2; i++, u+=p->linesize[1])
        f.write(u, width/2);
    for(int i=0; i<height/2; i++, v+=p->linesize[2])
        f.write(v, width/2);
}

