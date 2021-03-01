#include "common.hpp"

#include "../include/big_core_read.hpp"
#include "../include/big_core_write.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace big_test
{
    TEST_CLASS(BigCore_UnitTest)
    {
    public:

        TEST_METHOD(ReorganizeData)
        {
            std::vector<big::ChunkIds> dataOrder = { big::ChunkIds::IMAGE_HEIGHT, big::ChunkIds::IMAGE_WIDTH, big::ChunkIds::NUMBER_OF_PLANES, big::ChunkIds::NUMBER_OF_IMAGES };
            std::vector<big::DataTypes> dataType = { big::DataTypes::UINT16_T };
            uint16_t n = 5 * 3 * 2;
            std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
            for (uint16_t i = 0; i != n; ++i) data1.get()[i] = 10 * i;
            std::shared_ptr<uint16_t> data2{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
            for (uint16_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint16_t>(n) + i;
            std::shared_ptr<uint16_t> data3{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
            for (uint16_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint16_t>(n) + i;
            {
                std::ofstream file("testCore_Reorganize1.big", std::ios_base::binary | std::ios_base::out);
                writeHeader(file);
                std::vector<uint64_t> metaData{ 1, 8, 2, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32 };// , 1, 2, 3, 4, 6, 8, 1 };
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
                //catch exception, which is throw if dataorder is not correct
                try {
                    big::BigCoreRead big_read("testCore_Reorganize1.big");				
                    big::BigCoreWrite big_write("testCore_Reorganize2.big", big_read.getImageHeight(), big_read.getImageWidth(), big_read.getNumberOfPlanes(), big::defaultDataOrder);
                    for (uint64_t imageNum = 0; imageNum < big_read.getNumberOfImages(); ++imageNum) {
                        std::vector<uint16_t> data = big_read.getImage<uint16_t>(imageNum);
                        std::shared_ptr<uint16_t> d = std::shared_ptr<uint16_t>(data.data(), [](uint16_t *d) {});
                        big_write.addEntity(d, imageNum, big::DataTypes::UINT16_T);
                    }
                }
                catch (const char * str)
                {
                    Assert::AreEqual(str, "This data order is not supported!");
                }
            }
            {
                //catch exception, which is throw if dataorder is not correct
                try {
                    big::BigCoreRead big1("testCore_Reorganize1.big");
                    big::BigCoreRead big2("testCore_Reorganize2.big");
                    for (uint64_t imageNum = 0; imageNum != 2; ++imageNum) {
                        for (uint64_t row = 0; row != 3; ++row) {
                            for (uint64_t col = 0; col != 5; ++col) {
                                for (uint64_t plane = 0; plane != 3; ++plane) {
                                    uint16_t d1 = big1.get<uint16_t>(imageNum, row, col, plane);
                                    uint16_t d2 = big2.get<uint16_t>(imageNum, row, col, plane);
                                    Assert::AreEqual(d1, d2);
                                }
                            }
                        }
                    }
                }
                catch (const char * str)
                {
                    Assert::AreEqual(str, "This data order is not supported!");
                }
            }
        }


    };
}
