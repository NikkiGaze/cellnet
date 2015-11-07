#include "printer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

template <typename _T>
static inline std::string to_str(const _T& val)
{
    std::stringstream strstr;
    strstr << val;
    return strstr.str();
}

Printer * Printer::instance()
{
    static Printer printer;
    return &printer;
}

std::vector<StationDescr> Printer::loadStations() const
{
    std::vector<StationDescr> res;
    std::ifstream file("config.ini");
    if(!file.is_open())
        return res;

    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);
        if(line.empty())
            break;
        std::istringstream ist(line);

        StationDescr descr;
        ist >> descr.x >> descr.y;
        descr.h = 30;
        res.push_back(descr);
    }
    file.close();
    return res;
}

void Printer::addText(const std::string &str, int val)
{
    std::string string = str;
    if(val != -1)
        string += to_str(val);
    text.push_back(string);
}

void Printer::print() const
{
    std::ofstream out("generations_out.txt");
    for(size_t i = 0; i < text.size(); i++)
        out << text.at(i) << std::endl;
    out.close();
}
