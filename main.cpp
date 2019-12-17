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
int main()
{

    unsigned char buffer[4];
    char sob = sizeof(buffer);

    Pin pin("uniprot_sprot.fasta.pin");//string("uniprot_sprot.fasta")+string(".pin"));
    pin.dataData();
    FILE * fquery;
     // Extracting query sequence
    string queryname = "testproty.fasta"; //"P00533.fasta";//argv[2]; //
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

    ///////////////////////////////////////////////////
    Blosum *blosum;
    blosum = new Blosum("BLOSUM62");

    Psq psq("wesh.psq"); //"uniprot_sprot.fasta.psq");
    //auto start = std::chrono::high_resolution_clock::now();

    bool parcours = true;
    string dbsubstr;

    int proviMax = 0;
    int realMax = 0;
    vector<int> ranking(5,0);
    vector<int> tracking(5,0);

    int *found_pos(0);
    //cout << "yo" << endl;
    found_pos = new int;
    *found_pos = 0;
    //cout << "yo" << endl;


    auto start = std::chrono::high_resolution_clock::now();

    while(parcours)
    {
        dbsubstr = "";
        dbsubstr = psq.algo(found_pos, &parcours);
        //cout << dbsubstr << endl;
        Algo *algo;
        algo = new Algo(&query, &dbsubstr, blosum);

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
    int endHeaderString;
    int lenHeaderString;
    pin.seek();
    long int sequenceTableInitPos = pin.tell();

    int sequenceOffsetValue = 0;
    int sequenceIndex = 0;

    Phr phr(string("uniprot_sprot.fasta") + string(".phr"));

    for(int k = 0; k < 5; k++)
    {
        while (sequenceOffsetValue < tracking[k])
        {
            pin.read(buffer, sob);
            sequenceOffsetValue = bigToLittle(buffer,sob);
            sequenceIndex++;
        }
        sequenceIndex -- ;

        pin.seek(headerTableInitPos, sequenceIndex);
        pin.read(buffer, sob);

        int startHeaderString = bigToLittle(buffer,sob);
        fread(&buffer,1,sob,fin);
        endHeaderString = bigToLittle(buffer,sob);
        lenHeaderString = endHeaderString - startHeaderString;

        char headerStringBuffer[lenHeaderString];
        phr.afficheNom(lenHeaderString, startHeaderString, &headerStringBuffer);

    }

}
