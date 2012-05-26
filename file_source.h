#include "data_source.h"
#include <fstream>

class FileSource : public DataSource {
public:
   FileSource(const char* filename);

   bool Initialise();

private:
   const char* filename_;
   std::ifstream file_; 
};

