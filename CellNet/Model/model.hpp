#ifndef MODEL_HPP
#define MODEL_HPP

#include <set>
#include <vector>


#include "QProgressDialog"

class CoverageCalculator;
class MapScene;
class Genom;

struct Station;

typedef std::vector<std::pair<int, int> > CoverageMask;
typedef std::vector<std::vector<int> > Matrix;

class Model
{
    CoverageCalculator *calculator;

    std::vector<Station *> stations;
    Matrix map;

public:
    Model(float radius);
    ~Model();
    Matrix generateRelief(int mode);
    void loadStations();
    void clear();
    void runDemo();
    void startAlgorihtm(int population_count, int crossing, int generations_count, QProgressDialog *dlg) const;

    CoverageMask calcCoverageMask(const Genom &gen) const;
    std::vector<Station *> getStations() const;

private:
    void addStationPlace(int _x, int _y, int _h);
};

#endif // MODEL_HPP
