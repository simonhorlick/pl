#include "linux_dvb_source.h"
#include <fstream>
#include <iostream>

int main(int argc, const char *argv[]) {
    LinuxDVBSource source(0,0);
    source.Tune(545800);

    std::ofstream dest("output.ts", std::ios::binary);
    if(!dest.is_open()) {
        std::cerr << "Failed to open output file\n";
        return 1;
    }

    int counter = 0;

    uint8_t buffer[4096];
    while(counter < 10000) {
        int bytes = source.Read(buffer, 4096);
        if(bytes < 0) break;
        dest.write(reinterpret_cast<const char*>(buffer), bytes);
    }
    dest.close();
    return 0;
}

