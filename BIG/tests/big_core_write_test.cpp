#include "common.hpp"

#include "../include/big_core_write.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace big_test
{
    using half_float::half;
    using half_float::half_cast;
    TEST_CLASS(BigCoreWrite_UnitTest)
    {
    public:

        TEST_METHOD(BigCoreWrite_Constructor)
        {
            {
                {
                    big::BigCoreWrite big("testCoreWrite_Constructor1.big", 2, 4);

                    Assert::AreEqual(0ull, big.getNumberOfImages());
                    Assert::AreEqual(2ull, big.getImageHeight());
                    Assert::AreEqual(4ull, big.getImageWidth());
                    Assert::AreEqual(1ull, big.getNumberOfPlanes());
                }
                {
                    std::ifstream file("testCoreWrite_Constructor1.big", std::ios_base::binary | std::ios_base::in);
                    checkHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 0, 2, 8, 2, 3, 8, 4, 4, 8, 1, 5, 32, 1, 2, 3, 4};//, 6, 8, 1 
                    checkMetaData(file, metaData, "testCoreWrite_Constructor1.xml");
                }
            }
            {
                {
                    big::BigCoreWrite big("testCoreWrite_Constructor2.big", 3, 5, 3);
                }
                {
                    std::ifstream file("testCoreWrite_Constructor2.big", std::ios_base::binary | std::ios_base::in);
                    checkHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 0, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32, 1, 2, 3, 4 };//, 6, 8, 1
                    checkMetaData(file, metaData,"testCoreWrite_Constructor2.xml");
                }
            }
            {
                std::vector<big::ChunkIds> dataOrder = { big::ChunkIds::IMAGE_HEIGHT, big::ChunkIds::IMAGE_WIDTH, big::ChunkIds::NUMBER_OF_PLANES, big::ChunkIds::NUMBER_OF_IMAGES };
                std::vector<big::DataTypes> dataType = { big::DataTypes::UINT16_T };
                {
                    big::BigCoreWrite big("testCoreWrite_Constructor3.big", 3, 5, 3, dataOrder);
                }
                {
                    std::ifstream file("testCoreWrite_Constructor3.big", std::ios_base::binary | std::ios_base::in);
                    checkHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 0, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32, 2, 3, 4, 1 };//, 6, 8, 2
                    checkMetaData(file, metaData, "testCoreWrite_Constructor3.xml");
                }
            }
        }

        TEST_METHOD(BigCoreWrite_addEntity)
        {
            {
                uint64_t n = 2 * 4;
                std::shared_ptr<uint8_t> data{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (int i = 0; i != n; ++i) data.get()[i] = i;
                {
                    big::BigCoreWrite big("testCoreWrite_AddEntity1.big", 2, 4);
                    big.addEntity(data, 0, big::DataTypes::UINT8_T);
                }
                {
                    std::ifstream file("testCoreWrite_AddEntity1.big", std::ios_base::binary | std::ios_base::in);
                    checkHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 1, 2, 8, 2, 3, 8, 4, 4, 8, 1, 5, 32, 1, 2, 3, 4 }; // 6, 8, 1 old datatype
                    checkMetaData(file, metaData, "testCoreWrite_AddEntity1.xml");
                    checkData(file, data, n, 0, (uint64_t) 1); //1 = UINT8_T dataType
                }
            }
            {
                uint64_t n = 3 * 5 * 3;
                std::shared_ptr<uint8_t> data1{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (uint8_t i = 0; i != n; ++i) data1.get()[i] = i;
                std::shared_ptr<uint8_t> data2{ new uint8_t[n], [](uint8_t *p) {delete[] p; } };
                for (uint8_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint8_t>(n) + i;
                {
                    big::BigCoreWrite big("testCoreWrite_AddEntity2.big", 3, 5, 3);
                    big.addEntity(data1, 0, big::DataTypes::UINT8_T); 
                    big.addEntity(data2, 1, big::DataTypes::UINT8_T);
                }
                {
                    std::ifstream file("testCoreWrite_AddEntity2.big", std::ios_base::binary | std::ios_base::in);
                    checkHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 2, 2, 8, 3, 3, 8, 5, 4, 8, 3, 5, 32, 1, 2, 3, 4 }; // 6, 8, 1 old datatype
                    checkMetaData(file, metaData, "testCoreWrite_AddEntity2.xml");
                    checkData(file, data1, n, 0, (uint64_t) 1); //1 = UINT8_T dataType
                    checkData(file, data2, n, 1, (uint64_t) 1); //1 = UINT8_T dataType
                }
            }
            {
                //std::vector<big::ChunkIds> dataOrder = { big::ChunkIds::IMAGE_HEIGHT, big::ChunkIds::IMAGE_WIDTH, big::ChunkIds::NUMBER_OF_PLANES, big::ChunkIds::NUMBER_OF_IMAGES };
                big::DataTypes dataType = big::DataTypes::UINT16_T;
                uint16_t n = 5 * 3 * 2;
                std::shared_ptr<uint16_t> data1{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
                for (uint16_t i = 0; i != n; ++i) data1.get()[i] = i;
                std::shared_ptr<uint16_t> data2{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
                for (uint16_t i = 0; i != n; ++i) data2.get()[i] = static_cast<uint16_t>(n) + i;
                std::shared_ptr<uint16_t> data3{ new uint16_t[n], [](uint16_t *p) {delete[] p; } };
                for (uint16_t i = 0; i != n; ++i) data3.get()[i] = 2 * static_cast<uint16_t>(n) + i;
                {
                    big::BigCoreWrite big("testCoreWrite_AddEntity3.big", 2, 3, 5);
                    big.addEntity(data1, 0, dataType);
                    big.addEntity(data2, 1, dataType);
                    big.addEntity(data3, 2, dataType);
                }
                {
                    std::ifstream file("testCoreWrite_AddEntity3.big", std::ios_base::binary | std::ios_base::in);
                    checkHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 3, 2, 8, 2, 3, 8, 3, 4, 8, 5, 5, 32, 1, 2, 3, 4 }; //6, 8, 2 old datatype
                    checkMetaData(file, metaData, "testCoreWrite_AddEntity3.xml");
                    checkData(file, data1, n, 0, (uint64_t) 2); //2 = UINT16_T dataType
                    checkData(file, data2, n, 1, (uint64_t) 2); 
                    checkData(file, data3, n, 2, (uint64_t) 2);
                }
            }

            
        }
        TEST_METHOD(BigCoreWrite_diffrentDataTypes)
        {
            {
                //diffrent datatypes in one file
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
                for (int i = 0; i != n; ++i) data11.get()[i] = 2 * static_cast<half> (n)+i;
                {
                    big::BigCoreWrite big("testCoreWrite_AddEntity4.big", 2, 3, 5);
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
                {
                    std::ifstream file("testCoreWrite_AddEntity4.big", std::ios_base::binary | std::ios_base::in);
                    checkHeader(file);
                    std::vector<uint64_t> metaData{ 1, 8, 11, 2, 8, 2, 3, 8, 3, 4, 8, 5, 5, 32, 1, 2, 3, 4 }; //6, 8, 2 old datatype
                    checkMetaData(file, metaData, "testCoreWrite_AddEntity4.xml");
                    checkData(file, data1, n, 0, (uint64_t)2); //2 = UINT16_T dataType
                    checkData(file, data2, n, 1, (uint64_t)1); //1 = UINT8_T dataType
                    checkData(file, data3, n, 2, (uint64_t)3); //3 = UINT32_T dataType
                    checkData(file, data4, n, 3, (uint64_t)4); //4 = UINT64_T dataType
                    checkData(file, data5, n, 4, (uint64_t)5); //5 = INT8_T dataType
                    checkData(file, data6, n, 5, (uint64_t)6); //6 = INT16_T dataType
                    checkData(file, data7, n, 6, (uint64_t)7); //7 = INT32_T dataType
                    checkData(file, data8, n, 7, (uint64_t)8); //8 = INT64_T dataType
                    checkData(file, data9, n, 8, (uint64_t)10); //10 = FLOAT dataType
                    checkData(file, data10, n, 9, (uint64_t)11); //11 = DOUBLE dataType
                    checkData(file, data11, n, 10, (uint64_t)9); //9 = HALF dataType
                }
            }
        }
        TEST_METHOD(BigCoreWrite_XMLFile)
        {
          
            {
              big::BigCoreWrite big("testCoreWrite_XMLWrite1.big", 2, 4);

              Assert::AreEqual(0ull, big.getNumberOfImages());
              Assert::AreEqual(2ull, big.getImageHeight());
              Assert::AreEqual(4ull, big.getImageWidth());
              Assert::AreEqual(1ull, big.getNumberOfPlanes());
              big.AddAttributeToXmlInt("opacity", 20, "opacity of this material");
              big.AddAttributeToXmlDouble("opacity", 21.15, "opacity of this material");
              big.AddAttributeToXmlBool("isDark", false, "is this material dark");
              Assert::AreEqual(big.RemoveNodesByName("opacity"), true);
              Assert::AreEqual(big.RemoveNodesByName("isDark"), true);
              big.AddAttributeToXmlDouble("opacity", 21.15, "opacity of this material");
              big.AddAttributeToXmlString("data", "uniform", "Data are uniform");
            }
            {
              std::ifstream file("testCoreWrite_Constructor1.big", std::ios_base::binary | std::ios_base::in);
              checkHeader(file);
              std::vector<uint64_t> metaData{ 1, 8, 0, 2, 8, 2, 3, 8, 4, 4, 8, 1, 5, 32, 1, 2, 3, 4 };//, 6, 8, 1 
              checkMetaData(file, metaData, "testCoreWrite_Constructor1.xml");
            }
            {
              big::BigCoreWrite big("testCoreWrite_XMLWrite2.big", 2, 4);

              Assert::AreEqual(0ull, big.getNumberOfImages());
              Assert::AreEqual(2ull, big.getImageHeight());
              Assert::AreEqual(4ull, big.getImageWidth());
              Assert::AreEqual(1ull, big.getNumberOfPlanes());
              big.AddAttributeToXmlInt("opacity", 20, "opacity of this material");
              big.AddAttributeToXmlDouble("opacity", 21.15, "opacity of this material");
              big.AddAttributeToXmlBool("isDark", false, "is this material dark");
              big.AddAttributeToXmlDouble("opacity", 21.15, "opacity of this material");
              big.AddAttributeToXmlString("data", "uniform", "Data are uniform");
              std::string document = big.GetXMLString();
              big.RemoveAllNodes();
            }
            {
              std::ifstream file("testCoreWrite_Constructor1.big", std::ios_base::binary | std::ios_base::in);
              checkHeader(file);
              std::vector<uint64_t> metaData{ 1, 8, 0, 2, 8, 2, 3, 8, 4, 4, 8, 1, 5, 32, 1, 2, 3, 4 };//, 6, 8, 1 
              checkMetaData(file, metaData, "testCoreWrite_Constructor1.xml");
            }
         
        }
         
    };
}
