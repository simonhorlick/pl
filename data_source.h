#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include <cstdint>

class DataSource {
public:
    DataSource() { }
    virtual ~DataSource() { }
    virtual int Read(uint8_t* buffer, int size) = 0;
};

#endif // DATA_SOURCE_H

