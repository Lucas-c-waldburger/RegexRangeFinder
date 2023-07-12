
#ifndef REGEXRANGEFINDER_FILEREADER_H
#define REGEXRANGEFINDER_FILEREADER_H

#include <iostream>
#include <fstream>

struct FileReader
{
public:
    FileReader() = default;

    static std::string readFromTxt(const std::string& pathName);

    static std::string readFromCsv(const std::string& pathName, int dataColumn);

};


#endif //REGEXRANGEFINDER_FILEREADER_H
