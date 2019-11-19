#include <string>
#include "protlib.hh"

int bigToLittle(unsigned char buffer[4]){
    int result = (int)buffer[0]<<24 | (int)buffer[1]<<16 | (int)buffer[2]<<8 | (int)buffer[3];
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
