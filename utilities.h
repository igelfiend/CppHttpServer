#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>

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

bool dirExists( std::string &path )
{
    struct stat info;

    if(    ( stat( path.c_str(), &info ) == 0 )
        && ( info.st_mode & S_IFDIR ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

}

#endif // UTILITIES_H
