#include <iostream>

#include "libdvbpsi_demuxer.h"
#include "data_source.h"

extern "C" {
    #include <dvbpsi/dvbpsi.h>
    #include <dvbpsi/psi.h>
    #include <dvbpsi/pat.h>
}

static void debug_print(dvbpsi_t *handle, const dvbpsi_msg_level_t level, const char* msg) {
    switch(level) {
        case DVBPSI_MSG_ERROR: std::cout << "Error: "; break;
        case DVBPSI_MSG_WARN:  std::cout << "Warning: "; break;
        case DVBPSI_MSG_DEBUG: std::cout << "Debug: "; break;
        default: return;
    }
    std::cout << msg << '\n';
}

static void PATCallback(void* zero, dvbpsi_pat_t* table) {
    dvbpsi_pat_program_t* program = table->p_first_program;

    std::cout << "PAT\n";
    std::cout << "  transport_stream_id is " << table->i_ts_id << '\n';
    std::cout << "  version_number is " << int(table->i_version) << '\n';
    std::cout << "  active " << table->b_current_next << '\n';
    while(program) {
        std::cout << "  program " << program->i_number << " has pid " << program->i_pid << '\n';
        program = program->p_next;
    }

    dvbpsi_DeletePAT(table);
}

bool LibdvbpsiDemuxer::Initialise(DataSource* source) {
    source_ = source;

    context_ = dvbpsi_NewHandle(&debug_print, DVBPSI_MSG_DEBUG);
    if(!context_) {
        std::cerr << "Failed to initialise libdvbpsi\n";
        return false;
    }

    if(!dvbpsi_AttachPAT(context_, PATCallback, NULL)) {
        std::cerr << "Failed to start PAT callback\n";
        return false;
    }

    return true;
}

void LibdvbpsiDemuxer::PostDemuxTask() {

}

bool LibdvbpsiDemuxer::ReadPacket(uint8_t* buffer) {
    int i = 187;
    int bytes = 1;
    buffer[0] = 0;

    while((buffer[0] != 0x47) && (bytes > 0)) {
        bytes = source_->Read(buffer, 1);
    }

    while((i != 0) && (bytes > 0)) {
        bytes = source_->Read(buffer + 188 - i, i);
        if(bytes >= 0)
            i -= bytes;
    }

    return (i == 0);
}

void LibdvbpsiDemuxer::Demux() {
    uint8_t buffer[4096];
    int ok = ReadPacket(buffer);
    while(ok) {
        uint16_t pid = ((uint16_t)(buffer[1] & 0x1f) << 8) + buffer[2];
        if(pid == 0x0)
            dvbpsi_PushPacket(context_, buffer);
        ok = ReadPacket(buffer);
    }
}

void LibdvbpsiDemuxer::RegisterProgrammeCallback() {

}

