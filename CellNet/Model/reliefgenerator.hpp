#ifndef RELIEFGENERATOR_HPP
#define RELIEFGENERATOR_HPP

#include <vector>

class ReliefGenerator
{
    static int width;
    static int height;

    static int sinusFunction(int i, int j);
    static int linearFunction(int i, int j);
    static int moduleFunction(int i, int j);
    static int blockFunction(int i, int j);

    static std::vector<std::vector<int> > generateDefault();
    static std::vector<std::vector<int> > generateFunction(int (*highFunction)(int i, int j));

    static std::vector<std::vector<int> > generateSinus();
    static std::vector<std::vector<int> > generateLinear();
    static std::vector<std::vector<int> > generateModule();
    static std::vector<std::vector<int> > generateBlock();

public:
    enum Mode
    {
        DEFAULT,
        SINUS,
        LINEAR,
        MODULE,
        BLOCK
    };

    static std::vector<std::vector<int> > generate(Mode mode);
};

#endif // RELIEFGENERATOR_HPP
