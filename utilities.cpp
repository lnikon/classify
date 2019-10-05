#include "utilities.h"

unsigned int utilities::countLines(const std::vector<char> &buff, int sz) {
    int newlines = 0;
    const char* p = &buff[0];
    for (int i = 0; i < sz; i++)
    {
        if (p[i] == '\n')
        {
            newlines++;
        }
    }

    return newlines;
}

unsigned int utilities::fileRead(std::istream &is, std::vector<char> &buff) {
    is.read(&buff[0], buff.size());

    return is.gcount();
}
