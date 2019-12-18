#include "Algo.h"
#include "Blosum.h"


#define INDEL 10

using namespace std;

Algo::Algo(string *query, string *target, Blosum *blosum) : m_query(query), m_target(target), m_p((*query).size()), m_q((*target).size()), m_blosum(blosum), m_max(0)//, m_count(0)
{
    constructionMatrice(); //construction de la matrice de similarité
    valeursMatrice();
}

Algo::Algo(Algo const& clone) //constructeur par copie
{
}

Algo::~Algo() //destructeur
{
    for (int i = 0; i < m_p; i++)
    {
         delete[] m_matrice[i];
    }
    delete[] m_matrice;
}

//construit une matrice qui contiendra les scores de similartié et une autre qui contiendra les scores d'algo
void Algo::constructionMatrice()
{
    m_matrice = new int*[m_p];
    for(int i = 0; i < m_p; i++)
    {
        m_matrice[i] = new int[m_q];
    }
}

//remplissage de la matrice des scores
void Algo::valeursMatrice()
{
    for(int i = 0; i < m_p; i ++)
    {
        for(int j = 0; j < m_q; j++)
        {
            m_matrice[i][j] = bestChoice(i,j);
            m_max = max(m_max, m_matrice[i][j]);
        }
    }
}

//On remplit la matrice en fonction des scores des cases précédentes
//Si la case précédente est sur la diagonale ==> pas de pénalité
int Algo::bestChoice(int a, int b)
{
    int solution(0), solution1(0), solution2(0), solution3(0);
    if(a == 0 && b == 0)
    {
        solution1 = 0;
    }
    else if(a==0 && b != 0)
    {
        solution1 = (*m_blosum).score((*m_query)[a], (*m_target)[b]);
        solution2 = m_matrice[a][b-1] - INDEL;
    }
    else if(a != 0 && b == 0)
    {
        solution1 = (*m_blosum).score((*m_query)[a], (*m_target)[b]);
        solution2 = m_matrice[a-1][b] - INDEL;
    }
    else
    {
        solution1 = m_matrice[a-1][b-1] + (*m_blosum).score((*m_query)[a], (*m_target)[b]);
        solution2 = m_matrice[a-1][b] - INDEL;
        solution3 = m_matrice[a][b-1] - INDEL;
    }

    solution = max(max(0, solution1), max(solution2, solution3));
    //cout << solution << endl;
    return solution;
}

int Algo::getMax() const
{
    return m_max;
}
///////////////////////////////////////////////////////////////////////////////////////////
