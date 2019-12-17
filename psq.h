#include "file.h"
#include <string>

class Psq : public File
{
    public:
    Psq(std::string name);
    Psq(Psq const& clone);
    ~Psq();
    std::string algo(int *found_pos, bool *parcours);
};
