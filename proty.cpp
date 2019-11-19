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
    FILE * fseq;
    FILE * fquery;
    int i;
    // char a[10];
    unsigned char buffer[4];

    // Extracting database info from 'index' file
    string indexname = argv[1]+string(".pin");
    fin = fopen(indexname.c_str(),"rb");
    if (fin == NULL){
        cout << "Opening of 'index' file impossible." << endl;
        exit(1);
    }
       
    fread(&buffer,1,4,fin);
    int version = bigToLittle(buffer); // Version

    fread(&buffer,1,4,fin);
    int intdbtype = bigToLittle(buffer); // Type (DNA or Protein)
    string dbtype;
    if (intdbtype == 1) {
        dbtype = "Protein";
    }
    else
    {
        dbtype = "DNA";
    }
    
    fread(&buffer,1,4,fin);
    int len_title = bigToLittle(buffer); // Length 'T' of title
    char titlebuffer[len_title];

    fread(&titlebuffer,1,len_title,fin);
    string title = charToString(titlebuffer,len_title); // Title

    fread(&buffer,1,4,fin);
    int len_timestamp = bigToLittle(buffer); // Length 'S' of timestamp
    char timestampbuffer[len_timestamp];

    fread(&timestampbuffer,1,len_timestamp,fin);
    string timestamp = charToString(timestampbuffer,len_timestamp); // Timestamp

    fread(&buffer,1,4,fin);
    int num_seq = bigToLittle(buffer); // Number of sequences

    unsigned long long int num_residue;
    fread(&num_residue,1,8,fin); // Number of residues

    fread (&buffer,1,4,fin);
    int len_maxseq = bigToLittle(buffer); // Length of longest sequence

    // ---------- CONTROL PART ---------- //
    cout << endl
        << "Version : " << version << endl
        << "DB type : " << dbtype << endl 
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
    if (queryname.substr(queryname.length()-5,5) == "fasta"){
        char querytitle[1000];
        fscanf(fquery,"%[^\n]",querytitle);
        cout << querytitle << endl;
    }
    int c;
    string query;
    while ((c = getc(fquery)) != EOF){
        if ((c not_eq '\n')){
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

    char test = v[27];
    cout << test << endl;
    int len_query = query.size();
    cout << len_query << endl ;

    // ----------------------------- REAL BUSINESS ----------------------------- //
    string sequencename = argv[1] + string(".psq");
    fseq = fopen(sequencename.c_str(),"rb");
    unsigned char substring[len_query+1];
    fread(&substring,1,len_query,fseq);


    string dbsubstr;
    for (i=0;i<len_query;i++){
        dbsubstr += v[(int)substring[i]];
    }

    unsigned char key_bin;
    int n = len_query;
    int found_pos = 0;
    while (dbsubstr not_eq query){
        fread(&key_bin,1,1,fseq);
        if ((int)key_bin == EOF){
            cout << "Sequence not found" << endl;
            break;
        }
        // n = insertSorted(substring,n,key_bin,n+1);
        // n = deleteFirst(substring,n);
        dbsubstr += v[(int)key_bin];
        dbsubstr.erase(0,1);
        found_pos ++;
    }

    cout << "Sequence found !! At pos : " << found_pos << endl ; 

    // while ((h = getc(fseq)) != EOF)
    //     fread()
    //     if (dbchar == query[1])



    //     dbsubstr[query.length()] = 
    //     for (i=0;i<query.length();i++){
    //         for i = 
    //             find(database(i))
    //     }
        
        // while (fgets(line,MAX_LEN,fquery) != NULL){
        //     cout << line << endl;
        // }
    // 
    // int i;
    // for (i=0;i<10;i++){
    //     fread (&a[i],1,1,fseq);
    // }

        
       /*
        fread(a,1,10,fseq);
        fclose (fseq);
        */

    fclose(fin);
    fclose(fseq);
    fclose(fquery);
    return 0;
}
