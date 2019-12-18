#include "Blosum.h"
#define TAILLE 24

using namespace std;

Blosum::Blosum(string fileName) : m_fileName(fileName)
{
    init(fileName);
}

Blosum::Blosum(Blosum const& original) : m_fileName(original.m_fileName)
{
}

Blosum::~Blosum()
{
}

int Blosum::score(char a, char b) const
{
    int p = preScore(a);
    int q = preScore(b);
    return m_matrice[p][q];
}

void Blosum::init(string fileName)
{
    string line("");

    int position(0);
    int pre_position(0);
    int delta = 0;

    int temp(0);
    int p(0);
    int q(0);
    bool neg = false;
    bool start(false);

    ifstream file(fileName.c_str());
    if(file)
    {
        while(getline(file, line) && p < TAILLE)
        {
            position = file.tellg();
            if(line[0] != '#') //on ignore les lignes commençant par #
            {
                q = 0;
                delta = position - pre_position;
                for(int k = 0; k < delta; k++)
                {
                    if(isdigit(line[k]))
                    {
                        start = true;
                        temp = (int)line[k];
                        temp -= 48;
                        if(neg)
                        {
                            temp *= -1;
                        }
                        m_matrice[p][q] = temp;
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
                if(start)
                {
                    p++;
                }
            }
            else
            {
                p = 0;
            }
            pre_position = position;
        }
    }
}

int Blosum::preScore(char c) const //donne le numéro de la ligne ou colonne ou se trouve la lettre envoyée
{
    int position = 23;
    switch(c)
    {
        case 'A':
            position = 0;
            break;
        case 'R' :
            position = 1;
            break;
        case 'N':
            position = 2;
            break;
        case 'D':
            position = 3;
            break;
        case 'C':
            position = 4;
            break;
        case 'Q':
            position = 5;
            break;
        case 'E':
            position = 6;
            break;
        case 'G':
            position = 7;
            break;
        case 'H':
            position = 8;
            break;
        case 'I':
            position = 9;
            break;
        case 'L' :
            position = 10;
            break;
        case 'K':
            position = 11;
            break;
        case 'M':
            position = 12;
            break;
        case 'F':
            position = 13;
            break;
        case 'P':
            position = 14;
            break;
        case 'S':
            position = 15;
            break;
        case 'T':
            position = 16;
            break;
        case 'W':
            position = 17;
            break;
        case 'Y':
            position = 18;
            break;
        case 'V' :
            position = 19;
            break;
        case 'B':
            position = 20;
            break;
        case 'Z':
            position = 21;
            break;
        case 'X':
            position = 22;
            break;
        case '*':
            position = 23;
            break;
        default:
            position = 23; //En cas de problème on suppose qu'on a *
            break;
    }
    return position;
}
