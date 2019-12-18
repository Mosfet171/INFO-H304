#include "file.h"

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
        cout << "ça ne va pas" << endl;
    }
}

void File::close()
{
	fclose(m_file);
}
