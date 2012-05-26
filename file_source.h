#include "data_source.h"
#include <fstream>

class FileSource : public DataSource {
public:
   FileSource(const char* filename);

   bool Initialise();

   // Read up to size bytes into buffer and return the actual number of bytes
   // read.
   int Read(uint8_t* buffer, int size);

private:
   const char* filename_;
   std::ifstream file_; 
};

