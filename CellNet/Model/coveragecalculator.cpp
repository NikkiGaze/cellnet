#include "coveragecalculator.hpp"

#include <cmath>
#include <iostream>

inline float module(int x, int y)
{
    return sqrt(x * x + y * y);
}

CoverageCalculator::CoverageCalculator()
{

}

void CoverageCalculator::setMap(std::vector<std::vector<int> > _map, float _rastrSize)
{
    map = _map;
    rastrSize = _rastrSize;
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

    std::cout << std::endl;
    return res;
}

std::set<std::pair<int, int> > CoverageCalculator::calculate(Station * station, float radius) const
{
    CoverageMask res;
    int stationX = station->x;
    int stationY = station->y;
    float stationHeight = station->h + map.at(stationX).at(stationY);

    int startX = std::max((float)0, stationX - radius);
    int startY = std::max((float)0, stationY - radius);
    int endX = std::min((float)map.size() - 1, stationX + radius);
    int endY = std::min((float)map.at(0).size() - 1, stationY + radius);

    //1st: found rect with 2 * r
    for(int x = startX; x <= endX; x++)
        for(int y = startY; y <= endY; y++)
        {
            //2st: bound it with circle

            float dist = module(stationX - x, stationY - y);
            if(dist > radius)
                continue;

            //3st: check visibility
            int t = std::ceil(dist * sqrt(2.0));

            std::vector<std::pair<int, int> > pointsOnRay = getPointsOnRay(stationX, stationY, x, y, t);
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

            if(!isVisible)
                continue;

            res.insert(std::make_pair(x, y));
        }
    return res;
}
