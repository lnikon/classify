#include "memorymappedcsvcontainer.h"

MemoryMappedCsvContainer::MemoryMappedCsvContainer(const std::string &filename,
                                                   char delim)
{
    if(open(filename, delim) != ReturnCode::SUCCESS)
    {
        /*
         * Consider to throw exception
         */
        return;
    }
}

MemoryMappedCsvContainer::~MemoryMappedCsvContainer()
{
    /*
     * Unmaps memory mapped region,
     * closes opened file for memory mapping
     * and resets all the interal variables to their default values
     */
    close();
}

MemoryMappedCsvContainer::ReturnCode
MemoryMappedCsvContainer::open(const std::string &filename,
                               char delim)
{
    /*
     * Open file
     */
    fd_ = ::open(filename.c_str(), O_RDWR | O_LARGEFILE, 0600);
    if(fd_ == -1)
    {
        return ReturnCode::INVALID_DESCRIPTOR;
    }

    /*
     * Set delimeter
     */
    delim_ = delim;

    /*
     * Move cursor to the end and find out file size in bytes
     */
    size_ = lseek(fd_, 0L, SEEK_END);
    if(size_ < 0)
    {
        return ReturnCode::LSEEK_ERROR;
    }

    /*
     * Move cursor back to the begging
     */
    lseek(fd_, 0L, SEEK_SET);

    /*
     * Get pointer to the first byte in memory mapped region
     */
    void* addr = static_cast<char*>(mmap(nullptr, static_cast<size_t>(size_),
                                         PROT_READ,
                                         MAP_SHARED,
                                         fd_,
                                         0));
    if(addr == (void *)-1)
    {
        return ReturnCode::INVALID_MMAP_ADDRESS;
    }

    /*
     * Hold buffer to the first byte in memory mapped region
     */
    buffer_ = static_cast<char *>(addr);
    bufferOriginal_ = buffer_;

    /*
     * As row and column calculations are performance critical,
     * then we need to precarculate them and store in a object
     */
    rowCnt_ = calcRowCnt();
    colCnt_ = calcColCnt();

    /*
     * Successfull return
     */
    return ReturnCode::SUCCESS;
}

bool MemoryMappedCsvContainer::close()
{
    /*
     * Unmap memmory mapped region
     */
    if(bufferOriginal_)
    {
        munmap(bufferOriginal_, static_cast<size_t>(size_));
    }

    /*
     * Close opened file
     */
    if(fd_ != -1)
    {
        ::close(fd_);
    }

    /*
     * Reset all the members to their default values
     */
    fd_ = -1;
    filename_ = "";
    size_ = 0;
    buffer_ = nullptr;
    bufferOriginal_ = nullptr;

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
    if(!bufferOriginal_)
    {
        return "";
    }

    /*
     * Copy of the intenal pointer to the buffer
     */
    char* localBuffer = bufferOriginal_;

    /*
     * Swipe to row
     */
    for(auto i = 0u; i < row; ++i)
    {
        while(*localBuffer != '\n') {}
    }

    /*
     * Where to store whole row
     */
    std::string result;

    /*
     * Read whole row
     */
    while(*localBuffer != '\n')
    {
        result += *localBuffer++;
    }

    return result;
}

void
MemoryMappedCsvContainer::data(std::vector<std::vector<std::string>>& dataCache) const
{
    /*
     * Copy of the intenal pointer to the buffer
     */
    char* localBuffer = bufferOriginal_;

    for(int row = 0u; row < rowCnt_; ++row)
    {
        dataCache.emplace_back(std::vector<std::string>{});
        for(int col = 0u; col < colCnt_; ++col)
        {
            std::string result;
            while(*localBuffer != delim_ && *localBuffer != '\n')
            {
                result += *localBuffer++;
            }

            localBuffer++;
            dataCache.back().emplace_back(std::move(result));
        }
    }
}

void MemoryMappedCsvContainer::reset()
{
//    buffer_ = bufferOriginal_;
}

std::size_t MemoryMappedCsvContainer::calcRowCnt() const
{
    if(!bufferOriginal_)
    {
        return 0;
    }

    /*
     * Number of rows so far
     */
    std::size_t rows = 0;

    /*
     * Copy of the beggining of the buffer
     */
    char* localBuffer = bufferOriginal_;

    /*
     * Size so far in the bytes
     */
    long sz = 0;

    /*
     * Read #size_ bytes and check every byte for null-termination
     */
    while(sz++ != size_)
    {
        rows += *localBuffer++ == '\n';
    }

    return rows;
}

std::size_t MemoryMappedCsvContainer::calcColCnt() const
{
    if(!bufferOriginal_)
    {
        return 0;
    }

    /*
     * Number of columns so far
     */
    std::size_t cols = 0;

    /*
     * Copy of the beggining of the buffer
     */
    char* localBuffer = bufferOriginal_;

    /*
     * Read till end of line and check every byte for tab symbol
     */
    while(*localBuffer != '\n')
    {
        cols += *localBuffer++ == delim_;
    }

    return cols + 1;
}
