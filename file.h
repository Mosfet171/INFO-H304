#ifndef LOL
#define LOL


#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <chrono>
class File
{
    public:
        File(std::string name);
        File(File const& clone);
        ~File();
        void open(std::string name);

    protected:
        std::string m_name;
        FILE * m_file;
    //open
};
#endif
