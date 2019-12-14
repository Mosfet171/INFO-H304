/* Preliminary Version of the Protein Sequence Alignment Project
for the INF0-H304 course, by Alissa Komino, Jean-Charles Nsangolo and Arkady Mospan */

#include <iostream>
#include <cstdio>
#include <string>
#include <chrono>
#include "protlib.hh"
#include "Blosum.h"
#include <algorithm>
#include "algo.h"

using namespace std;

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
    unsigned char buffer[4];
    char sob = sizeof(buffer);

    cout<< "*---                                 PROTY v0.5                                   ---*" << endl
        << "*---             Sequence alignment program for peptides or proteins              ---*" << endl
        << "*---                         v0.x : Exact sequence search                         ---*" << endl
        << "*---              v1.x : Search using the Smith-Waterman algorithm                ---*" << endl
        << "*--- For the course 'INFO-H304 - Compléments de programmation et d'algorithmique' ---*" << endl
        << "*---          By Alissa Komino, Jean-Charles Nsangolo and Arkady Mospan.          ---*" << endl
        << endl;

    // ------------------------------ DATABASE INFO ------------------------------ //
    // --------------------------------------------------------------------------- //

    string indexname = argv[1] +string(".pin");//string("uniprot_sprot.fasta")+string(".pin");
    fin = fopen(indexname.c_str(),"rb");
    if (fin == NULL){
        cout << "Opening of 'index' file impossible." << endl;
        exit(1);
    }

    unsigned int version = readIndexFile(buffer,sob,fin); // Version
    unsigned int intdbtype = readIndexFile(buffer,sob,fin); // Type (DNA or Protein)
    string dbtype;
    if (intdbtype == 1) {
        dbtype = "Protein";
    } else {
        dbtype = "DNA";
    }
    unsigned int len_title = readIndexFile(buffer,sob,fin); // Length 'T' of title
    char titlebuffer[len_title];
    fread(&titlebuffer,1,len_title,fin);
    string title = charToString(titlebuffer,len_title); // Title

    unsigned int len_timestamp = readIndexFile(buffer,sob,fin); // Length 'S' of timestamp
    char timestampbuffer[len_timestamp];
    fread(&timestampbuffer,1,len_timestamp,fin);
    string timestamp = charToString(timestampbuffer,len_timestamp); // Timestamp

    unsigned int num_seq = readIndexFile(buffer,sob,fin); // Number of sequences

    unsigned long long int num_residue; // Using long long because dealing with a 8 byte int here (and definitely positive hence unsigned)
    fread(&num_residue,1,8,fin); // Number of residues

    unsigned int len_maxseq = readIndexFile(buffer,sob,fin); // Length of longest sequence

    // ****************************** OUTPUT PART ****************************** //
        cout << endl
        << "--------------- DATABASE INFO ---------------" << endl
        << "Version : " << version << endl
        << "DB type : " << dbtype << endl
        << "DB title : " << title << endl
        << "Timestamp : " << timestamp << endl
        << "Number of sequences : " << num_seq << endl
        << "Number of residues : " << num_residue << endl
        << "Length of maximum sequence : " << len_maxseq << " residues" << endl
        << endl;




    // ----------------------------- QUERY INFO --------------------------------- //
    // -------------------------------------------------------------------------- //

    // Extracting query sequence
    string queryname = argv[2]; //"testproty.fasta"; //"P00533.fasta"
    //string queryname = "P00533.fasta";
    cout << "--------------- QUERY INFO ---------------" << endl
        << "Query file name : " << queryname.c_str() << endl;
    fquery = fopen(queryname.c_str(),"r");
    if (fquery == NULL){
        cout << "Opening of 'query' file impossible." << endl;
        exit(1);
    }
    if (queryname.substr(queryname.length()-5,5) == "fasta"){
        if ((fgetc(fquery) == '>')){
            char querytitle[1000];
            fscanf(fquery,"%[^\n]",querytitle);
            cout << "Query descritpion : " << querytitle << endl;
        } else {
            cout << "Fake or corrupted FASTA file ... Sneaky Raccoon" << endl;
        }
    }
    cout << endl;

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
    // ------------------------------------------------------------------------- //

    /* This part can obviously be done in another way, as transforming everything to a string from the std::string
    package is maybe not the best option memory-ly speaking; however, at least for the preliminary project, it
    is simpler to use and verify.
    */

    // Reading from database sequence file (.psq) an array of char of same length as the query
    string sequencename =  argv[1] + string(".psq");//string("wesh") + string(".psq");
    fsq = fopen(sequencename.c_str(),"rb");
    if (fsq == NULL){
        cout << "Opening of 'sequence' file impossible." << endl;
        exit(1);
    }
    cout << "--------------- SEEKING MATCHES ---------------" << endl;
    unsigned char substring[len_query];
    fread(&substring,1,len_query,fsq);

    // Transforming the array of char into a continued string
    string dbsubstr;
    for (i=1;i<len_query;i++){ //i = 1 pour éviter de mettre un -
        dbsubstr += v[(int)substring[i]];
    }

    Blosum *blosum;
    blosum = new Blosum("BLOSUM62");

    auto start = std::chrono::high_resolution_clock::now();
    // Comparison code between query and string in database
    unsigned char key_bin;
    int found_pos = 0;
    /*while (dbsubstr not_eq query){
        fread(&key_bin,1,1,fsq);
        if ((int)key_bin == EOF){
            cout << "Sequence not found !" << endl;
            break;
            exit(1);
        }
        dbsubstr += v[(int)key_bin];
        cout << "1" << endl;
        cout << dbsubstr << endl;
        dbsubstr.erase(0,1);
        cout << "2" << endl;
        cout << dbsubstr << endl;
        found_pos ++;
    }*/
    int proviMax = 0;
    int realMax = 0;
    vector<int> ranking(5,0);
    vector<int> tracking(5,0);
    while(fread(&key_bin,1,1,fsq) != 0)
    {
        if((int)key_bin != 0)
        {
            if(v[(int)key_bin] != '\n' && v[(int)key_bin] != '\r')
            {
            dbsubstr += v[(int)key_bin];
            }
            //cout << "taille de chaine : " << dbsubstr.size() << endl;
            //cout << dbsubstr << endl;
            //cout << (int)key_bin << endl;

        }
        else
        {
            //dbsubstr.erase(0,1);
            //cout << "taille de query : " << len_query << " et " << query.size() << endl;
            //cout << query << endl;
            //cout << "taille de chaine : " << dbsubstr.size() << endl;
            //cout << dbsubstr << endl;
            //dbsubstr = "";

            Algo *algo;
            algo = new Algo(&query, &dbsubstr, blosum);
            proviMax = (*algo).score();
            delete algo;
            dbsubstr = "";

            int tempScore;
            int tempPos;
            int tempo;
            for(int k = 0; k < 5; k++)
            {
                if(proviMax > ranking[k])
                {
                    tempScore = ranking[k];
                    ranking[k] = proviMax;

                    tempPos = tracking[k];
                    tracking[k] = found_pos;
                    for(int m = k + 1; m < 5; m++)
                    {
                        proviMax = ranking[m];
                        ranking[m] = tempScore;
                        tempScore = proviMax;

                        tempo = tracking[m];
                        tracking[m] = tempPos;
                        tempPos = tempo;
                    }
                    break;
                }
            }
        }
        found_pos ++;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "Sequence found in database in " << duration.count() << " miliseconds" << endl;

    cout << "Real max = " << realMax << endl;
    for(int k = 0; k < 5; k++)
    {
        cout << k+1 << ") "  << ranking[k] << "    " << "position : " << tracking[k] << endl;
    }
    // Getting the position of the beggining of the Header offset table
    long int headerTableInitPos = ftell(fin);

    // Getting the position of the beggining of the Sequence offset table
    fseek(fin,(num_seq+1)*4,SEEK_CUR); // The header table is num_seq + 1 long, and each "seq" takes 4 bytes long, hence num_seq*4+1
    long int sequenceTableInitPos = ftell(fin);

    // Calculating where in the database the sequence is found (which index)
    int sequenceOffsetValue = 0;
    int sequenceIndex = 0;
    while (sequenceOffsetValue < found_pos){
        fread(&buffer,1,sob,fin);
        sequenceOffsetValue = bigToLittle(buffer,sob);
        sequenceIndex++;
    }

    sequenceIndex -- ; // To be consistent : we read then ++ then check, so the good one is one less than the last index returned

    // Seeking in the header table the length of the header
    fseek(fin,headerTableInitPos+4*(sequenceIndex),SEEK_SET);
    fread(&buffer,1,sob,fin);
    int startHeaderString = bigToLittle(buffer,sob);
    fread(&buffer,1,sob,fin);
    int endHeaderString = bigToLittle(buffer,sob);
    int lenHeaderString = endHeaderString - startHeaderString;

    // Reading the header string in the header file (.phr)
    string headername = argv[1] + string(".phr"); //string("uniprot_sprot.fasta") + string(".phr");
    fhr = fopen(headername.c_str(),"rb");
    if (fhr == NULL){
        cout << "Opening of 'header' file impossible." << endl;
        exit(1);
    }
    char headerStringBuffer[lenHeaderString];
    fseek(fhr,startHeaderString,SEEK_SET);
    fread(&headerStringBuffer,1,lenHeaderString,fhr);

    // Extracting the title (assuming it is the first visible string) from the header binary file
    // See NCBI BLAST Database format for more information
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

    cout << "Name of the first exact match : " << headerTitle << endl
        << endl;



    // Closing
    fclose(fin);
    fclose(fsq);
    fclose(fquery);
    fclose(fhr);
    return 0;
}
