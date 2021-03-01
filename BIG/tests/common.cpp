#include "common.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace big_test
{
    using half_float::half;
    using half_float::half_cast;
    void checkHeader(std::ifstream &file)
    {
        std::string header(big::CHUNK_LENGTH, ' ');
        file.read(&header[0], big::CHUNK_LENGTH);
        Assert::AreEqual(big::MAGIC, header);
    }
    //check metadata and xml filename
    void checkMetaData(std::ifstream &file, const std::vector<uint64_t> &metaData, std::string correctName)
    {
      for (const auto & d : metaData) {
        uint64_t number;
        file.read(reinterpret_cast<char*>(&number), big::CHUNK_LENGTH);
        Assert::AreEqual(d, number);
      }

      //check xml filename
      //uint64_t n_chunk;
      //file.read(reinterpret_cast<char*>(&n_chunk), big::CHUNK_LENGTH);
      //Assert::AreEqual(n_chunk, 7ull);
      //uint64_t length;
      //file.read(reinterpret_cast<char*>(&length), big::CHUNK_LENGTH);
      //std::string fileName;
      //fileName.resize(length);
      //file.read(reinterpret_cast<char*>(&fileName[0]), length);
      //fileName.resize(fileName.find_first_of('\0')); //resize string to \0 end symbol
      //Assert::AreEqual(fileName, correctName);

    }
    void checkMetaData(std::ifstream &file, const std::vector<uint64_t> &metaData)
    {
        for (const auto & d : metaData) {
            uint64_t number;
            file.read(reinterpret_cast<char*>(&number), big::CHUNK_LENGTH);
            Assert::AreEqual(d, number);
        }

    }
    template<typename T>
    void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<T> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n)
    {
        uint64_t index = 0;
        for (uint64_t rowIndex = 0; rowIndex != row; ++rowIndex) {
            for (uint64_t colIndex = 0; colIndex != col; ++colIndex) {
                for (uint64_t planeIndex = 0; planeIndex != plane; ++planeIndex) {
                    T d = big.at<T>(imageNum, rowIndex, colIndex, planeIndex);
                    Assert::AreEqual(data.get()[index], d);
                    d = big.get<T>(imageNum, rowIndex, colIndex, planeIndex);
                    Assert::AreEqual(data.get()[index % n], d);
                    ++index;
                }
            }
        }
    }

    template void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<uint8_t> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n);
    template void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<uint16_t> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n);
    template void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<uint32_t> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n);
    template void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<uint64_t> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n);
    template void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<int8_t> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n);
    template void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<int16_t> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n);
    template void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<int32_t> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n);
    template void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<int64_t> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n);
    template void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<float> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n);
    template void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<double> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n);
    template void checkAtandGet(big::BigCoreRead &big, std::shared_ptr<half> data, uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane, uint64_t n);
    
    template <typename T>
    void checkData(std::ifstream &file, std::shared_ptr<T> data, uint64_t n, uint64_t index, uint64_t dataType)
    {
        uint64_t number;
        file.read(reinterpret_cast<char*>(&number), big::CHUNK_LENGTH);
        Assert::AreEqual(6ull, number);
        file.read(reinterpret_cast<char*>(&number), big::CHUNK_LENGTH);
        uint64_t complement = (sizeof(T) * n) % big::CHUNK_LENGTH > 0 ? big::CHUNK_LENGTH - (sizeof(T) * n) % big::CHUNK_LENGTH : 0;
        uint64_t length = 2*big::CHUNK_LENGTH + sizeof(T) * n + complement;
        Assert::AreEqual(length, number);
        file.read(reinterpret_cast<char*>(&number), big::CHUNK_LENGTH);
        Assert::AreEqual(index, number);
        file.read(reinterpret_cast<char*>(&number), big::CHUNK_LENGTH);
        Assert::AreEqual(dataType, number);
        for (uint64_t i = 0; i != n; ++i) {
            T number;
            file.read(reinterpret_cast<char*>(&number), sizeof(T));
            Assert::AreEqual(data.get()[i], number);
        }
        uint64_t zero = 0;
        file.read(reinterpret_cast<char*>(&zero), complement);
        Assert::AreEqual(0ull, zero);
    }

    template void checkData(std::ifstream &file, std::shared_ptr<uint8_t> data, uint64_t n, uint64_t index, uint64_t dataType);
    template void checkData(std::ifstream &file, std::shared_ptr<uint16_t> data, uint64_t n, uint64_t index, uint64_t dataType);
    template void checkData(std::ifstream &file, std::shared_ptr<uint32_t> data, uint64_t n, uint64_t index, uint64_t dataType);
    template void checkData(std::ifstream &file, std::shared_ptr<uint64_t> data, uint64_t n, uint64_t index, uint64_t dataType);
    template void checkData(std::ifstream &file, std::shared_ptr<int8_t> data, uint64_t n, uint64_t index, uint64_t dataType);
    template void checkData(std::ifstream &file, std::shared_ptr<int16_t> data, uint64_t n, uint64_t index, uint64_t dataType);
    template void checkData(std::ifstream &file, std::shared_ptr<int32_t> data, uint64_t n, uint64_t index, uint64_t dataType);
    template void checkData(std::ifstream &file, std::shared_ptr<int64_t> data, uint64_t n, uint64_t index, uint64_t dataType);
    template void checkData(std::ifstream &file, std::shared_ptr<half> data, uint64_t n, uint64_t index, uint64_t dataType);
    template void checkData(std::ifstream &file, std::shared_ptr<float> data, uint64_t n, uint64_t index, uint64_t dataType);
    template void checkData(std::ifstream &file, std::shared_ptr<double> data, uint64_t n, uint64_t index, uint64_t dataType);

    void writeHeader(std::ofstream &file)
    {
        file.write(&big::MAGIC[0], big::CHUNK_LENGTH);
    }

    void writeMetaData(std::ofstream &file, const std::vector<uint64_t> &metaData)
    {
        for (const auto & d : metaData) {
            file.write(reinterpret_cast<const char*>(&d), big::CHUNK_LENGTH);
        }
    }

    template <typename T>
    void writeData(std::ofstream &file, std::shared_ptr<T> data, uint64_t n, uint64_t index)
    {
        uint64_t number = 7ull;
        file.write(reinterpret_cast<char*>(&number), big::CHUNK_LENGTH);
        uint64_t complement = (sizeof(T) * n) % big::CHUNK_LENGTH > 0 ? big::CHUNK_LENGTH - (sizeof(T) * n) % big::CHUNK_LENGTH : 0;
        uint64_t length = big::CHUNK_LENGTH + sizeof(T) * n + complement;
        file.write(reinterpret_cast<char*>(&length), big::CHUNK_LENGTH);
        file.write(reinterpret_cast<char*>(&index), big::CHUNK_LENGTH);
        for (uint64_t i = 0; i != n; ++i) {
            file.write(reinterpret_cast<char*>(&data.get()[i]), sizeof(T));
        }
        uint64_t zero = 0;
        file.write(reinterpret_cast<char*>(&zero), complement);
    }

    template void writeData(std::ofstream &file, std::shared_ptr<uint8_t> data, uint64_t n, uint64_t index);
    template void writeData(std::ofstream &file, std::shared_ptr<uint16_t> data, uint64_t n, uint64_t index);
    template void writeData(std::ofstream &file, std::shared_ptr<uint32_t> data, uint64_t n, uint64_t index);
    template void writeData(std::ofstream &file, std::shared_ptr<uint64_t> data, uint64_t n, uint64_t index);
    template void writeData(std::ofstream &file, std::shared_ptr<int8_t> data, uint64_t n, uint64_t index);
    template void writeData(std::ofstream &file, std::shared_ptr<int16_t> data, uint64_t n, uint64_t index);
    template void writeData(std::ofstream &file, std::shared_ptr<int32_t> data, uint64_t n, uint64_t index);
    template void writeData(std::ofstream &file, std::shared_ptr<int64_t> data, uint64_t n, uint64_t index);
    //template void writeData(std::ofstream &file, std::shared_ptr<half> data, uint64_t n, uint64_t index);
    template void writeData(std::ofstream &file, std::shared_ptr<float> data, uint64_t n, uint64_t index);
    template void writeData(std::ofstream &file, std::shared_ptr<double> data, uint64_t n, uint64_t index);
}
