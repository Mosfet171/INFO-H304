#ifndef PIN_H_INCLUDED
#define PIN_H_INCLUDED

#include "protlib.hh"
#include "file.h"
#include <string>

class Pin : public File
{
    public:
    Pin(std::string name);
    Pin(Pin const& clone);
    ~Pin();
    void dataData();
    int tell();
    void seek();
    void seek(int headertableposition, int sequenceIndex);
    void read(char *ptr, int sob);
    int calculData(unsigned char *ptr, int sob, int headerTableInitPos, int found_pos, int *startHeaderString);

    private:
    int m_num_seq;

};


#endif // PIN_H_INCLUDED
