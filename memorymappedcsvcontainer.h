#ifndef MEMORYMAPPEDCSVCONTAINER_H
#define MEMORYMAPPEDCSVCONTAINER_H

#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

struct MemoryMappedCsvContainer
{
    enum class ReturnCode
    {
        INVALID_DESCRIPTOR,
        INVALID_MMAP_ADDRESS,
        LSEEK_ERROR,
        SUCCESS
    };

    explicit MemoryMappedCsvContainer(const std::string& filename, char delim);
    MemoryMappedCsvContainer() = default;
    ~MemoryMappedCsvContainer();

    ReturnCode  open(const std::string& filename, char delim);
    bool        close();

    std::size_t rowCnt() const;
    std::size_t colCnt() const;

    std::string row(std::size_t row) const;

    void        data(std::vector<std::vector<std::string>>& dataCache) const;
    void        reset();

private:
    std::string filename_{};
    int         fd_{-1};
    long        size_{0};
    char*       buffer_{nullptr};
    char*       bufferOriginal_{nullptr};
    char        delim_{','};

    std::size_t rowCnt_{0};
    std::size_t colCnt_{0};

    std::size_t calcRowCnt() const;
    std::size_t calcColCnt() const;
};

#endif // MEMORYMAPPEDCSVCONTAINER_H
