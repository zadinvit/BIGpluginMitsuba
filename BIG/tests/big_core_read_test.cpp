#include "common.hpp"

#include "../include/big_core_read.hpp"
#include "../include/big_core_write.hpp"
#include <random>
#include <cmath>

#define DATA_TEST_SIZE 7500
#define DATA_TEST_SIZE2 300 //for cache test where i read all elements of file
#define NUMBER_OF_READS 150000
#define CACHE_SIZE 10000000ull
#define DEVIATION 10.0 // standart deviation for normal distrubution


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace big_test
{		
	inline bool exists_test(const std::string& name) {
    		if (FILE *file = fopen(name.c_str(), "r")) {
			fclose(file);
			return true;
		}
		else {
			return false;
		}
	}
    TEST_CLASS(BigCoreRead_UnitTest)
    {
    public:
        
        TEST_METHOD(BigCoreRead_GetAndAt)
        {
            {
                uint64_t n = 2 * 4;
                std::shared_ptr<uint8_t> data{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (int i = 0; i != n; ++i) data.get()[i] = i;
                {
                    
                    big::BigCoreWrite big("testCoreRead_Get1.big", 2, 4, 1 ); // default data order
                    big.addEntity(data, 0, big::DataTypes::UINT8_T);
                }
                {
                    big::BigCoreRead big("testCoreRead_Get1.big");
                    Assert::AreEqual(1ull, big.getNumberOfImages());
                    Assert::AreEqual(2ull, big.getImageHeight());
                    Assert::AreEqual(4ull, big.getImageWidth());
                    Assert::AreEqual(1ull, big.getNumberOfPlanes());
                    const auto &dataOrder = big.getDataOrder();
                    for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                        Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
                    }
                   
                    uint64_t index = 0;
                    uint64_t imageNum = 0;
                    uint64_t plane = 0;
                    for (uint64_t row = 0; row != 2; ++row) {
                        for (uint64_t col = 0; col != 4; ++col) {
                            uint8_t d = big.at<uint8_t>(imageNum, row, col);
                            Assert::AreEqual(data.get()[index], d);
                            d = big.get<uint8_t>(imageNum, row, col);
                            Assert::AreEqual(data.get()[index], d);
                            ++index;
                        }
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
                    /*std::ofstream file("testCoreRead_Get2.big", std::ios_base::binary | std::ios_base::out);
                    writeHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 2, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32, 1, 2, 3, 4, 6, 8, 1 };
                    writeMetaData(file, metaData);
                    writeData(file, data1, n, 0);
                    writeData(file, data2, n, 1);*/
                    big::BigCoreWrite big("testCoreRead_Get2.big", 3, 5, 3); // default data order
                    big.pushEntity(data1, big::DataTypes::UINT8_T);
                    big.pushEntity(data2, big::DataTypes::UINT8_T);
                    
                }
                {
                    big::BigCoreRead big("testCoreRead_Get2.big");
                    Assert::AreEqual(2ull, big.getNumberOfImages());
                    Assert::AreEqual(3ull, big.getImageHeight());
                    Assert::AreEqual(5ull, big.getImageWidth());
                    Assert::AreEqual(3ull, big.getNumberOfPlanes());
                    const auto &dataOrder = big.getDataOrder();
                    for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                        Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
                    }
                    const auto &dataType = big.getDataType();
                    for (uint64_t i = 0; i != dataType.size(); ++i) {
                        Assert::AreEqual(big::defaultDataType[0], dataType[i]);
                    }
                    std::shared_ptr<uint8_t> data = data1;
                    uint64_t index = 0;
                    for (uint64_t imageNum = 0; imageNum != 2; ++imageNum) {
                        for (uint64_t row = 0; row != 3; ++row) {
                            for (uint64_t col = 0; col != 5; ++col) {
                                for (uint64_t plane = 0; plane != 3; ++plane) {
                                    uint8_t d = big.at<uint8_t>(imageNum, row, col, plane);
                                    Assert::AreEqual(data.get()[index % n], d);
                                    d = big.get<uint8_t>(imageNum, row, col, plane);
                                    Assert::AreEqual(data.get()[index % n], d);
                                    ++index;
                                    if (index == n) data = data2;
                                }
                            }
                        }
                    }
                }
            }
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
                    std::ofstream file("testCoreRead_Get3.big", std::ios_base::binary | std::ios_base::out);
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
                    try
                    {
                        big::BigCoreRead big("testCoreRead_Get3.big");
                    
                        Assert::AreEqual(2ull, big.getNumberOfImages());
                        Assert::AreEqual(3ull, big.getImageHeight());
                        Assert::AreEqual(5ull, big.getImageWidth());
                        Assert::AreEqual(3ull, big.getNumberOfPlanes());
                    
                            const auto &dataOrder2 = big.getDataOrder();
                    
                    
                    
                        for (uint64_t i = 0; i != dataOrder2.size(); ++i) { 
                            Assert::AreEqual(dataOrder[i], dataOrder2[i]);
                        }
                        const auto &dataType2 = big.getDataType();
                        for (uint64_t i = 0; i != dataType2.size(); ++i) {
                            Assert::AreEqual(dataType[i], dataType2[i]);
                        }
                        std::shared_ptr<uint16_t> data = data1;
                        uint64_t index = 0;
                        for (uint64_t row = 0; row != 3; ++row) {
                            for (uint64_t col = 0; col != 5; ++col) {
                                for (uint64_t plane = 0; plane != 3; ++plane) {
                                    for (uint64_t imageNum = 0; imageNum != 2; ++imageNum) {
                                        uint16_t d = big.at<uint16_t>(imageNum, row, col, plane);
                                        Assert::AreEqual(data.get()[index % n], d);
                                        d = big.get<uint16_t>(imageNum, row, col, plane);
                                        Assert::AreEqual(data.get()[index % n], d);
                                        ++index;
                                        if (index == n) data = data2;
                                        else if (index == 2 * n) data = data3;
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
        }

        TEST_METHOD(BigCoreRead_SmallCache) //Cache have size of one entity
        {
            {
                uint64_t n = 3 * 5 * 3;
                std::shared_ptr<uint8_t> data1{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (uint8_t i = 0; i != n; ++i) data1.get()[i] = i;
                std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
                {
                    /*std::ofstream file("testCoreRead_SmallCache1.big", std::ios_base::binary | std::ios_base::out);
                    writeHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 2, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32, 1, 2, 3, 4}; , 6, 8, 1 
                    writeMetaData(file, metaData);
                    writeData(file, data1, n, 0);
                    writeData(file, data2, n, 1);*/
                    big::BigCoreWrite big("testCoreRead_SmallCache1.big", 3, 5, 3); // default data order
                    big.addEntity(data1, 0, big::DataTypes::UINT8_T);
                    big.addEntity(data2, 1, big::DataTypes::UINT8_T);

                }
                {
                    big::BigCoreRead big("testCoreRead_SmallCache1.big", false, n);
                    Assert::AreEqual(2ull, big.getNumberOfImages());
                    Assert::AreEqual(3ull, big.getImageHeight());
                    Assert::AreEqual(5ull, big.getImageWidth());
                    Assert::AreEqual(3ull, big.getNumberOfPlanes());
                    const auto &dataOrder = big.getDataOrder();
                    for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                        Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
                    }
                    const auto &dataType = big.getDataType();
                    for (uint64_t i = 0; i != dataType.size(); ++i) {
                        Assert::AreEqual(big::defaultDataType[0], dataType[i]);
                    }
                    std::shared_ptr<uint8_t> data = data1;
                    uint64_t index = 0;
                    for (uint64_t imageNum = 0; imageNum != 2; ++imageNum) {
                        for (uint64_t row = 0; row != 3; ++row) {
                            for (uint64_t col = 0; col != 5; ++col) {
                                for (uint64_t plane = 0; plane != 3; ++plane) {
                                    uint8_t d = big.get<uint8_t>(imageNum, row, col, plane);
                                    Assert::AreEqual(data.get()[index % n], d);
                                    ++index;
                                    if (index == n) data = data2;
                                }
                            }
                        }
                    }
                }
            }
            {

                //this test throw exception, is not upgrade to new datatype
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
                    std::ofstream file("testCoreRead_SmallCache2.big", std::ios_base::binary | std::ios_base::out);
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

                    
                        big::BigCoreRead big("testCoreRead_SmallCache2.big", false, sizeof(uint16_t) * n);
                

                        Assert::AreEqual(2ull, big.getNumberOfImages());
                        Assert::AreEqual(3ull, big.getImageHeight());
                        Assert::AreEqual(5ull, big.getImageWidth());
                        Assert::AreEqual(3ull, big.getNumberOfPlanes());
                        const auto &dataOrder2 = big.getDataOrder();
                        for (uint64_t i = 0; i != dataOrder2.size(); ++i) {
                            Assert::AreEqual(dataOrder[i], dataOrder2[i]);
                        }
                        const auto &dataType2 = big.getDataType();
                        for (uint64_t i = 0; i != dataType2.size(); ++i) {
                            Assert::AreEqual(dataType[i], dataType2[i]);
                        }
                        for (uint64_t imageNum = 0; imageNum != 2; ++imageNum) {
                            for (uint64_t row = 0; row != 3; ++row) {
                                for (uint64_t col = 0; col != 5; ++col) {
                                    for (uint64_t plane = 0; plane != 3; ++plane) {
                                        uint64_t index = row * 5 * 3 * 2 + col * 3 * 2 + plane * 2 + imageNum;
                                        std::shared_ptr<uint16_t> data;
                                        if (index / n == 0) data = data1;
                                        else if (index / n == 1) data = data2;
                                        else if (index / n == 2) data = data3;
                                        uint16_t d = big.get<uint16_t>(imageNum, row, col, plane);
                                        Assert::AreEqual(data.get()[index % n], d);
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
        }
        TEST_METHOD(BigCoreRead_ZeroCache)
        {
            {
                std::vector<std::shared_ptr<uint8_t>> data;
                uint64_t n = 3 * 5 * 3;
                std::shared_ptr<uint8_t> data1{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (uint8_t i = 0; i != n; ++i) data1.get()[i] = i;
                data.push_back(data1);
                std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
                data.push_back(data2);
                {
                    /*std::ofstream file("testCoreRead_ZeroCache1.big", std::ios_base::binary | std::ios_base::out);
                    writeHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 2, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32, 1, 2, 3, 4, 6, 8, 1 };
                    writeMetaData(file, metaData);
                    writeData(file, data1, n, 0);
                    writeData(file, data2, n, 1);*/
                    big::BigCoreWrite big("testCoreRead_ZeroCache1.big", 3, 5, 3); // default data order
                    big.addEntity(data1, 0, big::DataTypes::UINT8_T);
                    big.addEntity(data2, 1, big::DataTypes::UINT8_T);
                }
                {
                    big::BigCoreRead big("testCoreRead_ZeroCache1.big", false, 0);
                    Assert::AreEqual(2ull, big.getNumberOfImages());
                    Assert::AreEqual(3ull, big.getImageHeight());
                    Assert::AreEqual(5ull, big.getImageWidth());
                    Assert::AreEqual(3ull, big.getNumberOfPlanes());
                    const auto &dataOrder = big.getDataOrder();
                    for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                        Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
                    }
                    const auto &dataType = big.getDataType();
                    for (uint64_t i = 0; i != dataType.size(); ++i) {
                        Assert::AreEqual(big::defaultDataType[0], dataType[i]);
                    }
                    
                    uint64_t index = 0;
                    for (uint64_t imageNum = 0; imageNum != 2; ++imageNum) {
                        checkAtandGet(big, data[imageNum], imageNum, 3, 5, 3, n);
                        
                    }
                    
                }
            }
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
                    std::ofstream file("testCoreRead_ZeroCache2.big", std::ios_base::binary | std::ios_base::out);
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
                    try
                    {
                        big::BigCoreRead big("testCoreRead_ZeroCache2.big", false, 0);
                        Assert::AreEqual(2ull, big.getNumberOfImages());
                        Assert::AreEqual(3ull, big.getImageHeight());
                        Assert::AreEqual(5ull, big.getImageWidth());
                        Assert::AreEqual(3ull, big.getNumberOfPlanes());
                        const auto &dataOrder2 = big.getDataOrder();
                        for (uint64_t i = 0; i != dataOrder2.size(); ++i) {
                            Assert::AreEqual(dataOrder[i], dataOrder2[i]);
                        }
                        const auto &dataType2 = big.getDataType();
                        for (uint64_t i = 0; i != dataType2.size(); ++i) {
                            Assert::AreEqual(dataType[i], dataType2[i]);
                        }
                        for (uint64_t imageNum = 0; imageNum != 2; ++imageNum) {
                            for (uint64_t row = 0; row != 3; ++row) {
                                for (uint64_t col = 0; col != 5; ++col) {
                                    for (uint64_t plane = 0; plane != 3; ++plane) {
                                        uint64_t index = row * 5 * 3 * 2 + col * 3 * 2 + plane * 2 + imageNum;
                                        std::shared_ptr<uint16_t> data;
                                        if (index / n == 0) data = data1;
                                        else if (index / n == 1) data = data2;
                                        else if (index / n == 2) data = data3;
                                        uint16_t d = big.get<uint16_t>(imageNum, row, col, plane);
                                        Assert::AreEqual(data.get()[index % n], d);
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
        }


        TEST_METHOD(BigCoreRead_CacheTest)
        {
            {
                uint64_t n = 3 * 5 * 3;
                std::shared_ptr<uint8_t> data1{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (uint8_t i = 0; i != n; ++i) data1.get()[i] = i;
                std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
                {
                    /*std::ofstream file("testCoreRead_CacheTest1.big", std::ios_base::binary | std::ios_base::out);
                    writeHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 2, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32, 1, 2, 3, 4 };//, 6, 8, 1
                    writeMetaData(file, metaData);
                    writeData(file, data1, n, 0);
                    writeData(file, data2, n, 1);*/
                    big::BigCoreWrite big("testCoreRead_CacheTest1.big", 3, 5, 3); // default data order
                    big.addEntity(data1, 0, big::DataTypes::UINT8_T);
                    big.addEntity(data2, 1, big::DataTypes::UINT8_T);
                }
                {
                    big::BigCoreRead big("testCoreRead_CacheTest1.big", true);
                    Assert::AreEqual(true, big.isAllInCache());
                    Assert::AreEqual(2 * n, big.getCacheSize());
                    big.clearCache();
                    Assert::AreEqual(false, big.isAllInCache());
                    Assert::AreEqual(0ull, big.getCacheSize());
                    big.setCacheSize(0);
                   /* big.loadToCache();
                    Assert::AreEqual(false, big.isAllInCache());
                    Assert::AreEqual(0ull, big.getCacheSize());
                    big.loadToCache(2 * n);
                    Assert::AreEqual(true, big.isAllInCache());
                    Assert::AreEqual(2 * n, big.getCacheSize());*/
                   /* big.setCacheSize(n);
                    Assert::AreEqual(true, big.isAllInCache());
                    Assert::AreEqual(2 * n, big.getCacheSize());*/
                    big.shrinkCache();
                    Assert::AreEqual(false, big.isAllInCache());
                    Assert::AreEqual( (uint64_t)0, big.getCacheSize());
                    big.setCacheSize(2 * n);
                    Assert::AreEqual(false, big.isAllInCache());
                    Assert::AreEqual((uint64_t)0, big.getCacheSize());
                }
            }
        }

        TEST_METHOD(BigCoreRead_Operator)
        {
            {
                uint64_t n = 3 * 5 * 3;
                std::shared_ptr<uint8_t> data1{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (uint8_t i = 0; i != n; ++i) data1.get()[i] = i;
                std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
                {
                   /* std::ofstream file("testCoreRead_Operator1.big", std::ios_base::binary | std::ios_base::out);
                    writeHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 2, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32, 1, 2, 3, 4, 6, 8, 1 };
                    writeMetaData(file, metaData);
                    writeData(file, data1, n, 0);
                    writeData(file, data2, n, 1);*/
                    big::BigCoreWrite big("testCoreRead_Operator1.big", 3, 5, 3); // default data order
                    big.addEntity(data1, 0, big::DataTypes::UINT8_T);
                    big.addEntity(data2, 1, big::DataTypes::UINT8_T);
                }
                {
                    big::BigCoreRead big("testCoreRead_Operator1.big", false, n);
                    Assert::AreEqual(2ull, big.getNumberOfImages());
                    Assert::AreEqual(3ull, big.getImageHeight());
                    Assert::AreEqual(5ull, big.getImageWidth());
                    Assert::AreEqual(3ull, big.getNumberOfPlanes());
                    const auto &dataOrder = big.getDataOrder();
                    for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                        Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
                    }
                    const auto &dataType = big.getDataType();
                    for (uint64_t i = 0; i != dataType.size(); ++i) {
                        Assert::AreEqual(big::defaultDataType[0], dataType[i]);
                    }

                    auto d1 = big[0];
                    uint64_t index = 0;
                    uint64_t imageNum = 0;
                    for (uint64_t row = 0; row != 3; ++row) {
                        for (uint64_t col = 0; col != 5; ++col) {
                            for (uint64_t plane = 0; plane != 3; ++plane) {
                                Assert::AreEqual(data1.get()[index], reinterpret_cast<const uint8_t*>(d1.get())[index]);
                                ++index;
                            }
                        }
                    }
                    auto d2 = big[1];
                    index = 0;
                    imageNum = 1;
                    for (uint64_t row = 0; row != 3; ++row) {
                        for (uint64_t col = 0; col != 5; ++col) {
                            for (uint64_t plane = 0; plane != 3; ++plane) {
                                Assert::AreEqual(data2.get()[index], reinterpret_cast<const uint8_t*>(d2.get())[index]);
                                ++index;
                            }
                        }
                    }
                    index = 0;
                    imageNum = 0;
                    for (uint64_t row = 0; row != 3; ++row) {
                        for (uint64_t col = 0; col != 5; ++col) {
                            for (uint64_t plane = 0; plane != 3; ++plane) {
                                Assert::AreEqual(data1.get()[index], reinterpret_cast<const uint8_t*>(d1.get())[index]);
                                ++index;
                            }
                        }
                    }
                }
                {
                    big::BigCoreRead big("testCoreRead_Operator1.big", false, 0);
                    Assert::AreEqual(2ull, big.getNumberOfImages());
                    Assert::AreEqual(3ull, big.getImageHeight());
                    Assert::AreEqual(5ull, big.getImageWidth());
                    Assert::AreEqual(3ull, big.getNumberOfPlanes());
                    const auto &dataOrder = big.getDataOrder();
                    for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                        Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
                    }
                    const auto &dataType = big.getDataType();
                    for (uint64_t i = 0; i != dataType.size(); ++i) {
                        Assert::AreEqual(big::defaultDataType[0], dataType[i]);
                    }

                    auto d1 = big[0];
                    uint64_t index = 0;
                    uint64_t imageNum = 0;
                    for (uint64_t row = 0; row != 3; ++row) {
                        for (uint64_t col = 0; col != 5; ++col) {
                            for (uint64_t plane = 0; plane != 3; ++plane) {
                                Assert::AreEqual(data1.get()[index], reinterpret_cast<const uint8_t*>(d1.get())[index]);
                                ++index;
                            }
                        }
                    }
                    auto d2 = big[1];
                    index = 0;
                    imageNum = 1;
                    for (uint64_t row = 0; row != 3; ++row) {
                        for (uint64_t col = 0; col != 5; ++col) {
                            for (uint64_t plane = 0; plane != 3; ++plane) {
                                Assert::AreEqual(data2.get()[index], reinterpret_cast<const uint8_t*>(d2.get())[index]);
                                ++index;
                            }
                        }
                    }
                    index = 0;
                    imageNum = 0;
                    for (uint64_t row = 0; row != 3; ++row) {
                        for (uint64_t col = 0; col != 5; ++col) {
                            for (uint64_t plane = 0; plane != 3; ++plane) {
                                Assert::AreEqual(data1.get()[index], reinterpret_cast<const uint8_t*>(d1.get())[index]);
                                ++index;
                            }
                        }
                    }
                }
            }
        }



        TEST_METHOD(BigCoreRead_GetEntity)
        {
            {
                uint64_t n = 2 * 4;
                std::shared_ptr<uint8_t> data{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (int i = 0; i != n; ++i) data.get()[i] = i;
                {
                    //std::ofstream file("testCoreRead_GetEntity1.big", std::ios_base::binary | std::ios_base::out);
                    //writeHeader(file);
                    //std::vector<uint64_t> metaData{ 1, 8, 1, 2, 8, 2, 3, 8, 4, 4, 8, 1, 5, 32, 1, 2, 3, 4 }; //6, 8, 1
                    //writeMetaData(file, metaData);
                    //writeData(file, data, n, 0);
                    big::BigCoreWrite big("testCoreRead_GetEntity1.big", 2, 4, 1); // default data order
                    big.addEntity(data, 0, big::DataTypes::UINT8_T);
                }
                {
                    big::BigCoreRead big("testCoreRead_GetEntity1.big");
                    auto vec = big.getEntity<uint8_t>(0);
                    uint64_t index = 0;
                    for (uint64_t row = 0; row != 2; ++row) {
                        for (uint64_t col = 0; col != 4; ++col) {
                            Assert::AreEqual(data.get()[index], vec[index]);
                            ++index;
                        }
                    }
                }
                {
                    big::BigCoreRead big("testCoreRead_GetEntity1.big", false, 0);
                    auto vec = big.getEntity<uint8_t>(0);
                    uint64_t index = 0;
                    for (uint64_t row = 0; row != 2; ++row) {
                        for (uint64_t col = 0; col != 4; ++col) {
                            Assert::AreEqual(data.get()[index], vec[index]);
                            ++index;
                        }
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
                    //std::ofstream file("testCoreRead_GetEntity2.big", std::ios_base::binary | std::ios_base::out);
                    //writeHeader(file);
                    //std::vector<uint64_t> metaData{ 1, 8, 2, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32, 1, 2, 3, 4}; //, 6, 8, 1
                    //writeMetaData(file, metaData);
                    //writeData(file, data1, n, 0);
                    //writeData(file, data2, n, 1);
                    big::BigCoreWrite big("testCoreRead_GetEntity2.big", 3, 5, 3); // default data order
                    big.addEntity(data1, 0, big::DataTypes::UINT8_T);
                    big.addEntity(data2, 1, big::DataTypes::UINT8_T);
                }
                {
                    big::BigCoreRead big("testCoreRead_GetEntity2.big");
                    auto vec = big.getEntity<uint8_t>(0);
                    for (uint64_t index = 0; index != vec.size(); ++index) {
                        Assert::AreEqual(data1.get()[index], vec[index]);
                    }
                    auto vec2 = big.getEntity<uint8_t>(1);
                    for (uint64_t index = 0; index != vec2.size(); ++index) {
                        Assert::AreEqual(data2.get()[index], vec2[index]);
                    }
                }
                {
                    big::BigCoreRead big("testCoreRead_GetEntity2.big", false, 0);
                    auto vec = big.getEntity<uint8_t>(0);
                    for (uint64_t index = 0; index != vec.size(); ++index) {
                        Assert::AreEqual(data1.get()[index], vec[index]);
                    }
                    auto vec2 = big.getEntity<uint8_t>(1);
                    for (uint64_t index = 0; index != vec2.size(); ++index) {
                        Assert::AreEqual(data2.get()[index], vec2[index]);
                    }
                }
            }
            {

                //this test is not change for datatype in data chunk, but is OK because throw exception, if dataorder is not 1 2 3 4
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
                    std::ofstream file("testCoreRead_GetEntity3.big", std::ios_base::binary | std::ios_base::out);
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
                        big::BigCoreRead big("testCoreRead_GetEntity3.big");
                        for (uint64_t i = 0; i != 3; ++i) {
                            auto vec = big.getEntity<uint16_t>(i);
                            auto data = data1;
                            if (i == 1) data = data2;
                            else if (i == 2) data = data3;
                            for (uint64_t index = 0; index != vec.size(); ++index) {
                                Assert::AreEqual(data.get()[index], vec[index]);
                            }
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
                        big::BigCoreRead big("testCoreRead_GetEntity3.big", false, 0);
                        for (uint64_t i = 0; i != 3; ++i) {
                            auto vec = big.getEntity<uint16_t>(i);
                            auto data = data1;
                            if (i == 1) data = data2;
                            else if (i == 2) data = data3;
                            for (uint64_t index = 0; index != vec.size(); ++index) {
                                Assert::AreEqual(data.get()[index], vec[index]);
                            }
                        }
                    }
                    catch (const char * str)
                    {
                        Assert::AreEqual(str, "This data order is not supported!");
                    }
                }
            }
        }

        TEST_METHOD(BigCoreRead_GetImage)
        {
            {
                uint64_t n = 2 * 4;
                std::shared_ptr<uint8_t> data{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (int i = 0; i != n; ++i) data.get()[i] = i;
                {
                    
                    big::BigCoreWrite big("testCoreRead_GetImage1.big", 2, 4, 1); // default data order
                    big.addEntity(data, 0, big::DataTypes::UINT8_T);
                }
                {
                    big::BigCoreRead big("testCoreRead_GetImage1.big");
                    auto vec = big.getImage<uint8_t>(0);
                    for (uint64_t index = 0; index != vec.size(); ++index) {
                        Assert::AreEqual(data.get()[index], vec[index]);
                    }
                }
                {
                    big::BigCoreRead big("testCoreRead_GetImage1.big", false, 0);
                    auto vec = big.getImage<uint8_t>(0);
                    for (uint64_t index = 0; index != vec.size(); ++index) {
                        Assert::AreEqual(data.get()[index], vec[index]);
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
                 
                    big::BigCoreWrite big("testCoreRead_GetImage2.big", 3, 5, 3); // default data order
                    big.addEntity(data1, 0, big::DataTypes::UINT8_T);
                    big.addEntity(data2, 1, big::DataTypes::UINT8_T);
                }
                {
                    big::BigCoreRead big("testCoreRead_GetImage2.big");
                    auto vec = big.getImage<uint8_t>(0);
                    for (uint64_t index = 0; index != vec.size(); ++index) {
                        Assert::AreEqual(data1.get()[index], vec[index]);
                    }
                    auto vec2 = big.getImage<uint8_t>(1);
                    for (uint64_t index = 0; index != vec2.size(); ++index) {
                        Assert::AreEqual(data2.get()[index], vec2[index]);
                    }
                }
                {
                    big::BigCoreRead big("testCoreRead_GetImage2.big", false, 0);
                    auto vec = big.getImage<uint8_t>(0);
                    for (uint64_t index = 0; index != vec.size(); ++index) {
                        Assert::AreEqual(data1.get()[index], vec[index]);
                    }
                    auto vec2 = big.getImage<uint8_t>(1);
                    for (uint64_t index = 0; index != vec2.size(); ++index) {
                        Assert::AreEqual(data2.get()[index], vec2[index]);
                    }
                }
            }
            {
                //this test is not change for datatype in data chunk, but is OK because throw exception, if dataorder is not 1 2 3 4
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
                    std::ofstream file("testCoreRead_GetImage3.big", std::ios_base::binary | std::ios_base::out);
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
                        big::BigCoreRead big("testCoreRead_GetImage3.big");
                        auto vec1 = big.getImage<uint16_t>(0);
                        auto vec2 = big.getImage<uint16_t>(1);
                        std::shared_ptr<uint16_t> data;
                        uint64_t index = 0;
                        for (uint64_t row = 0; row != 3; ++row) {
                            for (uint64_t col = 0; col != 5; ++col) {
                                for (uint64_t plane = 0; plane != 3; ++plane) {
                                    auto vec = &vec1;
                                    for (uint64_t imageNum = 0; imageNum != 2; ++imageNum) {
                                        if (index / n == 0) data = data1;
                                        else if (index / n == 1) data = data2;
                                        else if (index / n == 2) data = data3;
                                        Assert::AreEqual(data.get()[index % n], (*vec)[row * 5 * 3 + col * 3 + plane]);
                                        ++index;
                                        vec = &vec2;
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
                {
                    //catch exception, which is throw if dataorder is not correct
                    try {
                        big::BigCoreRead big("testCoreRead_GetImage3.big", false, 0);
                        auto vec1 = big.getImage<uint16_t>(0);
                        auto vec2 = big.getImage<uint16_t>(1);
                        std::shared_ptr<uint16_t> data;
                        uint64_t index = 0;
                        for (uint64_t row = 0; row != 3; ++row) {
                            for (uint64_t col = 0; col != 5; ++col) {
                                for (uint64_t plane = 0; plane != 3; ++plane) {
                                    auto vec = &vec1;
                                    for (uint64_t imageNum = 0; imageNum != 2; ++imageNum) {
                                        if (index / n == 0) data = data1;
                                        else if (index / n == 1) data = data2;
                                        else if (index / n == 2) data = data3;
                                        Assert::AreEqual(data.get()[index % n], (*vec)[row * 5 * 3 + col * 3 + plane]);
                                        ++index;
                                        vec = &vec2;
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
        }
		
        TEST_METHOD(BigCoreRead_diffrentDataTypes) 
        {
            
            uint16_t n = 5 * 3 * 2;
            std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
            for (uint16_t i = 0; i != n; ++i) data1.get()[i] = i;
            std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
            for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
            std::shared_ptr<uint32_t> data3{ new uint32_t[n], [](uint32_t *p) {delete[] p; } };
            for (uint32_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint32_t>(n) + i;
            std::shared_ptr<uint64_t> data4{ new uint64_t[n], [](uint64_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data4.get()[i] = 2 * static_cast<uint64_t>(n) + i;
            std::shared_ptr<int8_t> data5{ new int8_t[n], [](int8_t *p) {delete[] p; } };
            for (int8_t i = 0; i != n; ++i) data5.get()[i] = 2 * static_cast<int8_t>(n) + i;
            std::shared_ptr<int16_t> data6{ new int16_t[n], [](int16_t *p) {delete[] p; } };
            for (int16_t i = 0; i != n; ++i) data6.get()[i] = 2 * static_cast<int16_t>(n) + i;
            std::shared_ptr<int32_t> data7{ new int32_t[n], [](int32_t *p) {delete[] p; } };
            for (int32_t i = 0; i != n; ++i) data7.get()[i] = 2 * static_cast<int32_t>(n) + i;
            std::shared_ptr<int64_t> data8{ new int64_t[n], [](int64_t *p) {delete[] p; } };
            for (int64_t i = 0; i != n; ++i) data8.get()[i] = 2 * static_cast<int64_t>(n) + i;
            std::shared_ptr<float> data9{ new float[n], [](float *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) data9.get()[i] = 2 * static_cast<float>(n) + i;
            std::shared_ptr<double> data10{ new double[n], [](double *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) data10.get()[i] = 2 * static_cast<double>(n) + i;
            std::shared_ptr<half> data11{ new half[n], [](half *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) data11.get()[i] = 2 * static_cast<half> (n) + i;
            {
                big::BigCoreWrite big("testCoreRead_DiffrentDataTypes1.big", 2, 3, 5);
                big.pushEntity(data1, big::DataTypes::UINT16_T);
                big.pushEntity(data2, big::DataTypes::UINT8_T);
                big.pushEntity(data3, big::DataTypes::UINT32_T);
                big.pushEntity(data4, big::DataTypes::UINT64_T);
                big.pushEntity(data5, big::DataTypes::INT8_T);
                big.pushEntity(data6, big::DataTypes::INT16_T);
                big.pushEntity(data7, big::DataTypes::INT32_T);
                big.pushEntity(data8, big::DataTypes::INT64_T);
                big.pushEntity(data9, big::DataTypes::FLOAT);
                big.pushEntity(data10, big::DataTypes::DOUBLE);
                big.pushEntity(data11, big::DataTypes::HALF);
            }
            big::BigCoreRead big("testCoreRead_DiffrentDataTypes1.big", false, 0);
            Assert::AreEqual(11ull, big.getNumberOfImages());
            Assert::AreEqual(2ull, big.getImageHeight());
            Assert::AreEqual(3ull, big.getImageWidth());
            Assert::AreEqual(5ull, big.getNumberOfPlanes());
            const auto &dataOrder = big.getDataOrder();
            for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
            }
            std::vector<big::DataTypes> testDataType{ big::DataTypes::UINT16_T, big::DataTypes::UINT8_T, big::DataTypes::UINT32_T, big::DataTypes::UINT64_T, big::DataTypes::INT8_T,  big::DataTypes::INT16_T, big::DataTypes::INT32_T, big::DataTypes::INT64_T, big::DataTypes::FLOAT, big::DataTypes::DOUBLE, big::DataTypes::HALF };
            const auto &dataType = big.getDataType();
            for (uint64_t i = 0; i != dataType.size(); ++i) {
                Assert::AreEqual(testDataType[i], dataType[i]);
            }

            checkAtandGet(big, data1, 0, 2, 3, 5, n);
            checkAtandGet(big, data2, 1, 2, 3, 5, n);
            checkAtandGet(big, data3, 2, 2, 3, 5, n);
            checkAtandGet(big, data4, 3, 2, 3, 5, n);
            checkAtandGet(big, data5, 4, 2, 3, 5, n);
            checkAtandGet(big, data6, 5, 2, 3, 5, n);
            checkAtandGet(big, data7, 6, 2, 3, 5, n);
            checkAtandGet(big, data8, 7, 2, 3, 5, n);
            checkAtandGet(big, data9, 8, 2, 3, 5, n);
            checkAtandGet(big, data10, 9, 2, 3, 5, n);
            checkAtandGet(big, data11, 10, 2, 3, 5, n);
        }

		
        TEST_METHOD(BigCoreRead_GetImage_diffrentDatatypes) //test for all datatypes and with or without cache
        {
            uint16_t n = 5 * 3 * 2;
            std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
            for (uint16_t i = 0; i != n; ++i) data1.get()[i] = i;
            std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
            for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
            std::shared_ptr<uint32_t> data3{ new uint32_t[n], [](uint32_t *p) {delete[] p; } };
            for (uint32_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint32_t>(n) + i;
            std::shared_ptr<uint64_t> data4{ new uint64_t[n], [](uint64_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data4.get()[i] = 2 * static_cast<uint64_t>(n) + i;
            std::shared_ptr<int8_t> data5{ new int8_t[n], [](int8_t *p) {delete[] p; } };
            for (int8_t i = 0; i != n; ++i) data5.get()[i] = 2 * static_cast<int8_t>(n) + i;
            std::shared_ptr<int16_t> data6{ new int16_t[n], [](int16_t *p) {delete[] p; } };
            for (int16_t i = 0; i != n; ++i) data6.get()[i] = 2 * static_cast<int16_t>(n) + i;
            std::shared_ptr<int32_t> data7{ new int32_t[n], [](int32_t *p) {delete[] p; } };
            for (int32_t i = 0; i != n; ++i) data7.get()[i] = 2 * static_cast<int32_t>(n) + i;
            std::shared_ptr<int64_t> data8{ new int64_t[n], [](int64_t *p) {delete[] p; } };
            for (int64_t i = 0; i != n; ++i) data8.get()[i] = 2 * static_cast<int64_t>(n) + i;
            std::shared_ptr<float> data9{ new float[n], [](float *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) data9.get()[i] = 2 * static_cast<float>(n) + i;
            std::shared_ptr<double> data10{ new double[n], [](double *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) data10.get()[i] = 2 * static_cast<double>(n) + i;
            std::shared_ptr<half> data11{ new half[n], [](half *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) data11.get()[i] = 2 * static_cast<half> (n) + i;
            {
                big::BigCoreWrite big("testCoreRead_getImage_DiffrentDataTypes1.big", 2, 3, 5);
                big.pushEntity(data1, big::DataTypes::UINT16_T);
                big.pushEntity(data2, big::DataTypes::UINT8_T);
                big.pushEntity(data3, big::DataTypes::UINT32_T);
                big.pushEntity(data4, big::DataTypes::UINT64_T);
                big.pushEntity(data5, big::DataTypes::INT8_T);
                big.pushEntity(data6, big::DataTypes::INT16_T);
                big.pushEntity(data7, big::DataTypes::INT32_T);
                big.pushEntity(data8, big::DataTypes::INT64_T);
                big.pushEntity(data9, big::DataTypes::FLOAT);
                big.pushEntity(data10, big::DataTypes::DOUBLE);
                big.pushEntity(data11, big::DataTypes::HALF);
            }
            big::BigCoreRead big("testCoreRead_getImage_DiffrentDataTypes1.big", false, 0);
            Assert::AreEqual(11ull, big.getNumberOfImages());
            Assert::AreEqual(2ull, big.getImageHeight());
            Assert::AreEqual(3ull, big.getImageWidth());
            Assert::AreEqual(5ull, big.getNumberOfPlanes());
            const auto &dataOrder = big.getDataOrder();
            for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
            }
            std::vector<big::DataTypes> testDataType{ big::DataTypes::UINT16_T, big::DataTypes::UINT8_T, big::DataTypes::UINT32_T, big::DataTypes::UINT64_T, big::DataTypes::INT8_T,  big::DataTypes::INT16_T, big::DataTypes::INT32_T, big::DataTypes::INT64_T, big::DataTypes::FLOAT, big::DataTypes::DOUBLE, big::DataTypes::HALF };
            const auto &dataType = big.getDataType();
            for (uint64_t i = 0; i != dataType.size(); ++i) {
                Assert::AreEqual(testDataType[i], dataType[i]);
            }
            
            {
                big::BigCoreRead big("testCoreRead_getImage_DiffrentDataTypes1.big", false, 0);
                auto vec = big.getImage<uint16_t>(0);
                for (uint64_t index = 0; index != vec.size(); ++index) {
                    Assert::AreEqual(data1.get()[index], vec[index]);
                }
            
				auto vec2 = big.getImage<uint8_t>(1);
				for (uint64_t index = 0; index != vec2.size(); ++index) {
					Assert::AreEqual(data2.get()[index], vec2[index]);
				}
			
				auto vec3 = big.getImage<uint32_t>(2);
				for (uint64_t index = 0; index != vec3.size(); ++index) {
					Assert::AreEqual(data3.get()[index], vec3[index]);
				}
				auto vec4 = big.getImage<uint64_t>(3);
				for (uint64_t index = 0; index != vec4.size(); ++index) {
					Assert::AreEqual(data4.get()[index], vec4[index]);
				}
				auto vec6 = big.getImage<int8_t>(4);
				for (uint64_t index = 0; index != vec6.size(); ++index) {
					Assert::AreEqual(data5.get()[index], vec6[index]);
				}
				auto vec7 = big.getImage<int16_t>(5);
				for (uint64_t index = 0; index != vec7.size(); ++index) {
					Assert::AreEqual(data6.get()[index], vec7[index]);
				}
				auto vec8 = big.getImage<int32_t>(6);
				for (uint64_t index = 0; index != vec8.size(); ++index) {
					Assert::AreEqual(data7.get()[index], vec8[index]);
				}
				auto vec9 = big.getImage<int64_t>(7);
				for (uint64_t index = 0; index != vec9.size(); ++index) {
					Assert::AreEqual(data8.get()[index], vec9[index]);
				}
				auto vec10 = big.getImage<float>(8);
				for (uint64_t index = 0; index != vec10.size(); ++index) {
					Assert::AreEqual(data9.get()[index], vec10[index]);
				}
				auto vec11 = big.getImage<double>(9);
				for (uint64_t index = 0; index != vec11.size(); ++index) {
					Assert::AreEqual(data10.get()[index], vec11[index]);
				}
				auto vec12 = big.getImage<half>(10);
				for (uint64_t index = 0; index != vec12.size(); ++index) {
					Assert::AreEqual(data11.get()[index], vec12[index]);
				}
			}
			{
				big::BigCoreRead big("testCoreRead_getImage_DiffrentDataTypes1.big", true);
				auto vec = big.getImage<uint16_t>(0);
				for (uint64_t index = 0; index != vec.size(); ++index) {
					Assert::AreEqual(data1.get()[index], vec[index]);
				}

				auto vec2 = big.getImage<uint8_t>(1);
				for (uint64_t index = 0; index != vec2.size(); ++index) {
					Assert::AreEqual(data2.get()[index], vec2[index]);
				}

				auto vec3 = big.getImage<uint32_t>(2);
				for (uint64_t index = 0; index != vec3.size(); ++index) {
					Assert::AreEqual(data3.get()[index], vec3[index]);
				}
				auto vec4 = big.getImage<uint64_t>(3);
				for (uint64_t index = 0; index != vec4.size(); ++index) {
					Assert::AreEqual(data4.get()[index], vec4[index]);
				}
				auto vec6 = big.getImage<int8_t>(4);
				for (uint64_t index = 0; index != vec6.size(); ++index) {
					Assert::AreEqual(data5.get()[index], vec6[index]);
				}
				auto vec7 = big.getImage<int16_t>(5);
				for (uint64_t index = 0; index != vec7.size(); ++index) {
					Assert::AreEqual(data6.get()[index], vec7[index]);
				}
				auto vec8 = big.getImage<int32_t>(6);
				for (uint64_t index = 0; index != vec8.size(); ++index) {
					Assert::AreEqual(data7.get()[index], vec8[index]);
				}
				auto vec9 = big.getImage<int64_t>(7);
				for (uint64_t index = 0; index != vec9.size(); ++index) {
					Assert::AreEqual(data8.get()[index], vec9[index]);
				}
				auto vec10 = big.getImage<float>(8);
				for (uint64_t index = 0; index != vec10.size(); ++index) {
					Assert::AreEqual(data9.get()[index], vec10[index]);
				}
				auto vec11 = big.getImage<double>(9);
				for (uint64_t index = 0; index != vec11.size(); ++index) {
					Assert::AreEqual(data10.get()[index], vec11[index]);
				}
				auto vec12 = big.getImage<half>(10);
				for (uint64_t index = 0; index != vec12.size(); ++index) {
					Assert::AreEqual(data11.get()[index], vec12[index]);
				}
			}

        }
		TEST_METHOD(BigCoreRead_GetImage_without_cache_time_test) { //39 sec // size 1500 èas 0:04:45,2582933,  600 prvkù dvakrát rychlejší 0:00:47,1344158
			uint16_t n = 5 * 3 * 2;
			uint64_t data_test_size = DATA_TEST_SIZE2; //if change this value delete test file or comment if for creating data
			std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
			for (uint16_t i = 0; i != n; ++i) data1.get()[i] = i;
			std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
			for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
			std::shared_ptr<uint32_t> data3{ new uint32_t[n], [](uint32_t *p) {delete[] p; } };
			for (uint32_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint32_t>(n) + i;
			std::shared_ptr<uint64_t> data4{ new uint64_t[n], [](uint64_t *p) {delete[] p; } };
			for (uint64_t i = 0; i != n; ++i) data4.get()[i] = 2 * static_cast<uint64_t>(n) + i;
			std::shared_ptr<int8_t> data5{ new int8_t[n], [](int8_t *p) {delete[] p; } };
			for (int8_t i = 0; i != n; ++i) data5.get()[i] = 2 * static_cast<int8_t>(n) + i;
			std::shared_ptr<int16_t> data6{ new int16_t[n], [](int16_t *p) {delete[] p; } };
			for (int16_t i = 0; i != n; ++i) data6.get()[i] = 2 * static_cast<int16_t>(n) + i;
			std::shared_ptr<int32_t> data7{ new int32_t[n], [](int32_t *p) {delete[] p; } };
			for (int32_t i = 0; i != n; ++i) data7.get()[i] = 2 * static_cast<int32_t>(n) + i;
			std::shared_ptr<int64_t> data8{ new int64_t[n], [](int64_t *p) {delete[] p; } };
			for (int64_t i = 0; i != n; ++i) data8.get()[i] = 2 * static_cast<int64_t>(n) + i;
			std::shared_ptr<float> data9{ new float[n], [](float *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) data9.get()[i] = 2 * static_cast<float>(n) + i;
			std::shared_ptr<double> data10{ new double[n], [](double *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) data10.get()[i] = 2 * static_cast<double>(n) + i;
			std::shared_ptr<half> data11{ new half[n], [](half *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) { data11.get()[i] = 2 * static_cast<half> (n) + i; }
			uint64_t number_of_imadges = data_test_size*11;
		    //if(!exists_test("testCoreRead_getImage_cache_test1.big"))
			{
				big::BigCoreWrite big("testCoreRead_getImage_cache_test1.big", 2, 3, 5);
				uint64_t j = 0;
				while(j  < data_test_size) {
					big.pushEntity(data1, big::DataTypes::UINT16_T);
					big.pushEntity(data2, big::DataTypes::UINT8_T);
					big.pushEntity(data3, big::DataTypes::UINT32_T);
					big.pushEntity(data4, big::DataTypes::UINT64_T);
					big.pushEntity(data5, big::DataTypes::INT8_T);
					big.pushEntity(data6, big::DataTypes::INT16_T);
					big.pushEntity(data7, big::DataTypes::INT32_T);
					big.pushEntity(data8, big::DataTypes::INT64_T);
					big.pushEntity(data9, big::DataTypes::FLOAT);
					big.pushEntity(data10, big::DataTypes::DOUBLE);
					big.pushEntity(data11, big::DataTypes::HALF);
					j++;
				}
			}
			big::BigCoreRead big("testCoreRead_getImage_cache_test1.big", false, 0);
			Assert::AreEqual(number_of_imadges, big.getNumberOfImages());
			Assert::AreEqual(2ull, big.getImageHeight());
			Assert::AreEqual(3ull, big.getImageWidth());
			Assert::AreEqual(5ull, big.getNumberOfPlanes());
			const auto &dataOrder = big.getDataOrder();
			for (uint64_t i = 0; i != dataOrder.size(); ++i) {
				Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
			}
			std::vector<big::DataTypes> testDataType{ big::DataTypes::UINT16_T, big::DataTypes::UINT8_T, big::DataTypes::UINT32_T, big::DataTypes::UINT64_T, big::DataTypes::INT8_T,  big::DataTypes::INT16_T, big::DataTypes::INT32_T, big::DataTypes::INT64_T, big::DataTypes::FLOAT, big::DataTypes::DOUBLE, big::DataTypes::HALF };
			const auto &dataType = big.getDataType();
			for (uint64_t i = 0; i != dataType.size(); ++i) {
				Assert::AreEqual(testDataType[i%11], dataType[i]);
			}
			uint64_t i = 0;
			while(i< data_test_size * 11)
			{
				big::BigCoreRead big("testCoreRead_getImage_cache_test1.big", false, 0);
				auto vec = big.getImage<uint16_t>(i);
				/*for (uint64_t index = 0; index != vec.size(); ++index) {
					Assert::AreEqual(data1.get()[index], vec[index]);
				}*/
				i++;

				auto vec2 = big.getImage<uint8_t>(i);
				/*for (uint64_t index = 0; index != vec2.size(); ++index) {
					Assert::AreEqual(data2.get()[index], vec2[index]);
				}*/
				i++;

				auto vec3 = big.getImage<uint32_t>(i);
				/*for (uint64_t index = 0; index != vec3.size(); ++index) {
					Assert::AreEqual(data3.get()[index], vec3[index]);
				}*/
				i++;
				auto vec4 = big.getImage<uint64_t>(i);
				/*for (uint64_t index = 0; index != vec4.size(); ++index) {
					Assert::AreEqual(data4.get()[index], vec4[index]);
				}*/
				i++;
				auto vec6 = big.getImage<int8_t>(i);
				/*for (uint64_t index = 0; index != vec6.size(); ++index) {
					Assert::AreEqual(data5.get()[index], vec6[index]);
				}*/
				i++;
				auto vec7 = big.getImage<int16_t>(i);
				/*for (uint64_t index = 0; index != vec7.size(); ++index) {
					Assert::AreEqual(data6.get()[index], vec7[index]);
				}*/
				i++;
				auto vec8 = big.getImage<int32_t>(i);
				/*for (uint64_t index = 0; index != vec8.size(); ++index) {
					Assert::AreEqual(data7.get()[index], vec8[index]);
				}*/
				i++;
				auto vec9 = big.getImage<int64_t>(i);
				/*for (uint64_t index = 0; index != vec9.size(); ++index) {
					Assert::AreEqual(data8.get()[index], vec9[index]);
				}*/
				i++;
				auto vec10 = big.getImage<float>(i);
				/*for (uint64_t index = 0; index != vec10.size(); ++index) {
					Assert::AreEqual(data9.get()[index], vec10[index]);
				}*/
				i++;
				auto vec11 = big.getImage<double>(i);
				/*for (uint64_t index = 0; index != vec11.size(); ++index) {
					Assert::AreEqual(data10.get()[index], vec11[index]);
				}*/
				i++;
				auto vec12 = big.getImage<half>(i);
				/*for (uint64_t index = 0; index != vec12.size(); ++index) {
					Assert::AreEqual(data11.get()[index], vec12[index]);
				}*/
				i++;

				auto vec13 = big.getImage<double>(9);
				/*for (uint64_t index = 0; index != vec13.size(); ++index) {
					Assert::AreEqual(data10.get()[index], vec13[index]);
				}*/
				vec13 = big.getImage<double>(9 * i % 3);
				vec13 = big.getImage<double>(9 * i % 6);
				vec13 = big.getImage<double>(9 * i % 5);
				vec13 = big.getImage<double>(9 * i % 4);
				vec13 = big.getImage<double>(9 * i % 12);
				vec13 = big.getImage<double>(9 * i % 7);
				vec13 = big.getImage<double>(9 * i % 8);
				vec13 = big.getImage<double>(9 * i % 13);
				auto vec14 = big.getImage<float>(8);
				/*for (uint64_t index = 0; index != vec14.size(); ++index) {
					Assert::AreEqual(data9.get()[index], vec14[index]);
				}*/
			}
		}
		TEST_METHOD(BigCoreRead_GetImage_with_cache_time_test) { //5000 -51minut //1500 cache 1000 - èas 4:42,0933288, 600 cache 1000000 - 0:01:30,9463438 o polovièku pomalejší
			uint16_t n = 5 * 3 * 2;
			uint64_t data_test_size = DATA_TEST_SIZE2; //if change this value delete test file or comment if for creating data file contain 11*more  images
			std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
			for (uint16_t i = 0; i != n; ++i) data1.get()[i] = i;
			std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
			for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
			std::shared_ptr<uint32_t> data3{ new uint32_t[n], [](uint32_t *p) {delete[] p; } };
			for (uint32_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint32_t>(n) + i;
			std::shared_ptr<uint64_t> data4{ new uint64_t[n], [](uint64_t *p) {delete[] p; } };
			for (uint64_t i = 0; i != n; ++i) data4.get()[i] = 2 * static_cast<uint64_t>(n) + i;
			std::shared_ptr<int8_t> data5{ new int8_t[n], [](int8_t *p) {delete[] p; } };
			for (int8_t i = 0; i != n; ++i) data5.get()[i] = 2 * static_cast<int8_t>(n) + i;
			std::shared_ptr<int16_t> data6{ new int16_t[n], [](int16_t *p) {delete[] p; } };
			for (int16_t i = 0; i != n; ++i) data6.get()[i] = 2 * static_cast<int16_t>(n) + i;
			std::shared_ptr<int32_t> data7{ new int32_t[n], [](int32_t *p) {delete[] p; } };
			for (int32_t i = 0; i != n; ++i) data7.get()[i] = 2 * static_cast<int32_t>(n) + i;
			std::shared_ptr<int64_t> data8{ new int64_t[n], [](int64_t *p) {delete[] p; } };
			for (int64_t i = 0; i != n; ++i) data8.get()[i] = 2 * static_cast<int64_t>(n) + i;
			std::shared_ptr<float> data9{ new float[n], [](float *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) data9.get()[i] = 2 * static_cast<float>(n) + i;
			std::shared_ptr<double> data10{ new double[n], [](double *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) data10.get()[i] = 2 * static_cast<double>(n) + i;
			std::shared_ptr<half> data11{ new half[n], [](half *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) { data11.get()[i] = 2 * static_cast<half> (n) + i; }
			uint64_t number_of_imadges = data_test_size * 11;
			//if (!exists_test("testCoreRead_getImage_cache_test1.big"))
			{
				big::BigCoreWrite big("testCoreRead_getImage_cache_test1.big", 2, 3, 5);
				uint64_t j = 0;
				while (j < data_test_size) {
					big.pushEntity(data1, big::DataTypes::UINT16_T);
					big.pushEntity(data2, big::DataTypes::UINT8_T);
					big.pushEntity(data3, big::DataTypes::UINT32_T);
					big.pushEntity(data4, big::DataTypes::UINT64_T);
					big.pushEntity(data5, big::DataTypes::INT8_T);
					big.pushEntity(data6, big::DataTypes::INT16_T);
					big.pushEntity(data7, big::DataTypes::INT32_T);
					big.pushEntity(data8, big::DataTypes::INT64_T);
					big.pushEntity(data9, big::DataTypes::FLOAT);
					big.pushEntity(data10, big::DataTypes::DOUBLE);
					big.pushEntity(data11, big::DataTypes::HALF);
					j++;
				}
			}
			big::BigCoreRead big("testCoreRead_getImage_cache_test1.big", false, 0);
			Assert::AreEqual(number_of_imadges, big.getNumberOfImages());
			Assert::AreEqual(2ull, big.getImageHeight());
			Assert::AreEqual(3ull, big.getImageWidth());
			Assert::AreEqual(5ull, big.getNumberOfPlanes());
			const auto &dataOrder = big.getDataOrder();
			for (uint64_t i = 0; i != dataOrder.size(); ++i) {
				Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
			}
			std::vector<big::DataTypes> testDataType{ big::DataTypes::UINT16_T, big::DataTypes::UINT8_T, big::DataTypes::UINT32_T, big::DataTypes::UINT64_T, big::DataTypes::INT8_T,  big::DataTypes::INT16_T, big::DataTypes::INT32_T, big::DataTypes::INT64_T, big::DataTypes::FLOAT, big::DataTypes::DOUBLE, big::DataTypes::HALF };
			const auto &dataType = big.getDataType();
			for (uint64_t i = 0; i != dataType.size(); ++i) {
				Assert::AreEqual(testDataType[i % 11], dataType[i]);
			}
			uint64_t i = 0;
			while (i < data_test_size*11)
			{
				big::BigCoreRead big("testCoreRead_getImage_cache_test1.big", false, CACHE_SIZE);
				auto vec = big.getImage<uint16_t>(i);
				/*for (uint64_t index = 0; index != vec.size(); ++index) {
					Assert::AreEqual(data1.get()[index], vec[index]);
				}*/
				i++;

				auto vec2 = big.getImage<uint8_t>(i);
				/*for (uint64_t index = 0; index != vec2.size(); ++index) {
					Assert::AreEqual(data2.get()[index], vec2[index]);
				}*/
				i++;

				auto vec3 = big.getImage<uint32_t>(i);
				/*for (uint64_t index = 0; index != vec3.size(); ++index) {
					Assert::AreEqual(data3.get()[index], vec3[index]);
				}*/
				i++;
				auto vec4 = big.getImage<uint64_t>(i);
				/*for (uint64_t index = 0; index != vec4.size(); ++index) {
					Assert::AreEqual(data4.get()[index], vec4[index]);
				}*/
				i++;
				auto vec6 = big.getImage<int8_t>(i);
				/*for (uint64_t index = 0; index != vec6.size(); ++index) {
					Assert::AreEqual(data5.get()[index], vec6[index]);
				}*/
				i++;
				auto vec7 = big.getImage<int16_t>(i);
				/*for (uint64_t index = 0; index != vec7.size(); ++index) {
					Assert::AreEqual(data6.get()[index], vec7[index]);
				}*/
				i++;
				auto vec8 = big.getImage<int32_t>(i);
				/*for (uint64_t index = 0; index != vec8.size(); ++index) {
					Assert::AreEqual(data7.get()[index], vec8[index]);
				}*/
				i++;
				auto vec9 = big.getImage<int64_t>(i);
				/*for (uint64_t index = 0; index != vec9.size(); ++index) {
					Assert::AreEqual(data8.get()[index], vec9[index]);
				}*/
				i++;
				auto vec10 = big.getImage<float>(i);
				/*for (uint64_t index = 0; index != vec10.size(); ++index) {
					Assert::AreEqual(data9.get()[index], vec10[index]);
				}*/
				i++;
				auto vec11 = big.getImage<double>(i);
				/*for (uint64_t index = 0; index != vec11.size(); ++index) {
					Assert::AreEqual(data10.get()[index], vec11[index]);
				}*/
				i++;
				auto vec12 = big.getImage<half>(i);
				/*for (uint64_t index = 0; index != vec12.size(); ++index) {
					Assert::AreEqual(data11.get()[index], vec12[index]);
				}*/
				i++;

				auto vec13 = big.getImage<double>(9);
				 vec13 = big.getImage<double>(9 * i % 3);
				 vec13 = big.getImage<double>(9 * i % 6);
				 vec13 = big.getImage<double>(9 * i % 5);
				 vec13 = big.getImage<double>(9 * i % 4);
				 vec13 = big.getImage<double>(9 * i % 12);
				 vec13 = big.getImage<double>(9 *  i % 7);
				 vec13 = big.getImage<double>(9 * i % 8);
				 vec13 = big.getImage<double>(9 * i%13);
				/*for (uint64_t index = 0; index != vec13.size(); ++index) {
					Assert::AreEqual(data10.get()[index], vec13[index]);
				}*/
				auto vec14 = big.getImage<float>(8);
				/*for (uint64_t index = 0; index != vec14.size(); ++index) {
					Assert::AreEqual(data9.get()[index], vec14[index]);
				}*/
			}
		}
		TEST_METHOD(BigCoreRead_Random_read_Cache) { 
			//pøi 7500*11 obrázcích a 45000 a velikostí cahce 500 000 je os 1s rychlejší,
			//pokud ale cachce pøeteèe, tak hroznì zpomalí pøi 150000 èteních, s cache 12000000 je o 3-4s rychlejší tedy 0:00:23,4509953, 0:00:22,5036908 než možnost bez cache, ale pokud je cache moc malá tak je velmi pomalejší
			uint16_t n = 5 * 3 * 2;
			uint64_t data_test_size = DATA_TEST_SIZE; //if change this value delete test file or comment if for creating data
			std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
			for (uint16_t i = 0; i != n; ++i) data1.get()[i] = i;
			std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
			for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
			std::shared_ptr<uint32_t> data3{ new uint32_t[n], [](uint32_t *p) {delete[] p; } };
			for (uint32_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint32_t>(n) + i;
			std::shared_ptr<uint64_t> data4{ new uint64_t[n], [](uint64_t *p) {delete[] p; } };
			for (uint64_t i = 0; i != n; ++i) data4.get()[i] = 2 * static_cast<uint64_t>(n) + i;
			std::shared_ptr<int8_t> data5{ new int8_t[n], [](int8_t *p) {delete[] p; } };
			for (int8_t i = 0; i != n; ++i) data5.get()[i] = 2 * static_cast<int8_t>(n) + i;
			std::shared_ptr<int16_t> data6{ new int16_t[n], [](int16_t *p) {delete[] p; } };
			for (int16_t i = 0; i != n; ++i) data6.get()[i] = 2 * static_cast<int16_t>(n) + i;
			std::shared_ptr<int32_t> data7{ new int32_t[n], [](int32_t *p) {delete[] p; } };
			for (int32_t i = 0; i != n; ++i) data7.get()[i] = 2 * static_cast<int32_t>(n) + i;
			std::shared_ptr<int64_t> data8{ new int64_t[n], [](int64_t *p) {delete[] p; } };
			for (int64_t i = 0; i != n; ++i) data8.get()[i] = 2 * static_cast<int64_t>(n) + i;
			std::shared_ptr<float> data9{ new float[n], [](float *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) data9.get()[i] = 2 * static_cast<float>(n) + i;
			std::shared_ptr<double> data10{ new double[n], [](double *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) data10.get()[i] = 2 * static_cast<double>(n) + i;
			std::shared_ptr<half> data11{ new half[n], [](half *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) { data11.get()[i] = 2 * static_cast<half> (n) + i; }
			uint64_t number_of_imadges = data_test_size * 11;
			//if (!exists_test("testCoreRead_randomRead_cache_test1.big"))
			{
				big::BigCoreWrite big("testCoreRead_randomRead_cache_test1.big", 2, 3, 5);
				uint64_t j = 0;
				while (j < data_test_size) {
					big.pushEntity(data1, big::DataTypes::UINT16_T);
					big.pushEntity(data2, big::DataTypes::UINT8_T);
					big.pushEntity(data3, big::DataTypes::UINT32_T);
					big.pushEntity(data4, big::DataTypes::UINT64_T);
					big.pushEntity(data5, big::DataTypes::INT8_T);
					big.pushEntity(data6, big::DataTypes::INT16_T);
					big.pushEntity(data7, big::DataTypes::INT32_T);
					big.pushEntity(data8, big::DataTypes::INT64_T);
					big.pushEntity(data9, big::DataTypes::FLOAT);
					big.pushEntity(data10, big::DataTypes::DOUBLE);
					big.pushEntity(data11, big::DataTypes::HALF);
					j++;
				}
			}
			big::BigCoreRead big("testCoreRead_randomRead_cache_test1.big", false, CACHE_SIZE);
			Assert::AreEqual(number_of_imadges, big.getNumberOfImages());
			Assert::AreEqual(2ull, big.getImageHeight());
			Assert::AreEqual(3ull, big.getImageWidth());
			Assert::AreEqual(5ull, big.getNumberOfPlanes());
			const auto &dataOrder = big.getDataOrder();
			for (uint64_t i = 0; i != dataOrder.size(); ++i) {
				Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
			}
			std::vector<big::DataTypes> testDataType{ big::DataTypes::UINT16_T, big::DataTypes::UINT8_T, big::DataTypes::UINT32_T, big::DataTypes::UINT64_T, big::DataTypes::INT8_T,  big::DataTypes::INT16_T, big::DataTypes::INT32_T, big::DataTypes::INT64_T, big::DataTypes::FLOAT, big::DataTypes::DOUBLE, big::DataTypes::HALF };
			const auto &dataType = big.getDataType();
			for (uint64_t i = 0; i != dataType.size(); ++i) {
				Assert::AreEqual(testDataType[i % 11], dataType[i]);
			}
			uint64_t i = 0;

			std::random_device rd;
			std::mt19937_64 gen(rd());
			std::uniform_int_distribution<uint64_t> dis(0, number_of_imadges-12);
			while (i < NUMBER_OF_READS)
			{
				uint64_t index = dis(gen);
				switch (big.getEntityDataType(index))
				{
					case big::DataTypes::UINT8_T:
					{
						auto vec13 = big.getImage<uint8_t>(index);
						break;

					}
					case big::DataTypes::UINT16_T:
					{
						auto vec13 = big.getImage<uint16_t>(index);
						break;
					}
					case big::DataTypes::UINT32_T:
					{
						auto vec13 = big.getImage<uint32_t>(index);
						break;
					}
					case big::DataTypes::UINT64_T:
					{
						auto vec13 = big.getImage<uint64_t>(index);
						break;
					}
					case big::DataTypes::INT8_T:
					{
						auto vec13 = big.getImage<int8_t>(index);
						break;
					}
					case big::DataTypes::INT16_T:
					{
						auto vec13 = big.getImage<int16_t>(index);
						break;
					}
					case big::DataTypes::INT32_T:
					{
						auto vec13 = big.getImage<int32_t>(index);
						break;
					}
					case big::DataTypes::INT64_T:
					{
						auto vec13 = big.getImage<int64_t>(index);
						break;
					}
					case big::DataTypes::HALF:
					{
						auto vec13 = big.getImage<half>(index);
						break;

					}
					case big::DataTypes::FLOAT:
					{
						auto vec13 = big.getImage<float>(index);
						break;
					}
					case big::DataTypes::DOUBLE:
					{
						auto vec13 = big.getImage<double>(index);
						break;
					}
				}
				i++;

			}


		}

		TEST_METHOD(BigCoreRead_Random_read_without_cache) { 
			uint16_t n = 5 * 3 * 2;
			uint64_t data_test_size = DATA_TEST_SIZE; //if change this value delete test file or comment if for creating data
			std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
			for (uint16_t i = 0; i != n; ++i) data1.get()[i] = i;
			std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
			for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
			std::shared_ptr<uint32_t> data3{ new uint32_t[n], [](uint32_t *p) {delete[] p; } };
			for (uint32_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint32_t>(n) + i;
			std::shared_ptr<uint64_t> data4{ new uint64_t[n], [](uint64_t *p) {delete[] p; } };
			for (uint64_t i = 0; i != n; ++i) data4.get()[i] = 2 * static_cast<uint64_t>(n) + i;
			std::shared_ptr<int8_t> data5{ new int8_t[n], [](int8_t *p) {delete[] p; } };
			for (int8_t i = 0; i != n; ++i) data5.get()[i] = 2 * static_cast<int8_t>(n) + i;
			std::shared_ptr<int16_t> data6{ new int16_t[n], [](int16_t *p) {delete[] p; } };
			for (int16_t i = 0; i != n; ++i) data6.get()[i] = 2 * static_cast<int16_t>(n) + i;
			std::shared_ptr<int32_t> data7{ new int32_t[n], [](int32_t *p) {delete[] p; } };
			for (int32_t i = 0; i != n; ++i) data7.get()[i] = 2 * static_cast<int32_t>(n) + i;
			std::shared_ptr<int64_t> data8{ new int64_t[n], [](int64_t *p) {delete[] p; } };
			for (int64_t i = 0; i != n; ++i) data8.get()[i] = 2 * static_cast<int64_t>(n) + i;
			std::shared_ptr<float> data9{ new float[n], [](float *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) data9.get()[i] = 2 * static_cast<float>(n) + i;
			std::shared_ptr<double> data10{ new double[n], [](double *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) data10.get()[i] = 2 * static_cast<double>(n) + i;
			std::shared_ptr<half> data11{ new half[n], [](half *p) {delete[] p; } };
			for (int i = 0; i != n; ++i) { data11.get()[i] = 2 * static_cast<half> (n) + i; }
			uint64_t number_of_imadges = data_test_size * 11;
			//if (!exists_test("testCoreRead_randomRead_cache_test1.big"))
			{
				big::BigCoreWrite big("testCoreRead_randomRead_cache_test1.big", 2, 3, 5);
				uint64_t j = 0;
				while (j < data_test_size) {
					big.pushEntity(data1, big::DataTypes::UINT16_T);
					big.pushEntity(data2, big::DataTypes::UINT8_T);
					big.pushEntity(data3, big::DataTypes::UINT32_T);
					big.pushEntity(data4, big::DataTypes::UINT64_T);
					big.pushEntity(data5, big::DataTypes::INT8_T);
					big.pushEntity(data6, big::DataTypes::INT16_T);
					big.pushEntity(data7, big::DataTypes::INT32_T);
					big.pushEntity(data8, big::DataTypes::INT64_T);
					big.pushEntity(data9, big::DataTypes::FLOAT);
					big.pushEntity(data10, big::DataTypes::DOUBLE);
					big.pushEntity(data11, big::DataTypes::HALF);
					j++;
				}
			}
			big::BigCoreRead big("testCoreRead_randomRead_cache_test1.big", false, 0);
			Assert::AreEqual(number_of_imadges, big.getNumberOfImages());
			Assert::AreEqual(2ull, big.getImageHeight());
			Assert::AreEqual(3ull, big.getImageWidth());
			Assert::AreEqual(5ull, big.getNumberOfPlanes());
			const auto &dataOrder = big.getDataOrder();
			for (uint64_t i = 0; i != dataOrder.size(); ++i) {
				Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
			}
			std::vector<big::DataTypes> testDataType{ big::DataTypes::UINT16_T, big::DataTypes::UINT8_T, big::DataTypes::UINT32_T, big::DataTypes::UINT64_T, big::DataTypes::INT8_T,  big::DataTypes::INT16_T, big::DataTypes::INT32_T, big::DataTypes::INT64_T, big::DataTypes::FLOAT, big::DataTypes::DOUBLE, big::DataTypes::HALF };
			const auto &dataType = big.getDataType();
			for (uint64_t i = 0; i != dataType.size(); ++i) {
				Assert::AreEqual(testDataType[i % 11], dataType[i]);
			}
			uint64_t i = 0;
			std::random_device rd;
			std::mt19937_64 gen(rd());
			std::uniform_int_distribution<uint64_t> dis(0, number_of_imadges - 12);
			while (i < NUMBER_OF_READS)
			{
				uint64_t index = dis(gen);
				switch (big.getEntityDataType(index))
				{
				case big::DataTypes::UINT8_T:
				{
					auto vec13 = big.getImage<uint8_t>(index);
					break;

				}
				case big::DataTypes::UINT16_T:
				{
					auto vec13 = big.getImage<uint16_t>(index);
					break;
				}
				case big::DataTypes::UINT32_T:
				{
					auto vec13 = big.getImage<uint32_t>(index);
					break;
				}
				case big::DataTypes::UINT64_T:
				{
					auto vec13 = big.getImage<uint64_t>(index);
					break;
				}
				case big::DataTypes::INT8_T:
				{
					auto vec13 = big.getImage<int8_t>(index);
					break;
				}
				case big::DataTypes::INT16_T:
				{
					auto vec13 = big.getImage<int16_t>(index);
					break;
				}
				case big::DataTypes::INT32_T:
				{
					auto vec13 = big.getImage<int32_t>(index);
					break;
				}
				case big::DataTypes::INT64_T:
				{
					auto vec13 = big.getImage<int64_t>(index);
					break;
				}
				case big::DataTypes::HALF:
				{
					auto vec13 = big.getImage<half>(index);
					break;

				}
				case big::DataTypes::FLOAT:
				{
					auto vec13 = big.getImage<float>(index);
					break;
				}
				case big::DataTypes::DOUBLE:
				{
					auto vec13 = big.getImage<double>(index);
					break;
				}
				}
				i++;

			}


		}
        TEST_METHOD(BigCoreRead_Random_at_without_cache) {
            uint16_t n = 5 * 3 * 2;
            uint64_t data_test_size = DATA_TEST_SIZE; //if change this value delete test file or comment if for creating data
            std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
            for (uint16_t i = 0; i != n; ++i) data1.get()[i] = i;
            std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
            for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
            std::shared_ptr<uint32_t> data3{ new uint32_t[n], [](uint32_t *p) {delete[] p; } };
            for (uint32_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint32_t>(n) + i;
            std::shared_ptr<uint64_t> data4{ new uint64_t[n], [](uint64_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data4.get()[i] = 2 * static_cast<uint64_t>(n) + i;
            std::shared_ptr<int8_t> data5{ new int8_t[n], [](int8_t *p) {delete[] p; } };
            for (int8_t i = 0; i != n; ++i) data5.get()[i] = 2 * static_cast<int8_t>(n) + i;
            std::shared_ptr<int16_t> data6{ new int16_t[n], [](int16_t *p) {delete[] p; } };
            for (int16_t i = 0; i != n; ++i) data6.get()[i] = 2 * static_cast<int16_t>(n) + i;
            std::shared_ptr<int32_t> data7{ new int32_t[n], [](int32_t *p) {delete[] p; } };
            for (int32_t i = 0; i != n; ++i) data7.get()[i] = 2 * static_cast<int32_t>(n) + i;
            std::shared_ptr<int64_t> data8{ new int64_t[n], [](int64_t *p) {delete[] p; } };
            for (int64_t i = 0; i != n; ++i) data8.get()[i] = 2 * static_cast<int64_t>(n) + i;
            std::shared_ptr<float> data9{ new float[n], [](float *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) data9.get()[i] = 2 * static_cast<float>(n) + i;
            std::shared_ptr<double> data10{ new double[n], [](double *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) data10.get()[i] = 2 * static_cast<double>(n) + i;
            std::shared_ptr<half> data11{ new half[n], [](half *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) { data11.get()[i] = 2 * static_cast<half> (n) + i; }
            uint64_t number_of_imadges = data_test_size * 11;
            //if (!exists_test("testCoreRead_random_at_cache_test1.big"))
            {
                big::BigCoreWrite big("testCoreRead_random_at_cache_test1.big", 2, 3, 5);
                uint64_t j = 0;
                while (j < data_test_size) {
                    big.pushEntity(data1, big::DataTypes::UINT16_T);
                    big.pushEntity(data2, big::DataTypes::UINT8_T);
                    big.pushEntity(data3, big::DataTypes::UINT32_T);
                    big.pushEntity(data4, big::DataTypes::UINT64_T);
                    big.pushEntity(data5, big::DataTypes::INT8_T);
                    big.pushEntity(data6, big::DataTypes::INT16_T);
                    big.pushEntity(data7, big::DataTypes::INT32_T);
                    big.pushEntity(data8, big::DataTypes::INT64_T);
                    big.pushEntity(data9, big::DataTypes::FLOAT);
                    big.pushEntity(data10, big::DataTypes::DOUBLE);
                    big.pushEntity(data11, big::DataTypes::HALF);
                    j++;
                }
            }
            big::BigCoreRead big("testCoreRead_random_at_cache_test1.big", false, 0);
            Assert::AreEqual(number_of_imadges, big.getNumberOfImages());
            Assert::AreEqual(2ull, big.getImageHeight());
            Assert::AreEqual(3ull, big.getImageWidth());
            Assert::AreEqual(5ull, big.getNumberOfPlanes());
            const auto &dataOrder = big.getDataOrder();
            for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
            }
            std::vector<big::DataTypes> testDataType{ big::DataTypes::UINT16_T, big::DataTypes::UINT8_T, big::DataTypes::UINT32_T, big::DataTypes::UINT64_T, big::DataTypes::INT8_T,  big::DataTypes::INT16_T, big::DataTypes::INT32_T, big::DataTypes::INT64_T, big::DataTypes::FLOAT, big::DataTypes::DOUBLE, big::DataTypes::HALF };
            const auto &dataType = big.getDataType();
            for (uint64_t i = 0; i != dataType.size(); ++i) {
                Assert::AreEqual(testDataType[i % 11], dataType[i]);
            }
            uint64_t i = 0;
            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::uniform_int_distribution<uint64_t> dis(0, number_of_imadges - 12);
            std::uniform_int_distribution<uint64_t> dis1(0, big.getImageHeight()-1);
            std::uniform_int_distribution<uint64_t> dis2(0, big.getImageWidth()-1);
            std::uniform_int_distribution<uint64_t> dis3(0, big.getNumberOfPlanes()-1);
           
            while (i < NUMBER_OF_READS)
            {
                uint64_t imageNum = dis(gen);
                uint64_t rowIndex = dis1(gen);
                uint64_t colIndex = dis2(gen);
                uint64_t numberOfPlanes = dis3(gen);
                switch (big.getEntityDataType(imageNum))
                {
                case big::DataTypes::UINT8_T:
                {
                    auto point = big.at<uint8_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;

                }
                case big::DataTypes::UINT16_T:
                {
                    auto point = big.at<uint16_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::UINT32_T:
                {
                    auto point = big.at<uint32_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::UINT64_T:
                {
                    auto point = big.at<uint64_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT8_T:
                {
                    auto point = big.at<int8_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT16_T:
                {
                    auto point = big.at<int16_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT32_T:
                {
                    auto point = big.at<int32_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT64_T:
                {
                    auto point = big.at<int64_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::HALF:
                {
                    auto point = big.at<half>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;

                }
                case big::DataTypes::FLOAT:
                {
                    auto point = big.at<float>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::DOUBLE:
                {
                    auto point = big.at<double>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                }
                i++;

            }


        }
        TEST_METHOD(BigCoreRead_Random_at_with_cache) {
            uint16_t n = 5 * 3 * 2;
            uint64_t data_test_size = DATA_TEST_SIZE; //if change this value delete test file or comment if for creating data
            std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
            for (uint16_t i = 0; i != n; ++i) data1.get()[i] = i;
            std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
            for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
            std::shared_ptr<uint32_t> data3{ new uint32_t[n], [](uint32_t *p) {delete[] p; } };
            for (uint32_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint32_t>(n) + i;
            std::shared_ptr<uint64_t> data4{ new uint64_t[n], [](uint64_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data4.get()[i] = 2 * static_cast<uint64_t>(n) + i;
            std::shared_ptr<int8_t> data5{ new int8_t[n], [](int8_t *p) {delete[] p; } };
            for (int8_t i = 0; i != n; ++i) data5.get()[i] = 2 * static_cast<int8_t>(n) + i;
            std::shared_ptr<int16_t> data6{ new int16_t[n], [](int16_t *p) {delete[] p; } };
            for (int16_t i = 0; i != n; ++i) data6.get()[i] = 2 * static_cast<int16_t>(n) + i;
            std::shared_ptr<int32_t> data7{ new int32_t[n], [](int32_t *p) {delete[] p; } };
            for (int32_t i = 0; i != n; ++i) data7.get()[i] = 2 * static_cast<int32_t>(n) + i;
            std::shared_ptr<int64_t> data8{ new int64_t[n], [](int64_t *p) {delete[] p; } };
            for (int64_t i = 0; i != n; ++i) data8.get()[i] = 2 * static_cast<int64_t>(n) + i;
            std::shared_ptr<float> data9{ new float[n], [](float *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) data9.get()[i] = 2 * static_cast<float>(n) + i;
            std::shared_ptr<double> data10{ new double[n], [](double *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) data10.get()[i] = 2 * static_cast<double>(n) + i;
            std::shared_ptr<half> data11{ new half[n], [](half *p) {delete[] p; } };
            for (int i = 0; i != n; ++i) { data11.get()[i] = 2 * static_cast<half> (n) + i; }
            uint64_t number_of_imadges = data_test_size * 11;
            //if (!exists_test("testCoreRead_random_at_cache_test1.big"))
            {
                big::BigCoreWrite big("testCoreRead_random_at_cache_test1.big", 2, 3, 5);
                uint64_t j = 0;
                while (j < data_test_size) {
                    big.pushEntity(data1, big::DataTypes::UINT16_T);
                    big.pushEntity(data2, big::DataTypes::UINT8_T);
                    big.pushEntity(data3, big::DataTypes::UINT32_T);
                    big.pushEntity(data4, big::DataTypes::UINT64_T);
                    big.pushEntity(data5, big::DataTypes::INT8_T);
                    big.pushEntity(data6, big::DataTypes::INT16_T);
                    big.pushEntity(data7, big::DataTypes::INT32_T);
                    big.pushEntity(data8, big::DataTypes::INT64_T);
                    big.pushEntity(data9, big::DataTypes::FLOAT);
                    big.pushEntity(data10, big::DataTypes::DOUBLE);
                    big.pushEntity(data11, big::DataTypes::HALF);
                    j++;
                }
            }
            big::BigCoreRead big("testCoreRead_random_at_cache_test1.big", false, CACHE_SIZE);
            Assert::AreEqual(number_of_imadges, big.getNumberOfImages());
            Assert::AreEqual(2ull, big.getImageHeight());
            Assert::AreEqual(3ull, big.getImageWidth());
            Assert::AreEqual(5ull, big.getNumberOfPlanes());
            const auto &dataOrder = big.getDataOrder();
            for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
            }
            std::vector<big::DataTypes> testDataType{ big::DataTypes::UINT16_T, big::DataTypes::UINT8_T, big::DataTypes::UINT32_T, big::DataTypes::UINT64_T, big::DataTypes::INT8_T,  big::DataTypes::INT16_T, big::DataTypes::INT32_T, big::DataTypes::INT64_T, big::DataTypes::FLOAT, big::DataTypes::DOUBLE, big::DataTypes::HALF };
            const auto &dataType = big.getDataType();
            for (uint64_t i = 0; i != dataType.size(); ++i) {
                Assert::AreEqual(testDataType[i % 11], dataType[i]);
            }
            uint64_t i = 0;
            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::uniform_int_distribution<uint64_t> dis(0, number_of_imadges - 12);
            std::uniform_int_distribution<uint64_t> dis1(0, big.getImageHeight() - 1);
            std::uniform_int_distribution<uint64_t> dis2(0, big.getImageWidth() - 1);
            std::uniform_int_distribution<uint64_t> dis3(0, big.getNumberOfPlanes() - 1);

            while (i < NUMBER_OF_READS)
            {
                uint64_t imageNum = dis(gen);
                uint64_t rowIndex = dis1(gen);
                uint64_t colIndex = dis2(gen);
                uint64_t numberOfPlanes = dis3(gen);
                switch (big.getEntityDataType(imageNum))
                {
                case big::DataTypes::UINT8_T:
                {
                    auto point = big.at<uint8_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;

                }
                case big::DataTypes::UINT16_T:
                {
                    auto point = big.at<uint16_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::UINT32_T:
                {
                    auto point = big.at<uint32_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::UINT64_T:
                {
                    auto point = big.at<uint64_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT8_T:
                {
                    auto point = big.at<int8_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT16_T:
                {
                    auto point = big.at<int16_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT32_T:
                {
                    auto point = big.at<int32_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT64_T:
                {
                    auto point = big.at<int64_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::HALF:
                {
                    auto point = big.at<half>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;

                }
                case big::DataTypes::FLOAT:
                {
                    auto point = big.at<float>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::DOUBLE:
                {
                    auto point = big.at<double>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                }
                i++;
                

            }


        }
        //test random read with normal distribution of data read and bigger images tests
        TEST_METHOD(BigCoreRead_Random_at_without_cache_normal_dist) {
            uint16_t n = 3 *20 * 10;
            uint64_t data_test_size = DATA_TEST_SIZE; //if change this value delete test file or comment if for creating data
            std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data1.get()[i] = i;
            std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
            std::shared_ptr<uint32_t> data3{ new uint32_t[n], [](uint32_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint32_t>(n) + i;
            std::shared_ptr<uint64_t> data4{ new uint64_t[n], [](uint64_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data4.get()[i] = 2 * static_cast<uint64_t>(n) + i;
            std::shared_ptr<int8_t> data5{ new int8_t[n], [](int8_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data5.get()[i] = 2 * static_cast<int8_t>(n) + i;
            std::shared_ptr<int16_t> data6{ new int16_t[n], [](int16_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data6.get()[i] = 2 * static_cast<int16_t>(n) + i;
            std::shared_ptr<int32_t> data7{ new int32_t[n], [](int32_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data7.get()[i] = 2 * static_cast<int32_t>(n) + i;
            std::shared_ptr<int64_t> data8{ new int64_t[n], [](int64_t *p) {delete[] p; } };
            for (int64_t i = 0; i != n; ++i) data8.get()[i] = 2 * static_cast<int64_t>(n) + i;
            std::shared_ptr<float> data9{ new float[n], [](float *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data9.get()[i] = 2 * static_cast<float>(n) + i;
            std::shared_ptr<double> data10{ new double[n], [](double *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data10.get()[i] = 2 * static_cast<double>(n) + i;
            std::shared_ptr<half> data11{ new half[n], [](half *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) { data11.get()[i] = 2 * static_cast<half> (n) + i; }
            uint64_t number_of_imadges = data_test_size * 11;
            //if (!exists_test("testCoreRead_random_at_cache_test1.big"))
            {
                big::BigCoreWrite big("testCoreRead_random_at_cache_test2_normal.big", 10, 20, 3);
                uint64_t j = 0;
                while (j < data_test_size) {
                    big.pushEntity(data1, big::DataTypes::UINT16_T);
                    big.pushEntity(data2, big::DataTypes::UINT8_T);
                    big.pushEntity(data3, big::DataTypes::UINT32_T);
                    big.pushEntity(data4, big::DataTypes::UINT64_T);
                    big.pushEntity(data5, big::DataTypes::INT8_T);
                    big.pushEntity(data6, big::DataTypes::INT16_T);
                    big.pushEntity(data7, big::DataTypes::INT32_T);
                    big.pushEntity(data8, big::DataTypes::INT64_T);
                    big.pushEntity(data9, big::DataTypes::FLOAT);
                    big.pushEntity(data10, big::DataTypes::DOUBLE);
                    big.pushEntity(data11, big::DataTypes::HALF);
                    j++;
                }
            }
            big::BigCoreRead big("testCoreRead_random_at_cache_test2_normal.big", false, 0);
            Assert::AreEqual(number_of_imadges, big.getNumberOfImages());
            Assert::AreEqual(10ull, big.getImageHeight());
            Assert::AreEqual(20ull, big.getImageWidth());
            Assert::AreEqual(3ull, big.getNumberOfPlanes());
            const auto &dataOrder = big.getDataOrder();
            for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
            }
            std::vector<big::DataTypes> testDataType{ big::DataTypes::UINT16_T, big::DataTypes::UINT8_T, big::DataTypes::UINT32_T, big::DataTypes::UINT64_T, big::DataTypes::INT8_T,  big::DataTypes::INT16_T, big::DataTypes::INT32_T, big::DataTypes::INT64_T, big::DataTypes::FLOAT, big::DataTypes::DOUBLE, big::DataTypes::HALF };
            const auto &dataType = big.getDataType();
            for (uint64_t i = 0; i != dataType.size(); ++i) {
                Assert::AreEqual(testDataType[i % 11], dataType[i]);
            }
            uint64_t i = 0;
            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::normal_distribution<double> dis(static_cast<double>((number_of_imadges - 12) / 2), DEVIATION);
            std::uniform_int_distribution<uint64_t> dis1(0, big.getImageHeight() - 1);
            std::uniform_int_distribution<uint64_t> dis2(0, big.getImageWidth() - 1);
            std::uniform_int_distribution<uint64_t> dis3(0, big.getNumberOfPlanes() - 1);

            while (i < NUMBER_OF_READS)
            {
                uint64_t imageNum = std::round(dis(gen));
                uint64_t rowIndex = dis1(gen);
                uint64_t colIndex = dis2(gen);
                uint64_t numberOfPlanes = dis3(gen);
                switch (big.getEntityDataType(imageNum))
                {
                case big::DataTypes::UINT8_T:
                {
                    auto point = big.at<uint8_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;

                }
                case big::DataTypes::UINT16_T:
                {
                    auto point = big.at<uint16_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::UINT32_T:
                {
                    auto point = big.at<uint32_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::UINT64_T:
                {
                    auto point = big.at<uint64_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT8_T:
                {
                    auto point = big.at<int8_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT16_T:
                {
                    auto point = big.at<int16_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT32_T:
                {
                    auto point = big.at<int32_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT64_T:
                {
                    auto point = big.at<int64_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::HALF:
                {
                    auto point = big.at<half>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;

                }
                case big::DataTypes::FLOAT:
                {
                    auto point = big.at<float>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::DOUBLE:
                {
                    auto point = big.at<double>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                }
                i++;

            }


        }
        // random read with normal distribution and bigger images
        TEST_METHOD(BigCoreRead_Random_at_with_cache_normal_distribution) {
            uint16_t n = 10 * 20 * 3;
            uint64_t data_test_size = DATA_TEST_SIZE; //if change this value delete test file or comment if for creating data
            std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
            for (uint64_t  i = 0; i != n; ++i) data1.get()[i] = i;
            std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
            for (uint64_t  i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
            std::shared_ptr<uint32_t> data3{ new uint32_t[n], [](uint32_t *p) {delete[] p; } };
            for (uint64_t  i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint32_t>(n) + i;
            std::shared_ptr<uint64_t> data4{ new uint64_t[n], [](uint64_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data4.get()[i] = 2 * static_cast<uint64_t>(n) + i;
            std::shared_ptr<int8_t> data5{ new int8_t[n], [](int8_t *p) {delete[] p; } };
            for (uint64_t  i = 0; i != n; ++i) data5.get()[i] = 2 * static_cast<int8_t>(n) + i;
            std::shared_ptr<int16_t> data6{ new int16_t[n], [](int16_t *p) {delete[] p; } };
            for (uint64_t  i = 0; i != n; ++i) data6.get()[i] = 2 * static_cast<int16_t>(n) + i;
            std::shared_ptr<int32_t> data7{ new int32_t[n], [](int32_t *p) {delete[] p; } };
            for (uint64_t  i = 0; i != n; ++i) data7.get()[i] = 2 * static_cast<int32_t>(n) + i;
            std::shared_ptr<int64_t> data8{ new int64_t[n], [](int64_t *p) {delete[] p; } };
            for (uint64_t  i = 0; i != n; ++i) data8.get()[i] = 2 * static_cast<int64_t>(n) + i;
            std::shared_ptr<float> data9{ new float[n], [](float *p) {delete[] p; } };
            for (uint64_t  i = 0; i != n; ++i) data9.get()[i] = 2 * static_cast<float>(n) + i;
            std::shared_ptr<double> data10{ new double[n], [](double *p) {delete[] p; } };
            for (uint64_t  i = 0; i != n; ++i) data10.get()[i] = 2 * static_cast<double>(n) + i;
            std::shared_ptr<half> data11{ new half[n], [](half *p) {delete[] p; } };
            for (uint64_t  i = 0; i != n; ++i) { data11.get()[i] = 2 * static_cast<half> (n) + i; }
            uint64_t number_of_imadges = data_test_size * 11;
            //if (!exists_test("testCoreRead_random_at_cache_test2_normal.big"))
            {
                big::BigCoreWrite big("testCoreRead_random_at_cache_test2_normal.big", 10, 20, 3);
                uint64_t j = 0;
                while (j < data_test_size) {
                    big.pushEntity(data1, big::DataTypes::UINT16_T);
                    big.pushEntity(data2, big::DataTypes::UINT8_T);
                    big.pushEntity(data3, big::DataTypes::UINT32_T);
                    big.pushEntity(data4, big::DataTypes::UINT64_T);
                    big.pushEntity(data5, big::DataTypes::INT8_T);
                    big.pushEntity(data6, big::DataTypes::INT16_T);
                    big.pushEntity(data7, big::DataTypes::INT32_T);
                    big.pushEntity(data8, big::DataTypes::INT64_T);
                    big.pushEntity(data9, big::DataTypes::FLOAT);
                    big.pushEntity(data10, big::DataTypes::DOUBLE);
                    big.pushEntity(data11, big::DataTypes::HALF);
                    j++;
                }
            }
            big::BigCoreRead big("testCoreRead_random_at_cache_test2_normal.big", false, CACHE_SIZE);
            Assert::AreEqual(number_of_imadges, big.getNumberOfImages());
            Assert::AreEqual(10ull, big.getImageHeight());
            Assert::AreEqual(20ull, big.getImageWidth());
            Assert::AreEqual(3ull, big.getNumberOfPlanes());
            const auto &dataOrder = big.getDataOrder();
            for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
            }
            std::vector<big::DataTypes> testDataType{ big::DataTypes::UINT16_T, big::DataTypes::UINT8_T, big::DataTypes::UINT32_T, big::DataTypes::UINT64_T, big::DataTypes::INT8_T,  big::DataTypes::INT16_T, big::DataTypes::INT32_T, big::DataTypes::INT64_T, big::DataTypes::FLOAT, big::DataTypes::DOUBLE, big::DataTypes::HALF };
            const auto &dataType = big.getDataType();
            for (uint64_t i = 0; i != dataType.size(); ++i) {
                Assert::AreEqual(testDataType[i % 11], dataType[i]);
            }
            uint64_t i = 0;
            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::normal_distribution<double> dis(static_cast<double>((number_of_imadges - 12)/2), DEVIATION); //first value is mean, second value is standard deviation
            std::uniform_int_distribution<uint64_t> dis1(0, big.getImageHeight() - 1);
            std::uniform_int_distribution<uint64_t> dis2(0, big.getImageWidth() - 1);
            std::uniform_int_distribution<uint64_t> dis3(0, big.getNumberOfPlanes() - 1);

            while (i < NUMBER_OF_READS)
            {
                uint64_t imageNum = static_cast<uint64_t>(std::round(dis(gen)));
                uint64_t rowIndex = dis1(gen);
                uint64_t colIndex = dis2(gen);
                uint64_t numberOfPlanes = dis3(gen);
                switch (big.getEntityDataType(imageNum))
                {
                case big::DataTypes::UINT8_T:
                {
                    auto point = big.at<uint8_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;

                }
                case big::DataTypes::UINT16_T:
                {
                    auto point = big.at<uint16_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::UINT32_T:
                {
                    auto point = big.at<uint32_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::UINT64_T:
                {
                    auto point = big.at<uint64_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT8_T:
                {
                    auto point = big.at<int8_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT16_T:
                {
                    auto point = big.at<int16_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT32_T:
                {
                    auto point = big.at<int32_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::INT64_T:
                {
                    auto point = big.at<int64_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::HALF:
                {
                    auto point = big.at<half>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;

                }
                case big::DataTypes::FLOAT:
                {
                    auto point = big.at<float>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                case big::DataTypes::DOUBLE:
                {
                    auto point = big.at<double>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    break;
                }
                }
                i++;
                if (i == 40000) { // for dubugging, watch if time working
                    continue;
                }

            }


        }
        //test random read with normal distribution of data read and bigger images tests
        TEST_METHOD(BigCoreRead_Random_at_with_cache_speed_normal_dist) {
            uint16_t n = 3 * 20 * 10;
            uint64_t data_test_size = DATA_TEST_SIZE; //if change this value delete test file or comment if for creating data
            std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
            for (uint64_t i = 0; i != n; ++i) data1.get()[i] = i;
            
            uint64_t number_of_imadges = data_test_size *11 ;
            if (!exists_test("testCoreRead_random_at_cache_test3_speed_normal.big"))
            {
                big::BigCoreWrite big("testCoreRead_random_at_cache_test3_speed_normal.big", 10, 20, 3);
                uint64_t j = 0;
                while (j < number_of_imadges) {
                    big.pushEntity(data1, big::DataTypes::UINT16_T);
                    j++;
                }
            }
            big::BigCoreRead big("testCoreRead_random_at_cache_test3_speed_normal.big", false, CACHE_SIZE);
            Assert::AreEqual(number_of_imadges, big.getNumberOfImages());
            Assert::AreEqual(10ull, big.getImageHeight());
            Assert::AreEqual(20ull, big.getImageWidth());
            Assert::AreEqual(3ull, big.getNumberOfPlanes());
            
            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::normal_distribution<double> dis(static_cast<double>((number_of_imadges - 12) / 2), DEVIATION);
            std::uniform_int_distribution<uint64_t> dis1(0, big.getImageHeight() - 1);
            std::uniform_int_distribution<uint64_t> dis2(0, big.getImageWidth() - 1);
            std::uniform_int_distribution<uint64_t> dis3(0, big.getNumberOfPlanes() - 1);
            uint64_t i = 0;
            while (i < NUMBER_OF_READS)
            {
                uint64_t imageNum = std::round(dis(gen));
                uint64_t rowIndex = dis1(gen);
                uint64_t colIndex = dis2(gen);
                uint64_t numberOfPlanes = dis3(gen);
               
                auto point = big.at<uint16_t>(imageNum, rowIndex, colIndex, numberOfPlanes);
                    
                
                i++;

            }


        }

        TEST_METHOD(BigCoreRead_XML)
        {
          {
            big::BigCoreWrite big("testCoreRead_XMLread.big", 2, 4);

            Assert::AreEqual(0ull, big.getNumberOfImages());
            Assert::AreEqual(2ull, big.getImageHeight());
            Assert::AreEqual(4ull, big.getImageWidth());
            Assert::AreEqual(1ull, big.getNumberOfPlanes());
            big.AddAttributeToXmlInt("opacity", 20, "opacity of this material");
            big.AddAttributeToXmlDouble("luminance", 21.15, "opacity of this material");
            big.AddAttributeToXmlBool("isDark", false, "is this material dark");
            
          }
          {
            big::BigCoreRead big("testCoreRead_XMLread.big", false, CACHE_SIZE);
            Assert::AreEqual(big.readXMLInt("opacity"), 20);
            Assert::AreEqual(big.readXMLDouble("luminance"), 21.15);
            Assert::AreEqual(big.readXMLBool("isDark"), false);
           
          }
        }


    };

    
}
