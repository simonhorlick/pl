#include "file_source.h"

FileSource::FileSource(const char* filename) {
    filename_ = filename;
}

bool FileSource::Initialise() {
    file_.open(filename_, std::ios::binary);
    return file_.is_open();
}

int FileSource::Read(uint8_t* buffer, int size) {
    file_.read(reinterpret_cast<char*>(buffer), size);
    return file_.gcount();
}

