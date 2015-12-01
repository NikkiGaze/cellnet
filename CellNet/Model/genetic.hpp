#ifndef GENETIC_HPP
#define GENETIC_HPP

#include <cstdlib>
#include <vector>
#include <string>

class Genom
{
    unsigned int mask;
    unsigned int size;

public:
    Genom(unsigned int _size = 32);
    void randomGenerate();
    void activate(unsigned int bit, bool activate = true);
    void mutate(unsigned int count = 1);

    int bitSum() const;
    int genom() const;
    bool contains(unsigned int bit) const;

    std::string toString(bool just_gen = false) const;

    static std::pair<Genom, Genom> cross(const Genom &p1, const Genom &p2);

    unsigned int survival;
};

class GeneticAlgorhitm
{
    std::vector<Genom> currentPopulation;
    unsigned int populaionCount;
    int crossing;

    void selection();

public:
    GeneticAlgorhitm(unsigned int genom_size, unsigned int _populaion, unsigned int _crossing);
    void process();
    void setSurvivalFactor(unsigned int gen_num, unsigned int factor);
    std::vector<Genom> getPopulation() const;
    float getAverageCoverage() const;
};

#endif // GENETIC_HPP
