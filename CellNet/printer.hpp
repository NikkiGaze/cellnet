#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <string>
#include <vector>

struct StationDescr
{
    StationDescr()
    {
        x = 0;
        y = 0;
        h = 30;
    }

    int x;
    int y;
    int h;
};

struct SubDescr
{
    int survival;
    int size;
};

class Printer
{
    std::vector<std::string> text;
    std::string inFileName;
    std::string outFileName;
    Printer() {}

public:
    std::vector<StationDescr> loadStations() const;
    void addText(const std::string &str, int val = -1);
    void print();

    static Printer * instance();
};

#endif // PRINTER_HPP
