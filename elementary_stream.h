#ifndef ELEMENTARY_STREAM_H
#define ELEMENTARY_STREAM_H

#include <vector>
#include <algorithm>
#include <stdint.h>
#include <cassert>

class ElementaryStream {
public:
    enum StreamType {
        kVideoStream,
        kAudioStream,
        kSubtitleStream,
        kUnknownStream
    };

    ElementaryStream(StreamType type) : type_(type) {}

    StreamType GetStreamType() { return type_; }

    bool IsPCR() { return ispcr_; }

    void AddPacket(uint8_t* begin, uint8_t* end) {
        std::copy(begin, end, std::back_inserter(buf_));
    }

    // Copy up to size bytes of the internal buffer, buf_, to the callees
    // buffer buf.
    int Read(uint8_t* buf, int size) {
        assert(buf && size);
        int read = std::min((int)buf_.size(), size);
        assert(read);
        std::copy(buf_.begin(), buf_.begin() + read, buf);
        buf_.erase(buf_.begin(), buf_.begin() + read);
        return read;
    }

private:
    StreamType type_;
    bool ispcr_;
    std::vector<uint8_t> buf_;
};

#endif // ELEMENTARY_STREAM_H

