#include "file_source.h"
#include "libdvbpsi_demuxer.h"

int main(int argc, char** argv) {
    FileSource source("/home/simon/output.ts");
    source.Initialise();

    LibdvbpsiDemuxer demuxer;
    demuxer.Initialise(&source);
    demuxer.Demux();
}

