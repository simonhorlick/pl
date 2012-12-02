#ifndef __FILE_RENDERER_H__
#define __FILE_RENDERER_H__

#include <string>
#include <fstream>

#include "libav_video_decoder.h"
#include "frame.h"

class FileRenderer {
public:
    FileRenderer();
    ~FileRenderer();

    void Initialise(std::string file);

    void Draw(const AVFrameSharedPtr& picture);

private:
    std::ofstream f;
};

#endif // __FILE_RENDERER_H__

