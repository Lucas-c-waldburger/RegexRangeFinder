
#include <iostream>
#include "RegAssist.h"
#include "FileReader.h"

int main() {

    // DEMO USING TEXT FILE OF NUMBERS
    std::string txtFileSubject;

    txtFileSubject = FileReader::readFromTxt("SampleNumberFiles/PosAndNeg.txt");

    RegAssist rgx(txtFileSubject);

    std::vector<std::string> txtFileMatches = rgx.rangeMatch(-77, 124); // ENTER SEARCH BOUNDS

    std::cout << "TEXT FILE MATCHES:\n";
    for (auto& match : txtFileMatches)
        std::cout << match << '\n';
    // ----------------------------- //


    // DEMO USING CSV FILE WITH A COLUMN OF NUMBERS
    std::string csvFileSubject;

    csvFileSubject = FileReader::readFromCsv("SampleNumberFiles/PosAndNegWithNames.csv", 2);

    rgx = RegAssist(csvFileSubject);

    std::vector<std::string> csvFileMatches = rgx.rangeMatch(-88, 309); // ENTER SEARCH BOUNDS

    std::cout << "\n\nCSV FILE MATCHES:\n";
    for (auto& match : csvFileMatches)
        std::cout << match << '\n';
    // ----------------------------- //



    return 0;
}
