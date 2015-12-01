#include "model.hpp"

#include <iostream>

#include "coveragecalculator.hpp"
#include "reliefgenerator.hpp"
#include "genetic.hpp"
#include "printer.hpp"

Model::Model(float radius)
{
    float rastrSize = 20.f;
    calculator = new CoverageCalculator(radius, rastrSize);
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
    if(mode == ReliefGenerator::FILE)
        return Matrix();

    map.clear();
    map = ReliefGenerator::generate((ReliefGenerator::Mode)mode);
    calculator->calcMask(map);
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
    calculator->load(stations);
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
            algo.setSurvivalFactor(sub, calculator->getCoverageSize(population.at(sub)));
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

CoverageMask Model::calcCoverageMask(const Genom &gen) const
{
    return calculator->getCoverageMask(gen);
}

std::vector<Station *> Model::getStations() const
{
    return stations;
}
