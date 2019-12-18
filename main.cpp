#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <chrono>
#include "file.h"
#include "Blosum.h"
#include "Algo.h"
#include "Phr.h"
#include "psq.h"
#include "Pin.h"
#include <vector>

using namespace std;
int main(int argc, char** argv)
{

    unsigned char buffer[4];
    char sob = sizeof(buffer);

    string pinString = string(argv[2]) + string(".pin");
    Pin pin(pinString.c_str());//string("uniprot_sprot.fasta")+string(".pin"));
    //pin.dataData();
    FILE * fquery;
     // Extracting query sequence
    string queryname = argv[1];//"testproty.fasta"; //"P00533.fasta";
    //string queryname = "P00533.fasta";
    cout << "--------------- QUERY INFO ---------------" << endl
        << "Query file name : " << queryname.c_str() << endl;
    fquery = fopen(queryname.c_str(),"r");
    if (fquery == NULL){
        cout << "Opening of 'query' file impossible." << endl;
        exit(1);
    }
    if (queryname.substr(queryname.length()-5,5) == "fasta")
    {
        if ((fgetc(fquery) == '>'))
        {
            char querytitle[1000];
            fscanf(fquery,"%[^\n]",querytitle);
            cout << "Query descritpion : " << querytitle << endl;
        } else
        {
            cout << "Fake or corrupted FASTA file ..." << endl;
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
    ///////////////////////////////////////////////////
    Blosum *blosum;
    string blosumString;
    if(argc == 4)
    {
        blosumString = argv[3];
    }
    else
    {
        blosumString = "BLOSUM62";
    }
    blosum = new Blosum(blosumString.c_str());

    string psqString = string(argv[2]) + string(".psq");
    Psq psq(psqString.c_str()); //"uniprot_sprot.fasta.psq"

    bool parcours = true;
    string dbsubstr;

    int proviMax = 0;
    vector<int> ranking(5,0);
    vector<int> tracking(5,0);

    int *found_pos(0);
    found_pos = new int;
    *found_pos = 0;


    auto start = std::chrono::high_resolution_clock::now();
    int nbr = 0;

    while(parcours)
    {
        dbsubstr = "";
        dbsubstr = psq.algo(found_pos, &parcours);
        //cout << dbsubstr << endl;
        Algo *algo;
        algo = new Algo(&query, &dbsubstr, blosum);
        nbr++;

        proviMax = (*algo).getMax();

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
                tracking[k] = *found_pos;
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
    delete found_pos;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "Sequence found in database in " << duration.count() << " miliseconds" << endl;

    for(int k = 0; k < 5; k++)
    {
        cout << k+1 << ") "  << ranking[k] << "    " << "position : " << tracking[k] << endl;
    }

    // Getting the position of the beggining of the Header offset table
    long int headerTableInitPos = pin.tell();
    cout << headerTableInitPos << endl;
    //int endHeaderString;
    int lenHeaderString;
    int *startHeaderString(0);
    startHeaderString = new int;
    pin.seek();
    long int sequenceTableInitPos = pin.tell();
    cout << sequenceTableInitPos << endl;

    //int sequenceOffsetValue = 0;
    //int sequenceIndex = 0;

    string phrString= string(argv[2]) + string(".phr");
    Phr phr(phrString.c_str());

    string nameProt;
    for(int k = 0; k < 5; k++)
    {
        lenHeaderString = pin.calculData(buffer, sob, headerTableInitPos, tracking[k], startHeaderString);
        nameProt = phr.afficheNom(lenHeaderString, startHeaderString);

        cout << nameProt << endl;
        cout << "              " << ranking[k] << endl;

    }

}
