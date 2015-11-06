#include "genetic.hpp"

#include <string>
#include <sstream>
#include <map>

#include "printer.hpp"

template <typename _T>
static inline std::string to_str(const _T& val)
{
    std::stringstream strstr;
    strstr << val;
    return strstr.str();
}

Genom::Genom(unsigned int _size)
{
    size = _size;
    mask = 0;
    survival = 1;
}

int Genom::bitSum() const
{
    int res = 0;
    for(unsigned int i = 0; i < size; i++)
    {
        if(contains(i))
            res++;
    }
    return res;
}
bool Genom::contains(unsigned int bit) const
{
    int tmp = 1 << bit;
    return mask & tmp;
}

std::string Genom::toString(bool just_gen) const
{
    int counter = 0;
    std::string res;
    if(!just_gen)
        res += "[ Gen: ";
    for(unsigned int i = 0; i < size; i++)
    {
        if(contains(i))
        {
            res += "1";
            counter++;
        }
        else
        {
            res += "0";
        }
    }

    if(!just_gen)
        res += " Weight: " + to_str(survival) + " Size: " + to_str(counter) + "]";
    return res;
}

void Genom::randomGenerate()
{
    mask = 0;
    int tmp = 0;
    for(unsigned int i = 0; i < size; i++)
    {
        tmp |= (1 << i);
    }

    tmp++;
    mask |= std::rand() % tmp;
}

void Genom::activate(unsigned int bit, bool activate)
{
    int tmp = 1 << bit;
    if(activate)
        mask |= tmp;
    else
        mask &= ~tmp;
}

void Genom::mutate(unsigned int count)
{
    for(unsigned int i = 0; i < count; i++)
    {
        int num = std::rand() % size;
        if(contains(num))
            activate(num, false);
        else
            activate(num, true);
    }
}

std::pair<Genom, Genom> Genom::cross(const Genom &p1, const Genom &p2)
{

    if(p1.size != p2.size)
        return std::pair<Genom, Genom>();

    unsigned int size = p1.size;
    Genom child1(size);
    Genom child2(size);

    unsigned int crossingPoint = std::rand() % size;

    for(unsigned int i = 0; i <= crossingPoint; i++)
    {
        if(p1.contains(i))
            child1.activate(i);

        if(p2.contains(i))
            child2.activate(i);
    }

    for(unsigned int i = crossingPoint + 1; i < size; i++)
    {
        if(p2.contains(i))
            child1.activate(i);

        if(p1.contains(i))
            child2.activate(i);
    }
    return std::pair<Genom, Genom>(child1, child2);
}

//-------------------------------------------------------------------

typedef std::vector<Genom> Bin;

GeneticAlgorhitm::GeneticAlgorhitm(unsigned int genom_size, unsigned int _populaion, unsigned int _crossing)
{
    crossing = _crossing;
    populaionCount = _populaion;
    for(unsigned int i = 0; i < populaionCount; i++)
    {
        Genom gen(genom_size);
        gen.randomGenerate();
        currentPopulation.push_back(gen);
    }
}

void GeneticAlgorhitm::selection()
{
    std::map<int, Bin> bins;
    for(size_t i = 0; i < currentPopulation.size(); i++)
    {
        int size = currentPopulation.at(i).bitSum();
        bins[size].push_back(currentPopulation.at(i));
    }

    for(std::map<int, Bin>::const_iterator iter = bins.begin(); iter != bins.end(); ++iter)
    {
        int size = (*iter).first;
        Bin bin = (*iter).second;
        Printer::instance()->addText("Bases count: " + to_str(size) + " Species count: " + to_str(bin.size()));
    }
    int extraCount = currentPopulation.size() - populaionCount;

    for(int i = 0; i < extraCount; i++)
    {
        std::map<int, Bin>::iterator iter;
        do
        {
            iter = bins.begin();
            std::advance(iter, (rand() % bins.size()));
        }
        while ((*iter).second.empty());

        Bin &bin = (*iter).second;

        unsigned int minWeight = (*bin.begin()).survival;
        int min_num = 0;
        for(size_t i = 0; i < bin.size(); i++)
        {
            Genom gen = bin.at(i);
            if(gen.survival < minWeight)
            {
                minWeight = gen.survival;
                min_num = i;
            }
        }

//        Printer::instance()->addText("REMOVE. sub: " + bin.at(min_num).toString(true) + " size: " + to_str(bin.at(min_num).bitSum()));
        bin.erase(bin.begin() + min_num);

        iter++;
        if(iter == bins.end())
            iter = bins.begin();
    }

    currentPopulation.clear();
    for(std::map<int, Bin>::const_iterator iter = bins.begin(); iter != bins.end(); ++iter)
    {
        Bin bin = (*iter).second;
        for(size_t i = 0; i < bin.size(); i++)
        {
            currentPopulation.push_back(bin.at(i));
//            Printer::instance()->addText("SURVIVE. sub: " + bin.at(i).toString(true));
        }
    }
}

void GeneticAlgorhitm::process()
{
    selection();

    //crossing
    std::vector<Genom> newPopulation;
    for(int i = 0; i < crossing; i++)
    {
        int rnd1 = std::rand() % populaionCount;
        Genom parent1 = currentPopulation.at(rnd1);
        int rnd2 = std::rand() % populaionCount;
        Genom parent2 = currentPopulation.at(rnd2);
        std::pair<Genom, Genom> children = Genom::cross(parent1, parent2);
        newPopulation.push_back(children.first);
        newPopulation.push_back(children.second);

    }

    //mutate
    for(size_t i = 0; i < newPopulation.size(); i++)
        newPopulation.at(i).mutate();

    currentPopulation = newPopulation;
}

void GeneticAlgorhitm::setSurvivalFactor(unsigned int gen_num, unsigned int factor)
{
    currentPopulation.at(gen_num).survival = factor;
}

std::vector<Genom> GeneticAlgorhitm::getPopulation() const
{
    return currentPopulation;
}
