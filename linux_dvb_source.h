#ifndef LINUX_DVB_SOURCE_H
#define LINUX_DVB_SOURCE_H

#include "data_source.h"

class LinuxDVBSource : public DataSource {
public:
    LinuxDVBSource(int device);

    bool Tune(int frequencykHz);

    int Read(uint8_t* buffer, int size);

private:
    int device_;
};

#endif // LINUX_DVB_SOURCE_H

