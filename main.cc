#include <fstream>
#include <sstream>
#include <iomanip>
#include "multicast_source.h"
#include "libav_demuxer.h"
#include "elementary_stream.h"
#include "file_renderer.h"

ElementaryStream* CreateStream(MulticastSource& source) {
    // Run this in the network thread
    ElementaryStream* stream = new ElementaryStream(ElementaryStream::kVideoStream);
    const int readbufsize = 4096;
    char buf[readbufsize];
    while(true) {
        int bytes = source.Read(buf, readbufsize);
        stream->AddPacket((uint8_t*)buf, (uint8_t*)buf+bytes);
    }
    return stream;
}

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

