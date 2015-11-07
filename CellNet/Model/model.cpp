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
    float rastrSize = 20.0;
    map = ReliefGenerator::generate((ReliefGenerator::Mode)mode);
    calculator->setMap(map, rastrSize);
    for(size_t i = 0; i < stations.size(); i++)
        stations.at(i)->mask = calculator->calculate(stations.at(i), radius);
    return map;
}

void Model::loadStations()
{
    std::vector<StationDescr> descrs = Printer::instance()->loadStations();
    for(size_t i = 0; i < descrs.size(); i++)
    {
        StationDescr d = descrs.at(i);
        addStationPlace(d.x, d.y, d.h);
    }
}

void Model::clear()
{
    map.clear();
}

void Model::startAlgorihtm(int population_count, int crossing, int generations_count, QProgressDialog *dlg) const
{
    if(stations.size() == 0)
        return;

    GeneticAlgorhitm algo(stations.size(), population_count, crossing);

    for(int generation_num = 0; generation_num < generations_count; generation_num++)
    {
        if(dlg->wasCanceled())
            return;

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

        Printer::instance()->addText(" Average cov for station: ", algo.getAverageCoverage());

        algo.process();
        dlg->setValue(generation_num);
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
