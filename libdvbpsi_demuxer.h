#ifndef LIBDVBPSI_DEMUXER_H
#define LIBDVBPSI_DEMUXER_H

#include <stdint.h>

class DataSource;
typedef struct dvbpsi_s dvbpsi_t;

class LibdvbpsiDemuxer {
public:

    LibdvbpsiDemuxer() : source_(0), context_(0) {}

    bool Initialise(DataSource* source);

    void PostDemuxTask();

    void Demux();

    void RegisterProgrammeCallback();

private:
    bool ReadPacket(uint8_t* buffer);

    DataSource* source_;
    dvbpsi_t* context_;
};

#endif // LIBDVBPSI_DEMUXER_H

