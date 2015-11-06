#include "reliefgenerator.hpp"

#include <stdlib.h>
#include <cmath>

inline int generateChange()
{
    return ((rand() % 11) - 5);
}

inline float sign(float x)
{
    if(x == 0)
        return 0;
    return std::abs(x) / x;
}

int ReliefGenerator::width = 3600;
int ReliefGenerator::height = 3600;

std::vector<std::vector<int> > ReliefGenerator::generate(Mode mode)
{
    switch (mode)
    {
    case DEFAULT:
        return generateDefault();
    case SINUS:
        return generateFunction(&sinusFunction);
    case LINEAR:
        return generateFunction(&linearFunction);
    case MODULE:
        return generateFunction(&moduleFunction);
    case BLOCK:
        return generateFunction(&blockFunction);
    default:
        return std::vector<std::vector<int> >();
    }
}

std::vector<std::vector<int> > ReliefGenerator::generateDefault()
{
    std::vector<std::vector<int> > res;
    const int startHeight = 140;

    int currHeight = startHeight;

    for(int i = 0; i < width; i++)
    {
        if(i != 0)
        {
            currHeight = res.at(i - 1).at(0) + generateChange();
        }
        res.push_back(std::vector<int>());
        for(int j = 0; j < height; j++)
        {
            if(i == 0 && j ==0)
            {
                res.at(i).push_back(startHeight);
                continue;
            }

            if(j != 0 )
            {
                if(i > 0)
                    currHeight = float((res.at(i).at(j - 1) + res.at(i - 1).at(j))) / 2.0 + generateChange();
                else
                    currHeight = res.at(0).at(j - 1) + generateChange();
            }

            if(currHeight > 255)
                currHeight = 255;

            if(currHeight < 0)
                currHeight = 0;

            res.at(i).push_back(currHeight);
        }
    }
    return res;
}

std::vector<std::vector<int> > ReliefGenerator::generateFunction(int(*highFunction)(int i, int j))
{
    std::vector<std::vector<int> > res;
    for(int i = 0; i < width; i++)
    {
        res.push_back(std::vector<int>());
        for(int j = 0; j < height; j++)
        {
            int currHeight = highFunction(i, j);
            res.at(i).push_back(currHeight);
        }
    }
    return res;
}

int ReliefGenerator::sinusFunction(int i, int j)
{
    return (std::sin(i) + std::sin(j)) * 15 + 130;
}

int ReliefGenerator::linearFunction(int i, int j)
{
    return (float(i + j) / float(width + height)) * 200 + 30;
}

int ReliefGenerator::moduleFunction(int i, int j)
{
    return (std::abs(float(i - j)) / float(width + height)) * 100 + 100;
}

int ReliefGenerator::blockFunction(int i, int j)
{
    return 20 * (sign(sin(i)) * sign(sin(j))) + 130;
}
