
#include "RegAssist.h"

RegAssist::RegAssist(std::string subject) : subject(subject)
{}

void RegAssist::setQuery(std::string newQuery)
{
    this->query = newQuery;
}

std::string RegAssist::getQuery() const
{
    return query;
}

void RegAssist::setSubject(std::string newSubject)
{
    this->subject = newSubject;
}

std::string RegAssist::getSubject() const
{
    return subject;
}


std::string RegAssist::buildRangeExpression(std::string min, std::string max)
{
    RangeFormat format;
    std::string dCount;
    char tempMin, tempMax;
    std::string::reverse_iterator minIt, maxIt;

    std::string rangeExp = "\\b(";

    while (true)
    {
        // PULL OUT
        tempMin = min.back(); tempMax = max.back();
        min.pop_back(); max.pop_back();

        // CHECK MAX EMPTY
        if (max.empty() || min == max)
        {
            format(max, tempMin, tempMax);
            return rangeExp + format.join(dCount) + ")\\b";
        }

        // BUILD
        format(min, tempMin, '9');
        format(max, '0', tempMax);
        rangeExp += format.join(dCount) + '|';

        // CHECK MIN EMPTY
        if (min.empty())
            min = "0";

        // INC/DEC FOR NEXT LOOP
        minIt = min.rbegin(); maxIt = max.rbegin();
        increment(min, minIt); decrement(max, maxIt);

        // ++DCOUNT FOR NEXT LOOP
        dCount += "\\d";
    }
}

// returns joined expressions WITHOUT enclosing \b( )\b OR signPrefix "-"/"(-)?"
std::string RegAssist::buildRangeExpNegatives(std::string min, std::string max)
{
    RangeFormat format;
    std::string dCount;
    char tempMin, tempMax;
    std::string::reverse_iterator minIt, maxIt;

    std::string rangeExp;

    while (true)
    {
        // PULL OUT
        tempMin = min.back(); tempMax = max.back();
        min.pop_back(); max.pop_back();

        // CHECK MAX EMPTY
        if (max.empty() || min == max)
        {
            format(max, tempMin, tempMax);
            return rangeExp + format.join(dCount); // optional = true means we need to do another build
        }

        // BUILD
        format(min, tempMin, '9');
        format(max, '0', tempMax);
        rangeExp += format.join(dCount) + '|';

        // CHECK MIN EMPTY
        if (min.empty())
            min = "0";

        // INC/DEC FOR NEXT LOOP
        minIt = min.rbegin(); maxIt = max.rbegin();
        increment(min, minIt); decrement(max, maxIt);

        // ++DCOUNT FOR NEXT LOOP
        dCount += "\\d";
    }
}


// performs new match using supplied regexQuery argument
// matches against "this->subject" by default; Optional argument tempSubject performs match on a newly-supplied subject string instead. Does NOT update "this->subject"
// stores match inside log as new shared_ptr and returns a reference to the match
std::vector<std::string>& RegAssist::match(std::string regexQuery, std::string tempSubject)
{

    re = std::regex(regexQuery); // update regex with new query

    log.emplace_back(std::make_shared<std::vector<std::string>>()); // create new match vector inside log

    auto itBegin = std::sregex_iterator(tempSubject.begin(), tempSubject.end(), re);
    auto itEnd = std::sregex_iterator();
    std::smatch m;

    for (std::sregex_iterator i = itBegin; i != itEnd; i++) // populate new match vector
    {
        m = *i;
        log.back()->push_back(m.str());
    }

    return *(log.back().get()); // return new match vector as reference
}


std::vector<std::string>& RegAssist::operator[](int i)
{
    if (i > log.size() - 1)
        throw std::out_of_range("Index exceeds log size");

    return *(log[i].get());
}

void RegAssist::operator()(std::string subject)
{
    setSubject(subject);
}

void RegAssist::increment(std::string& numStr, std::string::reverse_iterator currentIt)
{
    if (*currentIt == '9')
    {
        *currentIt = '0';

        if (currentIt == numStr.rend() - 1)
        {
            numStr.insert(0, "1");
            return;
        }
        else
            increment(numStr, std::next(currentIt));
    }
    else
        *currentIt += 1;
}

void RegAssist::decrement(std::string& numStr, std::string::reverse_iterator currentIt)
{
    if (*currentIt == '0')
    {
        *currentIt = '9';

        if (currentIt == numStr.rend() - 1)
        {
            numStr.erase(0, 1);
            return;
        }
        else
            decrement(numStr, std::next(currentIt));
    }
    else
        *currentIt -= 1;
}

std::vector<std::string> &RegAssist::rangeMatch(int min, int max) {

    std::string minStr = std::to_string(abs(min)); // remove '-' from min/max strings
    std::string maxStr = std::to_string(abs(max));
    std::string nonHyphen = "[^-,a-zA-Z0-9]+";
    std::string rangeQuery;

    if (min < 0 )
    {
        if (max < 0) // CASE BOTH_NEGATIVE
            rangeQuery = nonHyphen + "-(" + buildRangeExpNegatives(maxStr, minStr) + ")\\b"; // build with max as arg[0] and min as arg[1] with a required '-'

        else
            rangeQuery = nonHyphen + "-(" + buildRangeExpNegatives("1", minStr) + ")\\b|" + nonHyphen + "(" + buildRangeExpNegatives("0", maxStr) + ")\\b";
    }
    else // CASE BOTH_POSITIVE
        rangeQuery = nonHyphen + '(' + buildRangeExpNegatives(minStr, maxStr) + ")\\b";

    return match(rangeQuery, subject);
}
