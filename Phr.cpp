#include "Phr.h"

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

string Phr::afficheNom(int lenHeaderString, int startHeaderString, char *yop[])
{
    char headerStringBuffer[lenHeaderString];
    fseek(m_file,startHeaderString,SEEK_SET);
    fread(yop,1,lenHeaderString,fhr);

    i=0;
    string headerTitle;
    while (headerStringBuffer[i] not_eq 26){
        i++;
    }
    i++;
    if(headerStringBuffer[i] > 127){
        char lenlenString = headerStringBuffer[i]-128;
        i++;
        int j;
        unsigned char lenHeaderTitleBuffer[lenlenString];
        for (j=0;j<lenlenString;j++){
            lenHeaderTitleBuffer[j] = headerStringBuffer[i+j];
        }
        i += j;
        int lenHeaderTitle = bigToLittle(lenHeaderTitleBuffer,j);
        char headerTitleBuffer[lenHeaderTitle];
        for (j=0;j<lenHeaderTitle;j++){
            headerTitleBuffer[j] = headerStringBuffer[i+j];
        }
        headerTitle = charToString(headerTitleBuffer,lenHeaderTitle);
    } else {
        int j;
        char lenHeaderTitle = headerStringBuffer[i];
        i++;
        char headerTitleBuffer[lenHeaderTitle];
        for (j=0;j<lenHeaderTitle;j++){
            headerTitleBuffer[j] = headerStringBuffer[i+j];
        }
        headerTitle = charToString(headerTitleBuffer,lenHeaderTitle);
    }
    return headerTitle;

}
