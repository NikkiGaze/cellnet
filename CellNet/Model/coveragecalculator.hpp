#ifndef COVERAGECALCULATOR_HPP
#define COVERAGECALCULATOR_HPP

#include <vector>
#include <set>

typedef std::vector<std::pair<int, int> > CoverageMask;
typedef std::vector<std::vector<long int> > MaskedMap;

struct Genom;

struct Station
{
    Station(int _x, int _y, int _h) : x(_x), y (_y), h (_h)
    {
    }
    int x;
    int y;
    float h;
};

class CoverageCalculator
{
    float radius;
    float rastrSize;
    std::vector<Station *> stations;
    MaskedMap maskMap;

    std::vector<std::pair<int, int> > getPointsOnRay(int stationX, int stationY, int _x, int _y, int t) const;

public:
    CoverageCalculator(float _radius, float _rastrSize);
    void load(std::vector<Station *> _stations);
    void calcMask(const std::vector<std::vector<int> > &map);

    CoverageMask getCoverageMask(const Genom gen) const;
    int getCoverageSize(const Genom gen) const;
private:
    bool isPointVisible(int x, int y, const Station &station, const std::vector<std::vector<int> > &map) const;
};

#endif // COVERAGECALCULATOR_HPP
