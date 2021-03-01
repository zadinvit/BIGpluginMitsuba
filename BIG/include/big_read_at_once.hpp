#pragma once
#ifndef _BIG_READ_AT_ONCE_H_
#define _BIG_READ_AT_ONCE_H_

#include "big_core_read_info.hpp"

#include <fstream>
#include <array>
#include <memory>

namespace big
{
    class BigReadAtOnce : public BigCoreReadInfo
    {
    public:

        // Constructor. Opens file, reads parameters of the container, but does not read data.
        BigReadAtOnce(const std::string& fileName) : BigCoreReadInfo(fileName) {}

        // Copy constructor forbidden.
        BigReadAtOnce(const BigReadAtOnce &) = delete;

        // Move constructor.
        BigReadAtOnce(BigReadAtOnce &&other) = default;

        // Destructor.
        ~BigReadAtOnce() = default;

        // Copy-assignment operator forbidden.
        BigReadAtOnce &operator=(const BigReadAtOnce &) = delete;

        // Move-assignment operator.
        BigReadAtOnce &operator=(BigReadAtOnce &&rhs) = default;

        // Loads all data into an array.
        std::shared_ptr<char> getData();
    };
}

#endif // _BIG_READ_AT_ONCE_H_
