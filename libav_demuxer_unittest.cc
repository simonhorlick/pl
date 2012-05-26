#include <gtest/gtest.h>
#include "libav_demuxer.h"
#include "file_source.h"

class LibavDemuxerTest : public testing::Test {
public:
    LibavDemuxerTest() {
        demuxer_ = new LibavDemuxer();
        source_ = 0;
    }

    ~LibavDemuxerTest() {
        delete demuxer_;
        if(source_) delete source_;
    }

    void DoInitialise(const char* filename) {
        ASSERT_TRUE(source_ == 0);
        source_ = new FileSource(filename);
        ASSERT_TRUE(source_->Initialise());
        ASSERT_TRUE(demuxer_->Initialise(source_));
    }

    LibavDemuxer* demuxer_;
    DataSource* source_;
};

TEST_F(LibavDemuxerTest, InitialiseNoStreams) {
    DoInitialise("test/nostreams.ts");
    std::vector<Stream*>& streams = demuxer_->GetStreams();
    EXPECT_TRUE(streams.empty());
}

TEST_F(LibavDemuxerTest, InitialiseNoRecognisedStreams) {
    FAIL();
}

TEST_F(LibavDemuxerTest, InitialiseMultipleVideoStreams) {
    FAIL();
}

TEST_F(LibavDemuxerTest, InitialiseNormal) {
    FAIL();
}

TEST_F(LibavDemuxerTest, ReadAudioPacket) {
    FAIL();
}

TEST_F(LibavDemuxerTest, ReadAudioPacketsFromMultipleStreams) {
    FAIL();
}

TEST_F(LibavDemuxerTest, ReadVideoPacket) {
    FAIL();
}

TEST_F(LibavDemuxerTest, ReadEndOfStream) {
    FAIL();
}

