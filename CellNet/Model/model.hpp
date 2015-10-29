#ifndef MODEL_HPP
#define MODEL_HPP

#include <set>
#include <vector>

class CoverageCalculator;
class MapScene;
class Genom;

struct Station;

typedef std::set<std::pair<int, int> > CoverageMask;
typedef std::vector<std::vector<int> > Matrix;

class Model
{
    CoverageCalculator *calculator;

    std::vector<Station *> stations;
    Matrix map;
    float radius;

public:
    Model(float _radius);
    ~Model();
    Matrix generateRelief(int mode);
    void loadStations();
    void clear();
    void runDemo();
    void startAlgorihtm(int population_count, int crossing, int generations_count) const;

    CoverageMask calcCoverage(const Genom &gen) const;
    std::vector<Station *> getStations() const;

private:
    void addStationPlace(int _x, int _y, int _h);
};

#endif // MODEL_HPP
