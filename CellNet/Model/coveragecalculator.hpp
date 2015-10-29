#ifndef COVERAGECALCULATOR_HPP
#define COVERAGECALCULATOR_HPP

#include <vector>
#include <set>

typedef std::set<std::pair<int, int> > CoverageMask;

struct Station
{
    Station(int _x, int _y, int _h)
    {
        x = _x;
        y = _y;
        h = _h;
    }

    int x;
    int y;
    float h;
    CoverageMask mask;
};

class CoverageCalculator
{
    float rastrSize;
    std::vector<std::vector<int> > map;

    std::vector<std::pair<int, int> > getPointsOnRay(int stationX, int stationY, int _x, int _y, int t) const;

public:
    CoverageCalculator();
    void setMap(std::vector<std::vector<int> > _map, float _rastrSize);
    std::set<std::pair<int, int> > calculate(Station *station, float radius) const;
};

#endif // COVERAGECALCULATOR_HPP
