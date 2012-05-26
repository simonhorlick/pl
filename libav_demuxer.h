#ifndef LIBAV_DEMUXER_H
#define LIBAV_DEMUXER_H

#include <map>
#include "stream.h"
#include "data_source.h"

class LibavDemuxer {
public:
    LibavDemuxer();
    ~LibavDemuxer();

    bool Initialise(DataSource* source);

    std::map<int, Stream*>& GetStreams() {
        return streams_;
    }

private:
    DataSource* source_;
    std::map<int, Stream*> streams_;
};

#endif // LIBAV_DEMUXER_H

