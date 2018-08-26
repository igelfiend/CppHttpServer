#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>


namespace Utilities
{
/**
 * @brief split - splitting string to vector of strings with separator
 * @param s - string for splitting
 * @param seperator - symbol, using for separating
 * @return vector of strings after separating
 */
std::vector<std::string> split(const std::string& s, char seperator)
{
   std::vector<std::string> output;

    std::string::size_type prev_pos = 0, pos = 0;

    while((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );

        output.push_back(substring);

        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word

    return output;
}
}

#endif // UTILITIES_H
