#ifndef MULTICAST_SOURCE_H
#define MULTICAST_SOURCE_H

#include <string>
#include "data_source.h"

class MulticastSource : public DataSource {
public:
    MulticastSource(const std::string& ip, int port) : ip(ip), port(port) {}
    virtual ~MulticastSource() { }

    int Read(uint8_t* buffer, int size);

    bool Initialise();

private:
    std::string ip;
    int port;
};


#endif // MULTICAST_SOURCE_H

