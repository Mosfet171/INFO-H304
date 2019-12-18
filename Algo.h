#ifndef ALGO_H_INCLUDED
#define ALGO_H_INCLUDED

#include <string>
#include <algorithm>
#include "Blosum.h"

class Algo
{
    public:
        Algo(std::string *query, std::string *target, Blosum *blosum);
        Algo(Algo const& clone);
        ~Algo();

        void constructionMatrice();
        void valeursMatrice();
        int bestChoice(int a, int b);
        int scoreCalcul(int p, int q);
        int score();
        int getMax() const;

    private:
        int **m_matrice;
        //int **m_matriceMemory;
        int m_p;
        int m_q;
        int m_max;
        std::string *m_query;
        std::string *m_target;
        Blosum *m_blosum;
        //int m_count;
};


#endif // ALGO_H_INCLUDED
