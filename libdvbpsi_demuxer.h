#ifndef LIBDVBPSI_DEMUXER_H
#define LIBDVBPSI_DEMUXER_H

class DataSource;

class LibdvbpsiDemuxer {
public:

    bool Initialise(DataSource* source);

    void PostDemuxTask();

    void Demux();

    void RegisterProgrammeCallback();
};

#endif // LIBDVBPSI_DEMUXER_H

