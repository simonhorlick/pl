#include <gtest/gtest.h>
#include "dvb_demuxer.h"
#include "file_source.h"

#include <string>

class DVBDemuxerTest : public testing::Test {
public:
    DVBDemuxerTest() {
        demuxer_ = new DVBDemuxer();
        source_ = 0;
    }

    ~DVBDemuxerTest() {
        delete demuxer_;
        if(source_) delete source_;
    }

    void DoInitialise(const char* filename) {
        ASSERT_TRUE(source_ == 0);
        source_ = new FileSource(filename);
        ASSERT_TRUE(source_->Initialise());
        ASSERT_TRUE(demuxer_->Initialise(source_));
    }

    DVBDemuxer* demuxer_;
    DataSource* source_;
};

TEST_F(DVBDemuxerTest, ParseProgrammes) {
    demuxer_
    FAIL();
}


