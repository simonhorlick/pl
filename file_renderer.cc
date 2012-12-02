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

void FileRenderer::Draw(AVFrame* picture) {
    if(!picture) return;

    std::clog << "drawing frame\n";
    int width = picture->width;
    int height = picture->height;
    char* y = reinterpret_cast<char*>(picture->data[0]);
    char* u = reinterpret_cast<char*>(picture->data[1]);
    char* v = reinterpret_cast<char*>(picture->data[2]);

    for(int i=0; i<height; i++, y+=picture->linesize[0])
        f.write(y, width);
    for(int i=0; i<height/2; i++, u+=picture->linesize[1])
        f.write(u, width/2);
    for(int i=0; i<height/2; i++, v+=picture->linesize[2])
        f.write(v, width/2);
}

