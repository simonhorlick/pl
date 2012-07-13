#include <iostream>
#include <map>

#include "libdvbpsi_demuxer.h"
#include "data_source.h"

extern "C" {
    #include <dvbpsi/dvbpsi.h>
    #include <dvbpsi/psi.h>
    #include <dvbpsi/pat.h>
    #include <dvbpsi/descriptor.h>
    #include <dvbpsi/pmt.h>
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

static const char* GetStreamTypeString(uint8_t type) {
    switch (type) {
        case 0x00: return "Reserved";
        case 0x01: return "ISO/IEC 11172 Video";
        case 0x02: return "ISO/IEC 13818-2 Video";
        case 0x03: return "ISO/IEC 11172 Audio";
        case 0x04: return "ISO/IEC 13818-3 Audio";
        case 0x05: return "ISO/IEC 13818-1 Private Section";
        case 0x06: return "ISO/IEC 13818-1 Private PES data packets";
        case 0x07: return "ISO/IEC 13522 MHEG";
        case 0x08: return "ISO/IEC 13818-1 Annex A DSM CC";
        case 0x09: return "H222.1";
        case 0x0A: return "ISO/IEC 13818-6 type A";
        case 0x0B: return "ISO/IEC 13818-6 type B";
        case 0x0C: return "ISO/IEC 13818-6 type C";
        case 0x0D: return "ISO/IEC 13818-6 type D";
        case 0x0E: return "ISO/IEC 13818-1 auxillary";
        default:
            if (type < 0x80)
                return "ISO/IEC 13818-1 reserved";
            else
                return "User Private";
    }
}

static void PMTCallback(void* zero, dvbpsi_pmt_t* table) {
    std::cout << "PMT\n";
    std::cout << "  program number is " << table->i_program_number << '\n';
    std::cout << "  version number is " << table->i_version << '\n';
    std::cout << "  pcr pid is " << table->i_pcr_pid << '\n';

    // descriptors: p_pmt->p_first_descriptor

    dvbpsi_pmt_es_t* stream = table->p_first_es;
    std::cout << "  elementary streams:\n";
    while(stream) {
        std::cout << "    pid is " << stream->i_pid << " type " <<
            GetStreamTypeString(stream->i_type) << '\n';
        stream = stream->p_next;
    }

    dvbpsi_DeletePMT(table);
}

// Create a PMT handler for each entry in the PAT
static void PATCallback(void* opaque, dvbpsi_pat_t* table) {
    LibdvbpsiDemuxer* demuxer = reinterpret_cast<LibdvbpsiDemuxer*>(opaque);
    std::map<uint16_t, dvbpsi_t*>& pmt_ = demuxer->GetPMTMap();
    dvbpsi_pat_program_t* program = table->p_first_program;

    std::cout << "PAT\n";
    std::cout << "  transport_stream_id is " << table->i_ts_id << '\n';
    std::cout << "  version_number is " << int(table->i_version) << '\n';
    std::cout << "  active " << table->b_current_next << '\n';
    while(program) {
        std::cout << "  program " << program->i_number << " has pid " << program->i_pid << '\n';

        std::map<uint16_t, dvbpsi_t*>::iterator it = pmt_.find(program->i_pid);
        if(it != pmt_.end()) {
            std::clog << "Replacing existing PMT listener for pid " << program->i_pid << '\n';
            dvbpsi_DetachPMT(it->second);
            dvbpsi_DeleteHandle(it->second);
            pmt_.erase(it);
        }

        std::clog << "Attaching new PMT listener";
        pmt_[program->i_pid] = dvbpsi_NewHandle(&debug_print, DVBPSI_MSG_DEBUG);
        if(dvbpsi_AttachPMT(pmt_[program->i_pid], program->i_number, PMTCallback, 0)) {
            std::cerr << "Failed to start PMT callback\n";
            return false;
        }

        program = program->p_next;
    }

    dvbpsi_DeletePAT(table);
}

bool LibdvbpsiDemuxer::Initialise(DataSource* source) {
    source_ = source;

    pat_handle_ = dvbpsi_NewHandle(&debug_print, DVBPSI_MSG_DEBUG);
    if(!pat_handle_) {
        std::cerr << "Failed to initialise libdvbpsi\n";
        return false;
    }

    // A PAT contains n PMT references. 
    if(!dvbpsi_AttachPAT(pat_handle_, PATCallback, NULL)) {
        std::cerr << "Failed to start PAT callback\n";
        return false;
    }

    return true;
}

void LibdvbpsiDemuxer::Cleanup() {
    if(pat_handle_) {
        dvbpsi_DetachPAT(pat_handle_);
        dvbpsi_DeleteHandle(pat_handle_);
    }
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
        if(pid == 0x0) {
            std::clog << "Got PAT packet (pid 0)\n";
            dvbpsi_PushPacket(pat_handle_, buffer);
        } else if(pmt_.find(pid) != pmt_.end()) {
            std::clog << "Got PMT packet (pid "<<pid<<")\n";
            dvbpsi_PushPacket(pmt_[pid], buffer);
        }
        ok = ReadPacket(buffer);
    }
}

void LibdvbpsiDemuxer::RegisterProgrammeCallback() {

}

