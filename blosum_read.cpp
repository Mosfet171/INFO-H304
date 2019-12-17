#include "blosum_read.h"

using namespace std;
void blosumRead()
{
    string const fileName("BLOSUM62");
    ifstream file(fileName.c_str());
    string line("");
    //char a('0');
    int const m(24);
    int const n(24);

    int position(0);
    int pre_position(0);
    int delta = 0;

    int temp(0);
    int p(0);
    int q(0);
    bool neg = false;

    int matrice[n][n]; //version statique

    /*int **matrice = new int*[24];
    for(int i = 0; i < m; i++)
    {
        matrice[i] = new int[n];
    }*/
    /*for (int i = 0; i < m; i++)
    {
         delete[] matrice[i];
    }
    delete[] matrice;*/ //suppression du tableau

    if(file)
    {
        while(getline(file, line))
        {
            position = file.tellg();
            if(line[0] != '#')
            {
                q = 0;
                delta = position - pre_position;
                for(int k = 0; k < delta; k++)
                {
                    if(isdigit(line[k]))
                    {
                        temp = (int)line[k];
                        temp -= 48;
                        if(neg)
                        {
                            temp *= -1;
                        }
                        matrice[p][q] = temp;
                        cout << "(p,q) = " << "(" << p << "," << q << ")" << "    "<<matrice[p][q] << endl;
                        neg = false;
                        q++;
                    }
                    else if(line[k] == '-')
                    {
                        neg = true;
                    }
                    else
                    {
                        neg = false;
                    }
                }
                pre_position = file.tellg();
                p++;
            }
            else
            {
                p = 0;
            }
            pre_position = position;
        }
    }
}
