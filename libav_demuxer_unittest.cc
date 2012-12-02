#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <gtest/gtest.h>
#include "libav_demuxer.h"
#include "elementary_stream.h"

extern "C" {
    #include <libavcodec/avcodec.h>
}

class LibavDemuxerTest : public testing::Test {
public:
    ElementaryStream* CreateVideoStream(const char* file) {
        ElementaryStream* stream = new ElementaryStream(ElementaryStream::kVideoStream);
        const int readbufsize = 4096;
        char buf[readbufsize];
        std::ifstream f(file, std::ios::binary);
        if(!f.is_open()) {
            std::cerr << "Error opening " << file << "\n";
            return 0;
        }
        // Read the whole file into memory
        f.read(buf, readbufsize);
        while(!f.eof()) {
            stream->AddPacket((uint8_t*)buf, (uint8_t*)buf+f.gcount());
            f.read(buf, readbufsize);
        }
        return stream;
    }
};

TEST_F(LibavDemuxerTest, Decode) {
    ElementaryStream* s = CreateVideoStream("test/stream.264");
    ASSERT_TRUE(NULL != s);

    LibavDemuxer d;
    d.Initialise(s);

    AVPacket p;
    d.ReadFrame(p);

    std::clog << "Frame of size " << p.size << "\n";

    FAIL();
}

