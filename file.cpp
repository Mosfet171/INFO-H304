#include "File.h"

using namespace std;
File::File(string name) : m_name(name)
{
    open(name);
}
File::File(File const& clone)
{
}
File::~File()
{
}
void File::open(string name)
{
    m_file = fopen(name.c_str(), "rb");
    if(m_file == NULL)
    {
        cout << "�a ne va pas" << endl;
    }
}
