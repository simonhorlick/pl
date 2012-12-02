#include <gtest/gtest.h>
#include <list>
#include <boost/bind.hpp>
#include "bob.h"

#include "elementary_stream.h"
#include "libav_video_decoder.h"
#include "libav_demuxer.h"
#include "file_renderer.h"

extern "C" {
    #include <libavcodec/avcodec.h>
}

class BobTest : public testing::Test {
public:

    ElementaryStream* CreateVideoStream(const char* file) {
        ElementaryStream* stream = new ElementaryStream(ElementaryStream::kVideoStream);
        const int readbufsize = 4096;
        char buf[readbufsize];
        std::ifstream f(file, std::ios::binary);
        if(!f.is_open()) {
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

    void ReadAndDecodeFile(const char* filename) {
        ElementaryStream* stream = CreateVideoStream(filename);
        ASSERT_TRUE(NULL != stream) << "Error opening " << filename << "\n";

        LibavDemuxer demuxer;
        demuxer.Initialise(stream);

        LibavVideoDecoder decoder;
        decoder.Initialise(&demuxer);

        ReadPictureCallback cb(
                boost::bind(&BobTest::FrameDecodeCallbackCache, this, _1));
        while(decoder.ReadFrame(cb));
    }

    void FrameDecodeCallbackCache(AVFrame* f) {
        std::clog << "decoded frame\n"
            << "  coded " << f->coded_picture_number << "\n"
            << "  display " << f->display_picture_number << "\n"
            << "  interlaced " << f->interlaced_frame << "\n"
            << "  key frame " << f->key_frame << "\n"
            << "  picture type " << av_get_picture_type_char(f->pict_type) << "\n"
            << "  pts " << f->pts << "\n"
            << "  packet pts " << f->pkt_pts << "\n"
            << "  packet dts " << f->pkt_dts << "\n";
        // Take a copy of the frame
        AVFrame* picture = new AVFrame;
        // TODO: Need to copy rest of the AVFrame structure?
        //avpicture_alloc(picture, f->format, f->width, f->height);
        av_picture_copy((AVPicture*)picture, (AVPicture*)f, (PixelFormat)f->format, f->width, f->height);
        frames.push_back(picture);
    }

protected:
    std::list<AVFrame*> frames;
};

TEST_F(BobTest, Test) {
    ReadAndDecodeFile("test/stream2.264");

    FileRenderer renderer;
    renderer.Initialise("output.yuv");

    for(std::list<AVFrame*>::iterator it = frames.begin(); it != frames.end(); ++it) {
        std::list<AVFrame*> bobbed;
        bob(*it, bobbed);
        for(std::list<AVFrame*>::iterator jt = bobbed.begin(); jt != bobbed.end(); ++jt) {
            renderer.Draw(*jt);
        }
    }
}

