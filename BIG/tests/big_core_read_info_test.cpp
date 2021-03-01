#include "common.hpp"

#include "../include/big_core_read_info.hpp"
#include "../include/big_core_write.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace big_test
{
    TEST_CLASS(BigCoreReadInfo_UnitTest)
    {
    public:

        TEST_METHOD(BigCoreReadInfo_Constructor)
        {
            {
                {
                    std::ofstream file("testCoreReadInfo_Constructor1.big", std::ios_base::binary | std::ios_base::out);
                    writeHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 0, 2, 8, 2, 3, 8, 4, 4, 8, 1, 5, 32, 1, 2, 3, 4 };//, 6, 8, 1
                    writeMetaData(file, metaData);
                }
                {
                    big::BigCoreReadInfo big("testCoreReadInfo_Constructor1.big");
                    Assert::AreEqual(0ull, big.getNumberOfImages());
                    Assert::AreEqual(2ull, big.getImageHeight());
                    Assert::AreEqual(4ull, big.getImageWidth());
                    Assert::AreEqual(1ull, big.getNumberOfPlanes());
                    const auto &dataOrder = big.getDataOrder();
                    for (uint64_t i = 0; i != dataOrder.size(); ++i) {
                        Assert::AreEqual(big::defaultDataOrder[i], dataOrder[i]);
                    }
                    const auto &dataType = big.getDataType();
                    for (uint64_t i = 0; i != dataType.size(); ++i) {
                        Assert::AreEqual(big::defaultDataType[0], dataType[i]);
                    }
                }
            }
            {
                {
                    std::ofstream file("testCoreReadInfo_Constructor2.big", std::ios_base::binary | std::ios_base::out);
                    writeHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 0, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32, 1, 2, 3, 4 };//, 6, 8, 1
                    writeMetaData(file, metaData);
                }
                {
                    big::BigCoreReadInfo big("testCoreReadInfo_Constructor2.big");
                    Assert::AreEqual(0ull, big.getNumberOfImages());
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
                }
            }
            {
                std::vector<big::ChunkIds> dataOrder = { big::ChunkIds::IMAGE_HEIGHT, big::ChunkIds::IMAGE_WIDTH, big::ChunkIds::NUMBER_OF_PLANES, big::ChunkIds::NUMBER_OF_IMAGES };
                std::vector<big::DataTypes> dataType = { big::DataTypes::UINT16_T };
                {
                    std::ofstream file("testCoreReadInfo_Constructor3.big", std::ios_base::binary | std::ios_base::out);
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
                }
                {    
                    //catch exception, which is throw if dataorder is not correct
                    try
                    {


                        big::BigCoreReadInfo big("testCoreReadInfo_Constructor3.big");
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
                    }
                    catch(const char * str)
                    {
                        Assert::AreEqual(str, "This data order is not supported!");
                    }
                    
                }
            }
            {
              big::BigCoreWrite big("testCoreReadInfo_Constructor4.big", 2, 4);
            }
            {
              big::BigCoreReadInfo big("testCoreReadInfo_Constructor4.big");
              Assert::AreEqual(0ull, big.getNumberOfImages());
              Assert::AreEqual(2ull, big.getImageHeight());
              Assert::AreEqual(4ull, big.getImageWidth());
              Assert::AreEqual(1ull, big.getNumberOfPlanes());
              //Assert::AreEqual("testCoreReadInfo_Constructor4.xml", big.getXMLFileName().c_str());
            }
        }
    };
}
