#ifndef PHR_H_INCLUDED
#define PHR_H_INCLUDED

#include "file.h"
#include <string>
#include <vector>

class Phr : public File
{
    public:
    Phr(std::string name);
    Phr(Phr const& clone);
    ~Phr();
    std::string afficheNom(int lenHeaderString, int *startHeaderString);

};


#endif // PHR_H_INCLUDED
