#include <fstream>
#include <memory>

#include "../include/big_consts.hpp"
#include "../include/big_core_read.hpp"

namespace big_test
{
    //void checkHeader(std::ifstream &file);

    //void checkMetaData(std::ifstream &file, const std::vector<uint64_t> &metaData);

    //template <typename T>
    //void checkData(std::ifstream &file, std::shared_ptr<T> data, uint64_t n, uint64_t index);

    void writeHeader(std::ofstream &file);

    void writeMetaData(std::ofstream &file, const std::vector<uint64_t> &metaData);

    template <typename T>
    void writeData(std::ofstream &file, std::shared_ptr<T> data, uint64_t n, uint64_t index);

}

namespace big_test
{
    //void checkHeader(std::ifstream &file)
    //{
    //    std::string header(big::CHUNK_LENGTH, ' ');
    //    file.read(&header[0], big::CHUNK_LENGTH);
    //    Assert::AreEqual(big::MAGIC, header);
    //}

    //void checkMetaData(std::ifstream &file, const std::vector<uint64_t> &metaData)
    //{
    //    for (const auto & d : metaData) {
    //        uint64_t number;
    //        file.read(reinterpret_cast<char*>(&number), big::CHUNK_LENGTH);
    //        Assert::AreEqual(d, number);
    //    }
    //}

    //template <typename T>
    //void checkData(std::ifstream &file, std::shared_ptr<T> data, uint64_t n, uint64_t index)
    //{
    //    uint64_t number;
    //    file.read(reinterpret_cast<char*>(&number), big::CHUNK_LENGTH);
    //    Assert::AreEqual(7ull, number);
    //    file.read(reinterpret_cast<char*>(&number), big::CHUNK_LENGTH);
    //    uint64_t complement = (sizeof(T) * n) % big::CHUNK_LENGTH > 0 ? big::CHUNK_LENGTH - (sizeof(T) * n) % big::CHUNK_LENGTH : 0;
    //    uint64_t length = big::CHUNK_LENGTH + sizeof(T) * n + complement;
    //    Assert::AreEqual(length, number);
    //    file.read(reinterpret_cast<char*>(&number), big::CHUNK_LENGTH);
    //    Assert::AreEqual(index, number);
    //    for (uint64_t i = 0; i != n; ++i) {
    //        T number;
    //        file.read(reinterpret_cast<char*>(&number), sizeof(T));
    //        Assert::AreEqual(data.get()[i], number);
    //    }
    //    uint64_t zero = 0;
    //    file.read(reinterpret_cast<char*>(&zero), complement);
    //    Assert::AreEqual(0ull, zero);
    //}

    //template void checkData(std::ifstream &file, std::shared_ptr<uint8_t> data, uint64_t n, uint64_t index);
    //template void checkData(std::ifstream &file, std::shared_ptr<uint16_t> data, uint64_t n, uint64_t index);
    //template void checkData(std::ifstream &file, std::shared_ptr<uint32_t> data, uint64_t n, uint64_t index);
    //template void checkData(std::ifstream &file, std::shared_ptr<uint64_t> data, uint64_t n, uint64_t index);
    //template void checkData(std::ifstream &file, std::shared_ptr<int8_t> data, uint64_t n, uint64_t index);
    //template void checkData(std::ifstream &file, std::shared_ptr<int16_t> data, uint64_t n, uint64_t index);
    //template void checkData(std::ifstream &file, std::shared_ptr<int32_t> data, uint64_t n, uint64_t index);
    //template void checkData(std::ifstream &file, std::shared_ptr<int64_t> data, uint64_t n, uint64_t index);
    ////template void checkData(std::ifstream &file, std::shared_ptr<half> data, uint64_t n, uint64_t index);
    //template void checkData(std::ifstream &file, std::shared_ptr<float> data, uint64_t n, uint64_t index);
    //template void checkData(std::ifstream &file, std::shared_ptr<double> data, uint64_t n, uint64_t index);

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

using namespace big_test;

int main()
{

    {
        uint64_t n = 2 * 4;
        std::shared_ptr<uint8_t> data{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
        for (int i = 0; i != n; ++i) data.get()[i] = i;
        {
            std::ofstream file("testCoreRead_GetImage1.big", std::ios_base::binary | std::ios_base::out);
            writeHeader(file);
            std::vector<uint64_t> metaData{ 1, 8, 1, 2, 8, 2, 3, 8, 4, 4, 8, 1, 5, 32, 1, 2, 3, 4, 6, 8, 1 };
            writeMetaData(file, metaData);
            writeData(file, data, n, 0);
        }
        {
            big::BigCoreRead big("testCoreRead_GetImage1.big");
            auto vec = big.getImage<uint8_t>(0);
            for (uint64_t index = 0; index != vec.size(); ++index) {
                //Assert::AreEqual(data.get()[index], vec[index]);
            }
        }
        {
            big::BigCoreRead big("testCoreRead_GetImage1.big", false, 0);
            auto vec = big.getImage<uint8_t>(0);
            for (uint64_t index = 0; index != vec.size(); ++index) {
                //Assert::AreEqual(data.get()[index], vec[index]);
            }
        }
    }
    {
        uint64_t n = 3 * 5 * 3;
        std::shared_ptr<uint8_t> data1{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
        for (uint8_t i = 0; i != n; ++i) data1.get()[i] = i;
        std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
        for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
        {
            std::ofstream file("testCoreRead_GetImage2.big", std::ios_base::binary | std::ios_base::out);
            writeHeader(file);
            std::vector<uint64_t> metaData{ 1, 8, 2, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32, 1, 2, 3, 4, 6, 8, 1 };
            writeMetaData(file, metaData);
            writeData(file, data1, n, 0);
            writeData(file, data2, n, 1);
        }
        {
            big::BigCoreRead big("testCoreRead_GetImage2.big");
            auto vec = big.getImage<uint8_t>(0);
            for (uint64_t index = 0; index != vec.size(); ++index) {
                //Assert::AreEqual(data1.get()[index], vec[index]);
            }
            auto vec2 = big.getImage<uint8_t>(1);
            for (uint64_t index = 0; index != vec2.size(); ++index) {
                //Assert::AreEqual(data2.get()[index], vec2[index]);
            }
        }
        {
            big::BigCoreRead big("testCoreRead_GetImage2.big", false, 0);
            auto vec = big.getImage<uint8_t>(0);
            for (uint64_t index = 0; index != vec.size(); ++index) {
                //Assert::AreEqual(data1.get()[index], vec[index]);
            }
            auto vec2 = big.getImage<uint8_t>(1);
            for (uint64_t index = 0; index != vec2.size(); ++index) {
                //Assert::AreEqual(data2.get()[index], vec2[index]);
            }
        }
    }
    {
        std::vector<big::ChunkIds> dataOrder = { big::ChunkIds::IMAGE_HEIGHT, big::ChunkIds::IMAGE_WIDTH, big::ChunkIds::NUMBER_OF_PLANES, big::ChunkIds::NUMBER_OF_IMAGES };
        std::vector<big::DataTypes> dataType = { big::DataTypes::UINT16_T };
        uint16_t nCols = 50;
        uint16_t nPlanes = 3;
        uint16_t nImages = 20;
        uint16_t n = nCols * nPlanes * nImages;
        std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
        for (uint16_t i = 0; i != n; ++i) data1.get()[i] = i;
        std::shared_ptr<uint16_t> data2{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
        for (uint16_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint16_t>(n) + i;
        std::shared_ptr<uint16_t> data3{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
        for (uint16_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint16_t>(n) + i;
        {
            std::ofstream file("testCoreRead_GetImage3.big", std::ios_base::binary | std::ios_base::out);
            writeHeader(file);
            std::vector<uint64_t> metaData{ 1, 8, 20, 2, 8, 3, 3, 8, 50, 4, 8, 3, 5, 32 };// , 1, 2, 3, 4, 6, 8, 1 };
            writeMetaData(file, metaData);
            metaData.clear();
            for (const auto &d : dataOrder) metaData.push_back(static_cast<uint64_t>(d));
            writeMetaData(file, metaData);
            metaData = { 6, 8 };
            writeMetaData(file, metaData);
            metaData.clear();
            for (const auto &d : dataType) metaData.push_back(static_cast<uint64_t>(d));
            writeMetaData(file, metaData);
            writeData(file, data1, n, 0);
            writeData(file, data2, n, 1);
            writeData(file, data3, n, 2);
        }
        {
            big::BigCoreRead big("testCoreRead_GetImage3.big", true);

            for (int i = 0; i < 300000; ++i) {
                for (int j = 0; j < nImages; ++j) {
                    auto vec = big.getImage<uint16_t>(j);
                }
            }
            //std::shared_ptr<uint16_t> data;
            //uint64_t index = 0;
            //std::vector<bool> results;
            //results.reserve(3 * n);
            //for (uint64_t row = 0; row != 3; ++row) {
            //    for (uint64_t col = 0; col != 5; ++col) {
            //        for (uint64_t plane = 0; plane != 3; ++plane) {
            //            auto vec = &vec1;
            //            for (uint64_t imageNum = 0; imageNum != 2; ++imageNum) {
            //                if (index / n == 0) data = data1;
            //                else if (index / n == 1) data = data2;
            //                else if (index / n == 2) data = data3;
            //                results.push_back(data.get()[index % n] == (*vec)[row * 5 * 3 + col * 3 + plane]);
            //                //Assert::AreEqual(data.get()[index % n], (*vec)[row * 5 * 3 + col * 3 + plane]);
            //                ++index;
            //                vec = &vec2;
            //            }
            //        }
            //    }
            //}
        }
    }



    return 0;
}

