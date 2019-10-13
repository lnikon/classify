#include "memorymappedcsvcontainer.h"

MemoryMappedCsvContainer::MemoryMappedCsvContainer(const std::string &filename)
{
    if(open(filename) != ReturnCode::SUCCESS)
    {
        return;
    }
}

MemoryMappedCsvContainer::MemoryMappedCsvContainer()
{

}

MemoryMappedCsvContainer::~MemoryMappedCsvContainer()
{
    close();
}

MemoryMappedCsvContainer::ReturnCode MemoryMappedCsvContainer::open(const std::string &filename)
{
    fd_ = ::open(filename.c_str(), O_RDWR | O_LARGEFILE, 0600);
    if(fd_ == -1)
    {
        return ReturnCode::INVALID_DESCRIPTOR;
    }

    size_ = lseek(fd_, 0L, SEEK_END);
    lseek(fd_, 0L, SEEK_SET);

    void* addr = static_cast<char*>(mmap(nullptr, size_, PROT_READ, MAP_SHARED, fd_, 0));
    if(addr == (void *)-1)
    {
        return ReturnCode::INVALID_MMAP_ADDRESS;
    }

    buffer_ = static_cast<char *>(addr);

    rowCnt_ = calcRowCnt();
    colCnt_ = calcColCnt();

    return ReturnCode::SUCCESS;
}

bool MemoryMappedCsvContainer::close()
{
    if(buffer_)
    {
        munmap(buffer_, size_);
    }

    ::close(fd_);
    fd_ = -1;
    filename_ = "";
    size_ = 0;
    buffer_ = nullptr;

    return true;
}

std::size_t MemoryMappedCsvContainer::rowCnt() const
{
    return rowCnt_;
}

std::size_t MemoryMappedCsvContainer::colCnt() const
{
    return colCnt_;
}

std::string MemoryMappedCsvContainer::row(std::size_t row) const
{
    if(!buffer_)
    {
        return "";
    }

    char* localBuffer = buffer_;

    for(auto i = 0u; i < row; ++i)
    {
        while(*localBuffer != '\n') {}
    }

    std::string result;
    while(*localBuffer != '\n')
    {
        result += *localBuffer++;
    }

    return result;
}

std::string MemoryMappedCsvContainer::data(std::size_t row, std::size_t col) const
{
    char* bufferLocal = buffer_;

    for(int i = 0; i < row; ++i)
    {
        while(*bufferLocal++ != '\n') {}
    }

    for(int i = 0; i < col; ++i)
    {
        while(*bufferLocal++ != '\t') {}
    }

    std::string result;
    while(*bufferLocal != '\t')
    {
        result += *bufferLocal++;
    }

    return result;
}

std::size_t MemoryMappedCsvContainer::calcRowCnt() const
{
    if(!buffer_)
    {
        return 0;
    }

    std::size_t rows = 0;
    char* localBuffer = buffer_;
    std::size_t sz = 0;
    while(sz++ != size_)
    {
        rows += *localBuffer++ == '\n';
    }

    return rows;
}

std::size_t MemoryMappedCsvContainer::calcColCnt() const
{
    if(!buffer_)
    {
        return 0;
    }

    std::size_t cols = 0;
    char* localBuffer = buffer_;
    while(*localBuffer != '\n')
    {
        cols += *localBuffer++ == '\t';
    }

    return cols + 1;
}
