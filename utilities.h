#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <istream>

namespace utilities
{

unsigned int fileRead(std::istream & is, std::vector<char> & buff);

unsigned int countLines(const std::vector<char>& buff, int sz);

};

#endif // UTILITIES_H
