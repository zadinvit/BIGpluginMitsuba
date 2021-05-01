#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../include/MIF.hpp"
using mif::MIF;
using image::Image;

namespace MIFlibTest
{
    TEST_CLASS(MIFunitTest)
    {
    public:

        TEST_METHOD(TestEmptyConstructor)
        {
            {
                // test that empty constructor does nothing
                MIF mif;
                Assert::AreEqual(static_cast<uint64_t>(0), mif.getNumberOfImages());
                mif.close();
            }
            {
                // test that empty constructor does nothing and file can be opened later
                MIF mif;
                mif.close();
                remove("MIF_TestEmptyConstructor1.mif");
                bool ok = mif.open("MIF_TestEmptyConstructor1.mif");
                Assert::AreEqual(true, ok);
                std::ifstream in("MIF_TestEmptyConstructor1.mif", std::ifstream::ate | std::ifstream::binary);
                int size = static_cast<int>(in.tellg());
                Assert::AreEqual(3 * 8, size);
            }
            {
                // test that trying operations while file is not open throws exception
                // todo: test other operations
                Image<uint8_t> img(10, 20);
                MIF writer;
                try {
                    writer.addImage(0, img);
                }
                catch (const std::ios_base::failure & f) {
                    std::string message = "file not open";
                    Assert::AreEqual(message, std::string(f.what()).substr(0, message.length()));
                    return;
                }
                Assert::AreEqual(true, false);
            }
        }

        TEST_METHOD(TestConstructor)
        {
            {
                // test than constructor on non existing file creates one and writes header
                remove("MIF_TestConstructor1.mif");
                { MIF mif("MIF_TestConstructor1.mif"); }
                std::ifstream in("MIF_TestConstructor1.mif", std::ifstream::ate | std::ifstream::binary);
                int size = static_cast<int>(in.tellg());
                Assert::AreEqual(3 * 8, size);
            }
            {
                // test that new file is correctly opened for writing
                Image<uint8_t> img(10, 20);
                remove("MIF_TestConstructor2.mif");
                MIF mif("MIF_TestConstructor2.mif");
                try {
                    mif.addImage(0, img);
                }
                catch (...) {
                    Assert::AreEqual(true, false);
                }
                mif.close();
                std::ifstream in("MIF_TestConstructor2.mif", std::ifstream::ate | std::ifstream::binary);
                int size = static_cast<int>(in.tellg());
                Assert::AreEqual((3 * 8) + (2 * 8 + 6 * 8 + 10 * 20), size);
            }
        }

        TEST_METHOD(TestConstructorReadOnly)
        {
            {
                // create file for other test
                remove("MIF_TestConstructorReadOnly1.mif");
                MIF mif("MIF_TestConstructorReadOnly1.mif");
                Image<uint8_t> img(10, 20);
                mif.addImage(0, img);
                mif.addImage(1, img);
            }
            {
                // test that file is correctly readed
                MIF mif("MIF_TestConstructorReadOnly1.mif", true);
                Assert::AreEqual(static_cast<uint64_t>(2), mif.getNumberOfImages());
            }
            {
                // test that read-only file cannot be writed in
                MIF mif("MIF_TestConstructorReadOnly1.mif", true);
                try {
                    Image<uint8_t> img(10, 20);
                    mif.addImage(2, img);
                }
                catch (...) {
                    return;
                }
                Assert::AreEqual(true, false);
            }
        }

        //TEST_METHOD(TestFillConstructor3)
        //{
        //    {
        //        // assign incorrect 2D data
        //        std::vector<uint8_t> data = { 1, 2, 3, 4, 5, 6, 7 };
        //        bool correct = false;
        //        std::string error_string;
        //        try {
        //            Image<uint8_t> img(data, 2, 3);
        //        }
        //        catch (const std::length_error & le) {
        //            correct = true;
        //            error_string = le.what();
        //        }
        //        catch (...) {
        //            correct = false;
        //        }
        //        Assert::AreEqual(true, correct);
        //        Assert::AreEqual(false, error_string.empty());
        //    }
        //}


        // TODO: all other tests

    };
}