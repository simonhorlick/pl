#include "linux_dvb_source.h"

LinuxDVBSource::LinuxDVBSource(int adapter, int device) : device_(device) {
    frontend_handle = open("/dev/dvb/adapter0/frontend0", O_RDWR);
    if(frontend_handle < 0) {
        printf("Error opening frontend\n");
        return -1;
    }

    demux_handle = open("/dev/dvb/adapter0/demux0", O_RDWR);
    if(demux_handle < 0) {
        printf("Could not open demux device\n");
        return -1;
    }

    dvr_handle = open("/dev/dvb/adapter0/dvr0", O_RDONLY);
    if(dvr_handle < 0) {
        printf("Could not open dvr device\n");
        return -1;
    }
}

bool LinuxDVBSource::Tune(int frequencykHz) {
    struct dvb_frontend_parameters params;
    params.frequency = frequencykHz*1000;
    params.inversion = INVERSION_AUTO;

    params.u.qam.fec_inner = FEC_2_3;
    params.u.qam.modulation = QAM_256;

    // TODO: Check for EOVERFLOW

    printf("Tuning new parameters\n");
    if(ioctl(frontend_handle, FE_SET_FRONTEND, &params) < 0) {
        printf("Error setting frontend parameters\n");
        return false;
    }
    
    return true;
}

int LinuxDVBSource::Read(uint8_t* buffer, int size) {
    int bytes = read(dvr_handle, buffer, size);
    if(bytes < 0) {
        printf("Error reading from dvr device\n");
        if(errno == EOVERFLOW) return 0;
        return -1;
    }

    return bytes;
}

