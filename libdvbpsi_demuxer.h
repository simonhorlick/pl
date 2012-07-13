#ifndef LIBDVBPSI_DEMUXER_H
#define LIBDVBPSI_DEMUXER_H

#include <stdint.h>
#include <map>

class DataSource;
typedef struct dvbpsi_s dvbpsi_t;

class LibdvbpsiDemuxer {
public:

    LibdvbpsiDemuxer() : source_(0), context_(0) {}

    bool Initialise(DataSource* source);

    void Cleanup();

    void PostDemuxTask();

    void Demux();

    void RegisterProgrammeCallback();

    // FIXME: this sucks
    std::map<uint16_t, dvbpsi_t*>& GetPMTMap() {
        return pmt_;
    }

private:
    bool ReadPacket(uint8_t* buffer);

    DataSource* source_;
    dvbpsi_t* context_;
    std::map<uint16_t, dvbpsi_t*> pmt_;
};

#endif // LIBDVBPSI_DEMUXER_H

