#include "model.hpp"

#include <iostream>

#include "coveragecalculator.hpp"
#include "reliefgenerator.hpp"
#include "genetic.hpp"
#include "printer.hpp"

Model::Model(float _radius)
{
    radius = _radius;
    calculator = new CoverageCalculator;
}

Model::~Model()
{
    delete calculator;
    for(size_t i = 0; i < stations.size(); i++)
    {
        delete stations.at(i);
    }
}

void Model::addStationPlace(int _x, int _y, int _h)
{
    Station * station = new Station(_x, _y, _h);
    stations.push_back(station);
}

Matrix Model::generateRelief(int mode)
{
    float rastrSize = 20;
    map = ReliefGenerator::generate((ReliefGenerator::Mode)mode);
    calculator->setMap(map, rastrSize);
    for(size_t i = 0; i < stations.size(); i++)
        stations.at(i)->mask = calculator->calculate(stations.at(i), radius);
    return map;
}

void Model::loadStations()
{
    std::vector<StationDescr> descrs = Printer::instance()->loadStations();
    for(int i = 0; i < descrs.size(); i++)
    {
        StationDescr d = descrs.at(i);
        addStationPlace(d.x, d.y, d.h);
    }

//    int x = 10 * i;
//    int y = 25 + 20 * (i % 2);
//    int h = 10 + 5 * (i % 3);
//    for(int i = 1; i <= 30; i++)
//    {
//        int x = 5 * i;
//        int y = 65 + 20 * (i % 2);
//        int h = 10 + 5 * (i % 3);
//        addStationPlace(x, y, h);
//    }
//    addStationPlace(10, 25, 10);
//    addStationPlace(15, 45, 15);
//    addStationPlace(20, 25, 10);
//    addStationPlace(25, 45, 20);
//    addStationPlace(30, 25, 10);
//    addStationPlace(35, 45, 15);
//    addStationPlace(40, 25, 10);
//    addStationPlace(45, 45, 20);
//    addStationPlace(50, 25, 10);
//    addStationPlace(55, 45, 15);
//    addStationPlace(60, 25, 10);
//    addStationPlace(65, 45, 20);
//    addStationPlace(70, 25, 10);
//    addStationPlace(75, 45, 15);
//    addStationPlace(80, 25, 10);
//    addStationPlace(85, 45, 20);
//    addStationPlace(90, 25, 10);
//    addStationPlace(95, 45, 15);
//    addStationPlace(20, 25, 10);
//    addStationPlace(25, 45, 20);
//    addStationPlace(30, 25, 10);
//    addStationPlace(35, 45, 15);
//    addStationPlace(40, 25, 10);
//    addStationPlace(45, 45, 20);
//    addStationPlace(50, 25, 10);
//    addStationPlace(55, 45, 15);
//    addStationPlace(60, 25, 10);
//    addStationPlace(65, 45, 20);
//    addStationPlace(70, 25, 10);
//    addStationPlace(75, 45, 15);
//    addStationPlace(80, 25, 10);
//    addStationPlace(85, 45, 20);
}

void Model::clear()
{
    map.clear();
}

void Model::startAlgorihtm(int population_count, int crossing, int generations_count) const
{
    GeneticAlgorhitm algo(stations.size(), population_count, crossing);

    for(int generation_num = 0; generation_num < generations_count; generation_num++)
    {
        std::vector<Genom> population = algo.getPopulation();

        for(unsigned int sub = 0; sub < population.size(); sub++)
        {
            CoverageMask mask = calcCoverage(population.at(sub));
            algo.setSurvivalFactor(sub, mask.size());
        }

        population = algo.getPopulation();
        Printer::instance()->addText(" Generation #", generation_num);
        for(unsigned int sub = 0; sub < population.size(); sub++)
            Printer::instance()->addText(population.at(sub).toString());

        algo.process();
    }
    Printer::instance()->print();
}

CoverageMask Model::calcCoverage(const Genom &gen) const
{
    if(stations.size() == 0)
        return CoverageMask();

    CoverageMask mask;
    for(size_t stations_iter = 0; stations_iter < stations.size(); stations_iter++)
    {
        if(!gen.contains(stations_iter))
            continue;

        Station * station = stations.at(stations_iter);

        CoverageMask tmp_mask = station->mask;
        for(std::set<std::pair<int, int> >::iterator mask_iter = tmp_mask.begin(); mask_iter != tmp_mask.end(); ++mask_iter)
        {
            mask.insert(*mask_iter);
        }
    }
    return mask;
}

std::vector<Station *> Model::getStations() const
{
    return stations;
}
