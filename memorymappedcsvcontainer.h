#ifndef MEMORYMAPPEDCSVCONTAINER_H
#define MEMORYMAPPEDCSVCONTAINER_H

#include <string>

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
        SUCCESS
    };

    explicit MemoryMappedCsvContainer(const std::string& filename);
    MemoryMappedCsvContainer();
    ~MemoryMappedCsvContainer();

    ReturnCode  open(const std::string& filename);
    bool        close();

    std::size_t rowCnt() const;
    std::size_t colCnt() const;

    std::string row(std::size_t row) const;

    std::string data(std::size_t row, std::size_t col) const;

private:
    std::string filename_{};
    int         fd_{-1};
    std::size_t size_{0};
    char*       buffer_{nullptr};

    std::size_t rowCnt_{0};
    std::size_t colCnt_{0};

    std::size_t calcRowCnt() const;
    std::size_t calcColCnt() const;
};

#endif // MEMORYMAPPEDCSVCONTAINER_H
