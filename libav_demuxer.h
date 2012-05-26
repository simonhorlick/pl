#ifndef LIBAV_DEMUXER_H
#define LIBAV_DEMUXER_H

#include <vector>
#include <cstdint>
#include "data_source.h"

extern "C"
{
    #include <libavformat/avformat.h>
}

class Stream;

// A Stream will call PostDemuxTask on the Demuxer whenever it requires more
// data. The Demuxer will then read packets and pass these to the correct
// streams until every Stream no longer requires data.

class LibavDemuxer {
public:
    LibavDemuxer();
    ~LibavDemuxer();

    bool Initialise(DataSource* source);

    // This method is called by libav to retrieve data from the DataSource.
    int Read(uint8_t* buffer, int size);

    // Start a demux task that will run in a seperate thread to the callee.
    void PostDemuxTask();

    // The actual method that handles demuxing.
    void Demux();

    std::vector<Stream*>& GetStreams() {
        return streams_;
    }

private:
    std::vector<Stream*> FindStreams();

    DataSource* source_;
    AVFormatContext* format_context_;
    AVIOContext* io_context_;
    unsigned char* io_buffer_;
    std::vector<Stream*> streams_;
};

#endif // LIBAV_DEMUXER_H

