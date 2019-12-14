/*
 La classe fonctionne. Il reste à définir la fonction score
*/

#ifndef BLOSUM_H_INCLUDED
#define BLOSUM_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

class Blosum
{
    public:
    Blosum(std::string fileName); //constructeur
    Blosum(Blosum const& original); // constructeur par copie
    ~Blosum(); //destructeur

    int score(char a, char b) const;
    void init(std::string fileName);
    int preScore(char c) const; //changer ce nom

    private:
    int m_matrice[24][24];
    std::string m_fileName;

};


#endif // BLOSUM_H_INCLUDED
