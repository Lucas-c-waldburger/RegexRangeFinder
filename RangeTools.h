
#ifndef REGEXRANGEFINDER_RANGETOOLS_H
#define REGEXRANGEFINDER_RANGETOOLS_H

#include <iostream>
#include <utility>
#include <vector>

struct RangeTools
{
    RangeTools(std::string& boundStr, char bracketFirst, char bracketSecond) :
            boundStr(boundStr), bracket(bracketFirst, bracketSecond) {};

    std::string& boundStr;
    std::pair<char, char> bracket;

    void bracketNext()
    {
        bracket.first = boundStr.back();
        boundStr.pop_back();
    }
};

struct RangeFormat
{
    std::vector<std::string> subExps;

    void addSubExp(std::string& numStr, char bracketLow, char bracketHigh)
    {
        subExps.push_back(numStr + '[' + bracketLow + '-' + bracketHigh + ']');
    }

    std::string join(std::string& dCount)
    {
        std::string joined = "(";

        for (auto& sub : subExps)
            joined += sub + '|';

        subExps.clear();

        joined.pop_back();
        joined += ')' + dCount;

        return joined;
    }

    void operator()(std::string& numStr, char bracketLow, char bracketHigh)
    {
        addSubExp(numStr, bracketLow, bracketHigh);
    }

};
#endif //REGEXRANGEFINDER_RANGETOOLS_H
