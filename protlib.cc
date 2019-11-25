#include <string>
#include "protlib.hh"

int bigToLittle(unsigned char buffer[], int n){
    int i;
    int result = (int)buffer[0]<<(n-1)*8;
    for (i=1;i<4;i++){
        result = result | (int)buffer[i]<<(n-1-i)*8;
    }
    
    //int result = (int)buffer[0]<<24 | (int)buffer[1]<<16 | (int)buffer[2]<<8 | (int)buffer[3];
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
