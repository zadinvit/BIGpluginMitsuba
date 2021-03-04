#pragma once
#ifndef _BIG_CORE_READ_INFO_H_
#define _BIG_CORE_READ_INFO_H_

#include "big_core_base.hpp"

#include <fstream>

namespace big
{

    class BigCoreReadInfo : public BigCoreBase
    {
    public:

        // Constructor. Opens file, reads parameters of the container, but does not read data.
        BigCoreReadInfo(const std::string& fileName);

        // Copy constructor forbidden.
        BigCoreReadInfo(const BigCoreReadInfo &) = delete;

        // Move constructor.
        BigCoreReadInfo(BigCoreReadInfo &&other) = default;

        // Destructor.
        ~BigCoreReadInfo();

        // Copy-assignment operator forbidden.
        BigCoreReadInfo &operator=(const BigCoreReadInfo &) = delete;

        // Move-assignment operator.
        BigCoreReadInfo &operator=(BigCoreReadInfo &&rhs) = default;

    protected:

        // Reads chunk's id and its length.
        bool readChunk(ChunkIds& id, uint64_t& length);

        // Reads chunk's data, e.g., numberOfImages, imageHeight, etc. Does not read the data itself, just remembers their position.
        bool readData(ChunkIds id, uint64_t length);

    protected:

        std::ifstream file;     // attached file
    };

}

#endif // _BIG_CORE_READ_INFO_H_
