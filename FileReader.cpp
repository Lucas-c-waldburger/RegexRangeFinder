
#include "FileReader.h"

std::string FileReader::readFromTxt(const std::string& pathName)
{
    std::string subject;
    std::fstream fin(pathName);

    if (fin.is_open())
    {
        std::string temp;
        while (fin >> temp)
            subject += temp + ' ';
        fin.close();
    }

    return subject;
}

std::string FileReader::readFromCsv(const std::string& pathName, int dataColumn)
{
    std::string subject;
    std::fstream fin(pathName);

    if (fin.is_open())
    {
        std::string temp;
        while (fin >> temp)
        {
            auto currentPos = std::find(temp.begin(), temp.end(), ',');

            for (int i = 1; i < dataColumn; i++)
                currentPos = std::find(currentPos, temp.end(), ',');

            temp = *(++currentPos);

            while (std::isdigit(*(++currentPos)))
                temp += *currentPos;

            subject += temp + ' ';
        }
        fin.close();
    }

    return subject;
}
