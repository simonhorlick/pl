#include "file_source.h"

FileSource::FileSource(const char* filename) {
    filename_ = filename;
}

bool FileSource::Initialise() {
    return false;
}

