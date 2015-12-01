#include "coveragecalculator.hpp"

#include <cmath>

#include "genetic.hpp"

CoverageCalculator::CoverageCalculator(float _radius, float _rastrSize) : radius(_radius), rastrSize(_rastrSize)
{
}

void CoverageCalculator::load(std::vector<Station*> _stations)
{
    stations = _stations;
}

void CoverageCalculator::calcMask(const std::vector<std::vector<int> > &map)
{
    maskMap.clear();
    for(size_t x = 0; x < map.size(); x++)
    {
        maskMap.push_back(std::vector<long int>());
        for(size_t y = 0; y < map.at(x).size(); y++)
            maskMap.at(x).push_back(0);
    }
    //HERE WILL BE GPU CODE
    for(size_t x = 0; x < maskMap.size(); x++)
    {
        for(size_t y = 0; y < maskMap.at(x).size(); y++)
        {
            int mask = 1;
            for(size_t i = 0; i < stations.size(); i++)
            {
                if(isPointVisible(x, y, *stations.at(i), map))
                {
                    maskMap.at(x).at(y) |= mask;
                }
                mask = mask << 1;
            }
        }
    }
}

std::vector<std::pair<int, int> > CoverageCalculator::getPointsOnRay(int stationX, int stationY, int _x, int _y, int t) const
{
    std::vector<std::pair<int, int> > res;
    float r_x = _x * rastrSize;
    float r_y = _y * rastrSize;
    float r_stationX = stationX * rastrSize;
    float r_stationY = stationY * rastrSize;
    float vectorX = (r_x - r_stationX) / t;
    float vectorY = (r_y - r_stationY) / t;
    for(int i = 0; i < t; i++)
    {
        float tmp_x = std::abs(r_stationX + vectorX * i) / rastrSize;
        float tmp_y = std::abs(r_stationY + vectorY * i) / rastrSize;
        std::pair<int, int> point(std::floor(tmp_x), std::floor(tmp_y));
        res.push_back(point);
    }

    return res;
}

CoverageMask CoverageCalculator::getCoverageMask(const Genom gen) const
{
    CoverageMask res;
    for(size_t x = 0; x < maskMap.size(); x++)
    {
        for(size_t y = 0; y < maskMap.at(x).size(); y++)
        {
            if(gen.genom() & maskMap.at(x).at(y))
                res.push_back(std::make_pair(x, y));
        }
    }
    return res;
}

int CoverageCalculator::getCoverageSize(const Genom gen) const
{
    int res = 0;
    for(size_t x = 0; x < maskMap.size(); x++)
    {
        for(size_t y = 0; y < maskMap.at(x).size(); y++)
        {
            if(gen.genom() & maskMap.at(x).at(y))
                res++;
        }
    }
    return res;
}

bool CoverageCalculator::isPointVisible(int x, int y, const Station &station, const std::vector<std::vector<int> > &map) const
{
    float dist = hypot(station.x - x, station.y - y);
    if(dist > radius)
        return false;

    int t = std::ceil(dist * sqrt(2.0));

    float stationHeight = station.h + map.at(station.x).at(station.y);
    std::vector<std::pair<int, int> > pointsOnRay = getPointsOnRay(station.x, station.y, x, y, t);
    float highVector = (map.at(x).at(y) - stationHeight) / t;

    bool isVisible = true;
    for(size_t t = 0; t < pointsOnRay.size(); t++)
    {
        int tmp_x = pointsOnRay.at(t).first;
        int tmp_y = pointsOnRay.at(t).second;
        float rayHeght = stationHeight + highVector * t;
        float realHeight = map.at(tmp_x).at(tmp_y);
        if(rayHeght < realHeight)
        {
            isVisible = false;
            break;
        }
    }

    return isVisible;
}
