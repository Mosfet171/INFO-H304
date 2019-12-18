#include "Phr.h"
#include "protlib.hh"

using namespace std;
Phr::Phr(string name) : File(name)
{
}

/*Phr::Phr(Phr const& clone)
{
}*/
Phr::~Phr()
{
}

string Phr::afficheNom(int lenHeaderString, int *startHeaderString)
{
    char headerStringBuffer[lenHeaderString];
    fseek(m_file, *startHeaderString,SEEK_SET);
    fread(&headerStringBuffer,1,lenHeaderString,m_file);

    // Extracting the title (assuming it is the first visible string) from the header binary file
    // See NCBI BLAST Database format for more information
    int i=0;
    string headerTitle;
    while (headerStringBuffer[i] not_eq 26){
        i++;
    }
    i++;
    if(headerStringBuffer[i] > 127)
    {
        char lenlenString = headerStringBuffer[i]-128;
        i++;
        int j;
        unsigned char lenHeaderTitleBuffer[lenlenString];
        for (j=0;j<lenlenString;j++)
        {
            lenHeaderTitleBuffer[j] = headerStringBuffer[i+j];
        }
        i += j;
        int lenHeaderTitle = bigToLittle(lenHeaderTitleBuffer,j);
        char headerTitleBuffer[lenHeaderTitle];
        for (j=0;j<lenHeaderTitle;j++)
        {
            headerTitleBuffer[j] = headerStringBuffer[i+j];
        }
        headerTitle = charToString(headerTitleBuffer,lenHeaderTitle);
    }
    else
    {
        int j;
        char lenHeaderTitle = headerStringBuffer[i];
        i++;
        char headerTitleBuffer[lenHeaderTitle];
        for (j=0;j<lenHeaderTitle;j++)
        {
            headerTitleBuffer[j] = headerStringBuffer[i+j];
        }
        headerTitle = charToString(headerTitleBuffer,lenHeaderTitle);
    }
}
