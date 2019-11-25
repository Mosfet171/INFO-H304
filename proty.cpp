/* Preliminary Version of the Protein Sequence Alignment Project
for the INFP-H304 course, by Alissa Komino, Jean-Charles Nsangolo and Arkady Mospan */

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "protlib.hh"

#define MAX_LEN 120

using namespace std;

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
 
int main (int argc, char** argv)
{
    // If there is no arguments, we explain how to use the proty program
    if (argc == 1){
        cout << endl << "Proty usage : proty <path to database> <path to query file> OPTIONAL : <> <>" << endl << endl ;
        exit(1);
    }

    // Declaring variables
    FILE * fin;
    FILE * fsq;
    FILE * fhr;
    FILE * fquery;
    int i;
    // char a[10];
    unsigned char buffer[4];
    int sob = sizeof(buffer);

    // Extracting database info from 'index' file
    string indexname = argv[1]+string(".pin");
    fin = fopen(indexname.c_str(),"rb");
    if (fin == NULL){
        cout << "Opening of 'index' file impossible." << endl;
        exit(1);
    }
    
    fread(&buffer,1,sob,fin);
    int version = bigToLittle(buffer,sob); // Version

    fread(&buffer,1,sob,fin);
    int intdbtype = bigToLittle(buffer,sob); // Type (DNA or Protein)
    string dbtype;
    if (intdbtype == 1) {
        dbtype = "Protein";
    }
    else
    {
        dbtype = "DNA";
    }
    
    fread(&buffer,1,sob,fin);
    int len_title = bigToLittle(buffer,sob); // Length 'T' of title
    char titlebuffer[len_title];

    fread(&titlebuffer,1,len_title,fin);
    string title = charToString(titlebuffer,len_title); // Title

    fread(&buffer,1,sob,fin);
    int len_timestamp = bigToLittle(buffer,sob); // Length 'S' of timestamp
    char timestampbuffer[len_timestamp];

    fread(&timestampbuffer,1,len_timestamp,fin);
    string timestamp = charToString(timestampbuffer,len_timestamp); // Timestamp

    fread(&buffer,1,sob,fin);
    long int num_seq = bigToLittle(buffer,sob); // Number of sequences

    unsigned long long int num_residue; // Using long long because dealing with a 8 byte int here (and definitely positive)
    fread(&num_residue,1,8,fin); // Number of residues

    fread(&buffer,1,sob,fin);
    int len_maxseq = bigToLittle(buffer,sob); // Length of longest sequence

    // ---------- CONTROL PART ---------- //
    cout << endl
        << "Version : " << version << endl
        << "DB type : " << dbtype << endl 
        << "Title length : " << len_title << endl
        << "File title : " << title << endl 
        << "Timestamp : " << timestamp << endl
        << "Number of sequences : " << num_seq << endl
        << "Number of residues : " << num_residue << endl
        << "Length of maximum sequence : " << len_maxseq << " residues" << endl
        << endl;
    // ---------------------------------- //

    // Extracting query sequence
    string queryname = argv[2];
    fquery = fopen(queryname.c_str(),"r");
    if (fquery == NULL){
        cout << "Opening of 'query' file impossible." << endl;
        exit(1);
    }
    if (queryname.substr(queryname.length()-5,5) == "fasta"){
        char querytitle[1000];
        fscanf(fquery,"%[^\n]",querytitle);
        cout << "Query title : " << querytitle << endl;
    }

    // Transforming it into pure continued string (without \n nor \r)
    int c;
    string query;
    while ((c = getc(fquery)) != EOF){
        if ((c not_eq '\n' && c not_eq '\r')){
            query += c;
        }
    }

    /*
    // Char to int
    v['A'] = 1; v['B'] = 2; v['C'] = 3; v['D'] = 4; v['E'] = 5; v['F'] = 6; v['G'] = 7; v['H'] = 8; v['I'] = 9; v['J'] = 27; 
    v['K'] = 10; v['L'] = 11; v['M'] = 12; v['N'] = 13; v['O'] = 26; v['P'] = 14; v['Q'] = 15; v['R'] = 16; v['S'] = 17; 
    v['T'] = 18; v['U'] = 24; v['V'] = 19; v['W'] = 20; v['X'] = 21; v['Y'] = 22; v['Z'] = 23; v['*'] = 25; v['-'] = 0; 
    */

    // Int to char
    char v[28] = {'-','A','B','C','D','E','F','G','H','I','K','L','M','N','P','Q','R','S','T','V','W','X','Y','Z','U','*','Q','J'};

    int len_query = query.size();

    // ----------------------------- REAL BUSINESS ----------------------------- //

    /* This part can obviously be done in another way, as transforming everything to a string from the std::string
    package is maybe not the best option memory-ly speaking; however, at least for the preliminary project, it 
    is simpler to use and verify. 
    */

    // Reading from database sequence file (.psq) an array of char of same length as the query
    string sequencename = argv[1] + string(".psq");
    fsq = fopen(sequencename.c_str(),"rb");
    if (fsq == NULL){
        cout << "Opening of 'sequence' file impossible." << endl;
        exit(1);
    }
    unsigned char substring[len_query];
    fread(&substring,1,len_query,fsq);

    // Transforming the array of char into a continued string
    string dbsubstr;
    for (i=0;i<len_query;i++){
        dbsubstr += v[(int)substring[i]];
    }

    // Comparison code between query and string in database
    unsigned char key_bin;
    int found_pos = 0;
    while (dbsubstr not_eq query){
        fread(&key_bin,1,1,fsq);
        if ((int)key_bin == EOF){
            cout << "Sequence not found" << endl;
            break;
        }
        dbsubstr += v[(int)key_bin];
        dbsubstr.erase(0,1);
        found_pos ++;
    }

    cout << "Sequence found !! At pos : " << found_pos << endl ;

    long int headerTableInitPos = ftell(fin); // Getting the position of the beggining of the Header offset table
    cout << "Header init : " << headerTableInitPos << endl;

    fseek(fin,(num_seq+1)*4,SEEK_CUR); // The header table is num_seq + 1 long, and each "seq" takes 4 bytes long, hence num_seq*4+1
    long int sequenceTableInitPos = ftell(fin); // Getting the position of the beggining of the Sequence offset table
    cout << "Sequence init : " << sequenceTableInitPos << endl;

    // Calculating where in the database the sequence is found (which index)
    int sequenceOffsetValue = 0;
    int sequenceIndex = 0;
    while (sequenceOffsetValue < found_pos){
        fread(&buffer,1,sob,fin);
        sequenceOffsetValue = bigToLittle(buffer,sob);
        sequenceIndex++;
    }
    
    sequenceIndex -- ; //To be consistent : we read then ++ then check, so the good one is one less than the last index returned
    cout << sequenceIndex << endl ;

    // Seeking in the header table the length of the header
    fseek(fin,headerTableInitPos+4*(sequenceIndex),SEEK_SET);
    fread(&buffer,1,sob,fin);
    int startHeaderString = bigToLittle(buffer,sob);
    fread(&buffer,1,sob,fin);
    int endHeaderString = bigToLittle(buffer,sob);
    int lenHeaderString = endHeaderString - startHeaderString;

    // Reading the header string in the header file (.phr)
    string headername = argv[1] + string(".phr");
    fhr = fopen(headername.c_str(),"rb");
    if (fhr == NULL){
        cout << "Opening of 'header' file impossible." << endl;
        exit(1);
    }
    char headerStringBuffer[lenHeaderString];
    fseek(fhr,startHeaderString,SEEK_SET);
    fread(&headerStringBuffer,1,lenHeaderString,fhr);

    // See NCBI BLAST Database format for more information
    /*
    switch(headerStringBuffer[6]){
        case 26: // 1A in hex, 
            if (headerStringBuffer[7] > 128){
                char lenlenHeaderTitle = headerStringBuffer[7]-128;
                int lenHeaderTitle = headerStringBuffer[7+]
            }
            char lenHeaderTitle = headerStringBuffer[7];
    }
    */
    

    cout << endl 
        << startHeaderString << endl
        << endHeaderString << endl 
        << lenHeaderString << endl
        << (int)headerStringBuffer[4] << "\t" << (int)headerStringBuffer[5] << endl
        << endl;

    fseek(fhr,1434,SEEK_SET);
    char buffer2;
    fread(&buffer2,1,1,fhr);
    cout << (int)buffer2+256 << endl;

    fclose(fin);
    fclose(fsq);
    fclose(fquery);
    fclose(fhr);
    return 0;
}
