#include "linux_dvb_source.h"

#include <iostream>

#include <stdlib.h>
//#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <inttypes.h>

#include <linux/dvb/dmx.h>
#include <linux/dvb/frontend.h>

LinuxDVBSource::LinuxDVBSource(int adapter, int device) : device_(device) {
    frontend_handle = open("/dev/dvb/adapter0/frontend0", O_RDWR);
    if(frontend_handle < 0) {
        std::cerr << "Error opening frontend\n";
        return;
    }

    demux_handle = open("/dev/dvb/adapter0/demux0", O_RDWR);
    if(demux_handle < 0) {
        std::cout << "Could not open demux device\n";
        return;
    }

    dvr_handle = open("/dev/dvb/adapter0/dvr0", O_RDONLY);
    if(dvr_handle < 0) {
        std::cout << "Could not open dvr device\n";
        return;
    }
}

bool LinuxDVBSource::Tune(int frequencykHz) {
    struct dvb_frontend_parameters params;
    params.frequency = frequencykHz*1000;
    params.inversion = INVERSION_AUTO;

    params.u.qam.fec_inner = FEC_2_3;
    params.u.qam.modulation = QAM_256;

    // TODO: Check for EOVERFLOW

    std::cout << "Tuning new parameters\n";
    if(ioctl(frontend_handle, FE_SET_FRONTEND, &params) < 0) {
        std::cout << "Error setting frontend parameters\n";
        return false;
    }
    
    return true;
}

int LinuxDVBSource::Read(uint8_t* buffer, int size) {
    int bytes = read(dvr_handle, buffer, size);
    if(bytes < 0) {
        std::cout << "Error reading from dvr device\n";
        if(errno == EOVERFLOW) return 0;
        return -1;
    }

    return bytes;
}

