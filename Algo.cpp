#include "Algo.h"
#include "Blosum.h"

#define INDEL 6

using namespace std;

Algo::Algo(string *query, string *target, Blosum *blosum) : m_query(query), m_target(target), m_p((*query).size()), m_q((*target).size()), m_blosum(blosum)//, m_count(0), m_max(0)
{
    constructionMatrice(); //construction de la matrice de similarité
    valeursMatrice(0,0);
}

Algo::Algo(Algo const& clone) //constructeur par copie
{
}

Algo::~Algo() //destructeur
{
    for (int i = 0; i < m_p; i++)
    {
         delete[] m_matrice[i];
         delete[] m_matriceMemory[i];
    }
    delete[] m_matrice;
    delete[] m_matriceMemory;
}

//construit une matrice qui contiendra les scores de similartié et une autre qui contiendra les scores d'algo
void Algo::constructionMatrice()
{
    m_matrice = new int*[m_p];
    m_matriceMemory = new int*[m_p];
    for(int i = 0; i < m_p; i++)
    {
        m_matrice[i] = new int[m_q];
        m_matriceMemory[i] = new int[m_q];
    }
    for(int i = 0; i < m_p; i++)
    {
        for(int j = 0; j < m_q; j++)
        {
            m_matrice[i][j] = 0;
            m_matriceMemory[i][j] = -1; //-1 veut dire que la case (i,j) n'a pas encore été traitée
        }
    }
}

//remplissage de la matrice des scores
void Algo::valeursMatrice(int p, int q)
{
    int a(p), b(q);
    for(int i = p; i < m_p; i++)
    {
        m_matrice[i][q] = bestChoice(i, q);
    }
    for(int j = q + 1; j < m_q; j++)
    {
        m_matrice[p][j] = bestChoice(p, j);
    }
    if(p < m_p - 1)
    {
        a = p + 1;
    }
    if(q < m_q - 1)
    {
        b = q + 1;
    }
    if((a-p)+(b-q) != 0)
    {
        valeursMatrice(a, b);
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
    return solution;
}

int Algo::scoreCalcul(int p, int q)
{
    int sol1(0), sol2(0), sol3(0), sol(0);
    if(p >= m_p || q >= m_q)
    {
    }
    else
    {
            if(m_matriceMemory[p][q] != -1)
            {
                sol = m_matriceMemory[p][q];
            }
            else if(m_matrice[p][q] == 0)
            {
                m_matriceMemory[p][q] = 0;
            }
            else
            {
                sol1 = scoreCalcul(p+1, q+1);
                sol2 = scoreCalcul(p, q+1);
                sol3 = scoreCalcul(p+1, q);

                sol = max(sol1, max(sol2, sol3)) + m_matrice[p][q];
                m_matriceMemory[p][q] = sol;

                //m_max = max(sol, m_max);
            }
    }
    return sol;
}

//Applique l'algo de SW a chaque case du tableau
int Algo::score()
{
    int temp(0);
    int maxx(0);
    for(int i = 0; i < m_p; i++)
    {
        for(int j = 0; j < m_q; j++)
        {
            temp = scoreCalcul(i,j);
            maxx = max(temp, maxx);
        }
    }
    return maxx;
}



