#include "data_source.h"
#include <fstream>

class FileSource : public DataSource {
public:
   bool Initialise(const char* filename);

private:
   std::ifstream file_; 
};

