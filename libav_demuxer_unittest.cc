#include <gtest/gtest.h>

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
        bool init = demuxer_->Initialise(source_);
        ASSERT_TRUE(init);
    }

    LibavDemuxer* demuxer_;
    DataSource* source_;
};

TEST_F(LibavDemuxerTest, InitialiseNoStreams) {
    DoInitialise("test/nostreams.ts");
    std::map<int, Stream*>& streams = demuxer_->GetStreams();
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

