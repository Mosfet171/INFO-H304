#include <string>
#include "protlib.hh"

int bigToLittle(unsigned char buffer[], int n){
    int i;
    int result = (int)buffer[0]<<(n-1)*8;
    for (i=1;i<4;i++){
        result = result | (int)buffer[i]<<(n-1-i)*8;
    }
    return result;
}

std::string charToString(char* a, int size) 
{ 
    int i; 
    std::string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
} 

/*
std::string blastHeaderDecode(char stringBuffer[]){
    int i = 0;
    while(stringBuffer[i] == 48 || stringBuffer[i] == -128 || stringBuffer[i] == -96)
        i++;
    
    switch(stringBuffer[i]){
        case 26:
            i++;
            if(stringBuffer[i] > 128){
                char lenlenString = stringBuffer[i]-128;
                i++;
                int j;
                char lenHeaderTitleBuffer[lenlenString];
                for (j=0;j<lenlenString;j++){
                    lenHeaderTitleBuffer[j] = stringBuffer[i+j];
                }
                i += j;
                int lenHeaderTitle = bigToLittle(lenHeaderTitleBuffer);
                char headerTitleBuffer[lenHeaderTitle];
                for (j=0;j<lenHeaderTitle;j++){
                    headerTitleBuffer[j] = stringBuffer[i+j];
                }
                std::string headerTitle = charToString(headerTitleBuffer);
            } else {
                int j;
                char lenHeaderTitle = stringBuffer[i];
                char headerTitleBuffer[lenHeaderTitle];
                for (j=0;j<lenHeaderTitle;j++){
                    headerTitleBuffer[j] = stringBuffer[i+j];
                }
                std::string headerTitle = charToString(headerTitleBuffer);
            }
            return headerTitle;
        default:
        std::string headerTitle = std::string("Hey, failed.");
        return headerTitle;
    }
}
*/
