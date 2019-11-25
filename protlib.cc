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

unsigned int readIndexFile(unsigned char * buffer, char sizeOfBuffer, FILE * indexFile){
    fread(buffer,1,sizeOfBuffer,indexFile);
    unsigned int returnLittle = bigToLittle(buffer,sizeOfBuffer);
    return returnLittle;
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

// Functions to insert at the end and delete the first element of an array, not used for now.

/* 
int insertSorted(int arr[], int n, int key, int capacity) { 
    // Cannot insert more elements if n is already more than or equal to capcity 
    if (n >= capacity) 
       return n; 
  
    arr[n] = key; 
  
    return (n + 1);
}

int deleteFirst(int arr[], int n) { 
    int i; 
    for (i = 0; i < n - 1; i++) 
        arr[i] = arr[i + 1]; 
  
    return n - 1; 
} 
*/

// Function to directly decode the BLAST header format, not used for now.

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
