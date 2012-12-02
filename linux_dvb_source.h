#ifndef LINUX_DVB_SOURCE_H
#define LINUX_DVB_SOURCE_H

#include "data_source.h"

class LinuxDVBSource : public DataSource {
public:
    LinuxDVBSource(int adapter, int device);

    bool Tune(int frequencykHz);

    int Read(uint8_t* buffer, int size);

private:
    int device_;
    int frontend_handle;
    int demux_handle;
    int dvr_handle;
};

#endif // LINUX_DVB_SOURCE_H

