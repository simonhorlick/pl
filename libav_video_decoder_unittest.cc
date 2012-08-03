#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <gtest/gtest.h>
#include "libav_video_decoder.h"
#include "elementary_stream.h"

extern "C" {
    #include <libavcodec/avcodec.h>
}

class LibavDecoderTest : public testing::Test {
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

void WritePGM(const char *buf, int pitch, int width, int height,
        const std::string& filename) {
    std::ofstream f(filename.c_str(), std::ios::binary);
    f << "P5\n"
      << width << " " << height << "\n"
      << 255 << "\n";
    for(int i=0; i<height; i++)
        f.write(buf + i*pitch, width);
    f.close();
}

void FrameDecodeCallback(AVFrame* f) {
    std::clog << "Got " << av_get_picture_type_char(f->pict_type) << " frame\n";
    std::ostringstream oss;
    oss << "frame" << std::setw(6) << (int)f->coded_picture_number << ".pgm";
    WritePGM(reinterpret_cast<const char*>(f->data[0]),
        f->linesize[0], 1920, 1080, oss.str());
}

TEST_F(LibavDecoderTest, Decode) {
    std::cout << "Initialising video decoder\n";
    LibavVideoDecoder d;
    d.Initialise(CreateVideoStream("test/stream.264"));

    ReadPictureCallback cb = &FrameDecodeCallback;
    std::cout << "Calling GetFrame\n";
    for(int i=0; i<5; i++)
        d.ReadFrame(cb);
    std::cout << "Done\n";
}

TEST_F(LibavDecoderTest, DecodePartialFrame) {
    FAIL();
}

TEST_F(LibavDecoderTest, DecodeEnd) {
    FAIL();
}

