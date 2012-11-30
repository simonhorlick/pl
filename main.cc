#include <fstream>
#include <sstream>
#include <iomanip>
#include "multicast_source.h"
#include "libav_demuxer.h"
#include "elementary_stream.h"
#include "file_renderer.h"

int main(int argc, char** argv) {
    MulticastSource source("239.255.0.54", 5004);
 
    // connect
    source.Initialise();

    LibavDemuxer demuxer;
    demuxer.Initialise(source);

    LibavVideoDecoder decoder;
    decoder.Initialise(&demuxer);

    renderer.Initialise("output.yuv");

    ReadPictureCallback cb = &FrameDecodeCallbackCache;
    while(decoder.ReadFrame(cb));
}

