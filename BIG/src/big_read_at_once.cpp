#include "../include/big_read_at_once.hpp"

namespace big
{
    std::shared_ptr<char> BigReadAtOnce::getData()
    {
        std::shared_ptr<char> data = std::shared_ptr<char>(new char[dataSize], [](char *p) { delete[] p; });;
        for (uint64_t i = 0; i != dimensions[0]; ++i) {
            file.seekg(dataPositions[i]);
            file.read(data.get() + offsets[i], entitySizes[i]);
        }
        return data;
    }
}
