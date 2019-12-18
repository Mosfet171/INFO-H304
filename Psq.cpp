#include "Blosum.h"
#include "Algo.h"
#include "psq.h"
using namespace std;
Psq::Psq(string name) : File(name)
{
}

/*Psq::Psq(Psq const& clone)
{
}*/
Psq::~Psq()
{
}

string Psq::algo(int *found_pos, bool *parcours)
{
    //cout << "yo" << endl;
    char v[28] = {'-','A','B','C','D','E','F','G','H','I','K','L','M','N','P','Q','R','S','T','V','W','X','Y','Z','U','*','Q','J'};

    
    unsigned char key_bin;

    string dbsubstr = "";

    int j = 0;
    while(fread(&key_bin,1,1,m_file) != 0) //Tant qu'on a pas de - on continue d'extraire des "caractères"
    {
        if((int)key_bin != 0)
        {
            if(v[(int)key_bin] != '\n' && v[(int)key_bin] != '\r')
            {
                dbsubstr += v[(int)key_bin];
            }

        }
        else
        {
            if(j == 0)
            {
                cout << "yep" << endl;
            }
            else
            {
                return dbsubstr;
            }
        }
        (*found_pos) ++;
        j++;
    }
    cout << "ptn" << endl;
    *parcours = false;
    return dbsubstr;

}
