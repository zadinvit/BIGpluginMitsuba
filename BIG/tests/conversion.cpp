#include "common.hpp"
#include <cmath> 

#include "../include/half.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace big_test
{

    //double asDouble(unsigned __int64 v)
    //{
    //    return ((__int64)(v >> 11)) / (double)(1L << 53);
    //}
    //double asDouble(uint64_t v)
    //{
    //    // the 0x3FF sets the exponent to the 0..1 range.
    //    uint64_t vv = (v >> 11) | (0x3FFL << 53);
    //    return *(double*)&vv;
    //}


    using half_float::half;
    using half_float::half_cast;
    using namespace half_float::literal;

    template<typename Tdst, typename Tsrc>
    Tdst convert(Tsrc value);

    template<> uint8_t convert(uint8_t value) { return value; }
    template<> uint8_t convert(uint16_t value) { return static_cast<uint8_t>(value >> 8); }
    template<> uint8_t convert(uint32_t value) { return static_cast<uint8_t>(value >> 24); }
    template<> uint8_t convert(uint64_t value) { return static_cast<uint8_t>(value >> 56); }
    template<> uint8_t convert(int8_t value) { return value <= 0 ? 0 : static_cast<uint8_t>((value << 1) + (value >> 6)); }
    template<> uint8_t convert(int16_t value) { return value <= 0 ? 0 : static_cast<uint8_t>(value >> 7); }
    template<> uint8_t convert(int32_t value) { return value <= 0 ? 0 : static_cast<uint8_t>(value >> 23); }
    template<> uint8_t convert(int64_t value) { return value <= 0 ? 0 : static_cast<uint8_t>(value >> 55); }
    template<> uint8_t convert(half value) { return value <= 0 ? 0 : value >= 1.0_h ? 255 : static_cast<uint8_t>(value * 255.0_h + 0.5_h); }
    template<> uint8_t convert(float value) { return value <= 0 ? 0 : value >= 1.0f ? 255 : static_cast<uint8_t>(value * 255.0f + 0.5f); }
    template<> uint8_t convert(double value) { return value <= 0 ? 0 : value >= 1.0 ? 255 : static_cast<uint8_t>(value * 255.0 + 0.5); }

    template<> uint16_t convert(uint8_t value) { return (static_cast<uint16_t>(value) << 8) + value; }
    template<> uint16_t convert(uint16_t value) { return value; }
    template<> uint16_t convert(uint32_t value) { return static_cast<uint16_t>(value >> 16); }
    template<> uint16_t convert(uint64_t value) { return static_cast<uint16_t>(value >> 48); }
    template<> uint16_t convert(int8_t value) { return value <= 0 ? 0 : static_cast<uint16_t>((value << 9) + ((value & 64) << 2) + (value << 1) + (value >> 6)); }
    template<> uint16_t convert(int16_t value) { return value <= 0 ? 0 : static_cast<uint16_t>((value << 1) + (value >> 14)); }
    template<> uint16_t convert(int32_t value) { return value <= 0 ? 0 : static_cast<uint16_t>(value >> 15); }
    template<> uint16_t convert(int64_t value) { return value <= 0 ? 0 : static_cast<uint16_t>(value >> 47); }
    template<> uint16_t convert(half value) { return value <= 0 ? 0 : value >= 1.0_h ? 0xFFFF : static_cast<uint16_t>(static_cast<float>(value) * 65535.0f + 0.5f); }
    template<> uint16_t convert(float value) { return value <= 0 ? 0 : value >= 1.0f ? 0xFFFF : static_cast<uint16_t>(value * 65535.0f + 0.5f); }
    template<> uint16_t convert(double value) { return value <= 0 ? 0 : value >= 1.0 ? 0xFFFF : static_cast<uint16_t>(value * 65535.0 + 0.5); }

    template<> uint32_t convert(uint8_t value) { uint32_t v = static_cast<uint32_t>(value); return (v << 24) + (v << 16) + (v << 8) + v; }
    template<> uint32_t convert(uint16_t value) { return (static_cast<uint32_t>(value) << 16) + value; }
    template<> uint32_t convert(uint32_t value) { return value; }
    template<> uint32_t convert(uint64_t value) { return static_cast<uint32_t>(value >> 32); }
    template<> uint32_t convert(int8_t value) { if (value <= 0) return 0; uint32_t v = (value << 1) + (value >> 6); return (v << 24) + (v << 16) + (v << 8) + v; }
    template<> uint32_t convert(int16_t value) { if (value <= 0) return 0; uint32_t v = static_cast<uint32_t>((value << 1) + (value >> 14)); return (v << 16) + v; }
    template<> uint32_t convert(int32_t value) { return value <= 0 ? 0 : static_cast<uint32_t>((value << 1) + (value >> 30)); }
    template<> uint32_t convert(int64_t value) { return value <= 0 ? 0 : static_cast<uint32_t>(value >> 31); }
    template<> uint32_t convert(half value) { return value <= 0 ? 0 : value >= 1.0_h ? 0xFFFFFFFF : static_cast<uint32_t>(static_cast<double>(value) * 4294967295.0 + 0.5); }
    template<> uint32_t convert(float value) { return value <= 0 ? 0 : value >= 1.0f ? 0xFFFFFFFF : static_cast<uint32_t>(static_cast<double>(value) * 4294967295.0 + 0.5); }
    template<> uint32_t convert(double value) { return value <= 0 ? 0 : value >= 1.0 ? 0xFFFFFFFF : static_cast<uint32_t>(value * 4294967295.0 + 0.5); }

    template<> uint64_t convert(uint8_t value) { uint64_t v = static_cast<uint64_t>(value); return (v << 56) + (v << 48) + (v << 40) + (v << 32) + (v << 24) + (v << 16) + (v << 8) + v; }
    template<> uint64_t convert(uint16_t value) { uint64_t v = static_cast<uint64_t>(value); return (v << 48) + (v << 32) + (v << 16) + v; }
    template<> uint64_t convert(uint32_t value) { uint64_t v = static_cast<uint64_t>(value); return (v << 32) + v; }
    template<> uint64_t convert(uint64_t value) { return value; }
    template<> uint64_t convert(int8_t value) { if (value <= 0) return 0; uint64_t v = static_cast<uint64_t>((value << 1) + (value >> 6)); return (v << 56) + (v << 48) + (v << 40) + (v << 32) + (v << 24) + (v << 16) + (v << 8) + v; }
    template<> uint64_t convert(int16_t value) { if (value <= 0) return 0; uint64_t v = static_cast<uint64_t>((value << 1) + (value >> 14)); return (v << 48) + (v << 32) + (v << 16) + v; }
    template<> uint64_t convert(int32_t value) { if (value <= 0) return 0; uint64_t v = static_cast<uint64_t>(value); v = static_cast<uint64_t>((v << 1) + (v >> 30)); return (v << 32) + v; }
    template<> uint64_t convert(int64_t value) { return value <= 0 ? 0 : static_cast<uint64_t>((value << 1) + (value >> 62)); }
    template<> uint64_t convert(half value) { if (value <= 0) return 0; else if (value >= 1.0_h) return 0xFFFFFFFFFFFFFFFF; uint64_t v = static_cast<uint64_t>(static_cast<double>(value) * 9007199254740991.0); return (v << 11) + (v >> 42); }
    template<> uint64_t convert(float value) { if (value <= 0) return 0; else if (value >= 1.0f) return 0xFFFFFFFFFFFFFFFF; uint64_t v = static_cast<uint64_t>(static_cast<double>(value) * 9007199254740991.0); return (v << 11) + (v >> 42); }
    template<> uint64_t convert(double value) { if (value <= 0) return 0; else if (value >= 1.0) return 0xFFFFFFFFFFFFFFFF; uint64_t v = static_cast<uint64_t>(value * 9007199254740991.0); return (v << 11) + (v >> 42); }

    template<> int8_t convert(uint8_t value) { return static_cast<int8_t>(value >> 1); }
    template<> int8_t convert(uint16_t value) { return static_cast<int8_t>(value >> 9); }
    template<> int8_t convert(uint32_t value) { return static_cast<int8_t>(value >> 25); }
    template<> int8_t convert(uint64_t value) { return static_cast<int8_t>(value >> 57); }
    template<> int8_t convert(int8_t value) { return value; }
    template<> int8_t convert(int16_t value) { return static_cast<int8_t>(value >> 8); }
    template<> int8_t convert(int32_t value) { return static_cast<int8_t>(value >> 24); }
    template<> int8_t convert(int64_t value) { return static_cast<int8_t>(value >> 56); }
    template<> int8_t convert(half value) { return value <= -1.0_h ? -127 : value >= 1.0_h ? 127 : half_cast<int8_t, std::round_to_nearest>(value * 127.0_h); }
    template<> int8_t convert(float value) { return value <= -1.0f ? -127 : value >= 1.0f ? 127 : static_cast<int8_t>(round(value * 127.0f)); }
    template<> int8_t convert(double value) { return value <= -1.0 ? -127 : value >= 1.0 ? 127 : static_cast<int8_t>(round(value * 127.0)); }

    template<> int16_t convert(uint8_t value) { int16_t v = static_cast<int16_t>(value); return (v << 7) | v; }
    template<> int16_t convert(uint16_t value) { return static_cast<int16_t>(value >> 1); }
    template<> int16_t convert(uint32_t value) { return static_cast<int16_t>(value >> 17); }
    template<> int16_t convert(uint64_t value) { return static_cast<int16_t>(value >> 49); }
    template<> int16_t convert(int8_t value) { int16_t v = static_cast<int16_t>(value); return (v > 0) ? (v << 8) + (v << 1) + (v >> 6) : (v << 8); }
    template<> int16_t convert(int16_t value) { return value; }
    template<> int16_t convert(int32_t value) { return static_cast<int16_t>(value >> 16); }
    template<> int16_t convert(int64_t value) { return static_cast<int16_t>(value >> 48); }
    template<> int16_t convert(half value) { return value <= -1.0_h ? -32767 : value >= 1.0_h ? 32767 : static_cast<int16_t>(round(static_cast<double>(value) * 32767.0)); }
    template<> int16_t convert(float value) { return value <= -1.0f ? -32767 : value >= 1.0f ? 32767 : static_cast<int16_t>(round(value * 32767.0f)); }
    template<> int16_t convert(double value) { return value <= -1.0 ? -32767 : value >= 1.0 ? 32767 : static_cast<int16_t>(round(value * 32767.0)); }

    template<> int32_t convert(uint8_t value) { int32_t v = static_cast<int32_t>(value); return (v << 23) | (v << 15) | (v << 7) | v; }
    template<> int32_t convert(uint16_t value) { int32_t v = static_cast<int32_t>(value); return (v << 15) | v; }
    template<> int32_t convert(uint32_t value) { return static_cast<int32_t>(value >> 1); }
    template<> int32_t convert(uint64_t value) { return static_cast<int32_t>(value >> 33); }
    template<> int32_t convert(int8_t value) { int32_t v = static_cast<int32_t>(value); return (v > 0) ? (v << 24) + (v << 17) + (v << 10) + (v << 3) + (v >> 4) : (v << 24); }
    template<> int32_t convert(int16_t value) { int32_t v = static_cast<int32_t>(value); return (v > 0) ? (v << 16) + (v << 1) + (v >> 14) : (v << 16); }
    template<> int32_t convert(int32_t value) { return value; }
    template<> int32_t convert(int64_t value) { return static_cast<int32_t>(value >> 32); }
    template<> int32_t convert(half value) { return value <= -1.0_h ? -2147483647 : value >= 1.0_h ? 2147483647 : static_cast<int32_t>(round(static_cast<double>(value) * 2147483647.0)); }
    template<> int32_t convert(float value) { return value <= -1.0f ? -2147483647 : value >= 1.0f ? 2147483647 : static_cast<int32_t>(round(static_cast<double>(value) * 2147483647.0)); }
    template<> int32_t convert(double value) { return value <= -1.0 ? -2147483647 : value >= 1.0 ? 2147483647 : static_cast<int32_t>(round(value * 2147483647.0)); }

    template<> int64_t convert(uint8_t value) { int64_t v = static_cast<int64_t>(value); return (v << 55) | (v << 47) | (v << 39) | (v << 31) | (v << 23) | (v << 15) | (v << 7) | v; }
    template<> int64_t convert(uint16_t value) { int64_t v = static_cast<int64_t>(value); return (v << 47) | (v << 31) | (v << 15) | v; }
    template<> int64_t convert(uint32_t value) { int64_t v = static_cast<int64_t>(value); return (v << 31) | v; }
    template<> int64_t convert(uint64_t value) { return static_cast<int64_t>(value >> 1); }
    template<> int64_t convert(int8_t value) { int64_t v = static_cast<int64_t>(value); return (v > 0) ? (v << 56) + (v << 49) + (v << 42) + (v << 35) + (v << 28) + (v << 21) + (v << 14) + (v << 7) + v : (v << 56); }
    template<> int64_t convert(int16_t value) { int64_t v = static_cast<int64_t>(value); return (v > 0) ? (v << 48) + (v << 33) + (v << 18) + (v << 3) + (v >> 12) : (v << 48); }
    template<> int64_t convert(int32_t value) { int64_t v = static_cast<int64_t>(value); return (v > 0) ? (v << 32) + (v << 1) + (v >> 30) : (v << 32); }
    template<> int64_t convert(int64_t value) { return value; }
    template<> int64_t convert(half value) { if (value <= -1.0_h) return 0x8000000000000000; else if (value >= 1.0_h) return 0x7FFFFFFFFFFFFFFF; int64_t v = static_cast<int64_t>(round(static_cast<double>(value) * 9007199254740991.0)); return (v << 10) + (v >> 43); }
    template<> int64_t convert(float value) { if (value <= -1.0f) return 0x8000000000000000; else if (value >= 1.0f) return 0x7FFFFFFFFFFFFFFF; int64_t v = static_cast<int64_t>(round(static_cast<double>(value) * 9007199254740991.0)); return (v << 10) + (v >> 43); }
    template<> int64_t convert(double value) { if (value <= -1.0) return 0x8000000000000000; else if (value >= 1.0) return 0x7FFFFFFFFFFFFFFF; int64_t v = static_cast<int64_t>(round(value * 9007199254740991.0)); return (v << 10) + (v >> 43); }


    template<> float convert(uint8_t value) { return static_cast<float>(value) / 255.0f; }
    template<> float convert(uint16_t value) { return static_cast<float>(value) / 65535.0f; }
    template<> float convert(uint32_t value) { return static_cast<float>(value) / 4294967295.0f; }
    template<> float convert(uint64_t value) { return static_cast<float>(value >> 40) / 16777215.0f; }
    template<> float convert(int8_t value) { return static_cast<float>(value) / 127.0f; }
    template<> float convert(int16_t value) { return static_cast<float>(value) / 32767.0f; }
    template<> float convert(int32_t value) { return static_cast<float>(value) / 2147483647.0f; }
    template<> float convert(int64_t value) { return static_cast<float>(value) / static_cast<float>(0x7FFFFFFFFFFFFFFF); }
    template<> float convert(half value) { return static_cast<float>(value); }
    template<> float convert(float value) { return value; }
    template<> float convert(double value) { return static_cast<float>(value); }

    template<> double convert(uint8_t value) { return static_cast<double>(value) / 255.0; }
    template<> double convert(uint16_t value) { return static_cast<double>(value) / 65535.0; }
    template<> double convert(uint32_t value) { return static_cast<double>(value) / 4294967295.0; }
    template<> double convert(uint64_t value) { return static_cast<double>(value >> 11) / 9007199254740991.0; }
    template<> double convert(int8_t value) { return static_cast<double>(value) / 127.0; }
    template<> double convert(int16_t value) { return static_cast<double>(value) / 32767.0; }
    template<> double convert(int32_t value) { return static_cast<double>(value) / 2147483647.0;  }
    template<> double convert(int64_t value) { return static_cast<double>(value) / static_cast<double>(0x7FFFFFFFFFFFFFFF); }
    template<> double convert(half value) { return static_cast<double>(value); }
    template<> double convert(float value) { return static_cast<double>(value); }
    template<> double convert(double value) { return value; }

    template<> half convert(uint8_t value) { return half_cast<half>(value / 255.0); }
    template<> half convert(uint16_t value) { return half_cast<half>(value) / 65535.0_h;; }
    template<> half convert(uint32_t value) { return half_cast<half>(value / 4294967295.0); }
    template<> half convert(uint64_t value) { return half_cast<half>((value >> 11) / 9007199254740991.0);}
    template<> half convert(int8_t value) { return value == -128 ? (half_cast<half>(value) / 127.0_h) - 1.0_h : half_cast<half>(value) / 127.0_h; }
    template<> half convert(int16_t value) { return  half_cast<half>(value / 32767.0); }
    template<> half convert(int32_t value) { return  half_cast<half>(value / 2147483647.0); }
    template<> half convert(int64_t value) { return  half_cast<half>(value / static_cast<double>(0x7FFFFFFFFFFFFFFF)); }
    template<> half convert(half value) { return value; }
    template<> half convert(float value) { return half_cast<half>(value); }
    template<> half convert(double value) { return half_cast<half>(value); }

    TEST_CLASS(BigConversionTest)
    {
    public:

        TEST_METHOD(BigConversionTest_uint8_t)
        {
            // uint8_t
            {
                std::vector<uint8_t> val = { 255, 128, 127, 64, 63, 1, 0 };
                std::vector<uint8_t> exp = { 255, 128, 127, 64, 63, 1, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint8_t w1 = convert<uint8_t, uint8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint16_t
            {
                std::vector<uint16_t> val = { 65535, 65280, 65279, 32768, 32767, 1, 0 };
                std::vector<uint8_t> exp = { 255, 255, 254, 128, 127, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint8_t w1 = convert<uint8_t, uint16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint32_t
            {
                std::vector<uint32_t> val = { 4294967295, 4278190080, 4278190079, 2147483648, 2147483647, 1, 0 };
                std::vector<uint8_t> exp = { 255, 255, 254, 128, 127, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint8_t w1 = convert<uint8_t, uint32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint64_t
            {
                std::vector<uint64_t> val = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 9223372036854775808, 9223372036854775807, 1, 0 };
                std::vector<uint8_t> exp = { 255, 255, 254, 128, 127, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint8_t w1 = convert<uint8_t, uint64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int8_t
            {
                std::vector<int8_t> val = { 127, 64, 63, 1, 0, -1, -63, -64, -128 };
                std::vector<uint8_t> exp = { 255, 129, 126, 2, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint8_t w1 = convert<uint8_t, int8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int16_t
            {
                std::vector<int16_t> val = { 32767, 16384, 16383, 1, 0, -1, -16383, -16384, -32768 };
                std::vector<uint8_t> exp = { 255, 128, 127, 0, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint8_t w1 = convert<uint8_t, int16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int32_t
            {
                std::vector<int32_t> val = { 0x7FFFFFFF, 0x40000000, 0x3FFFFFFF, 1, 0, -1 };
                std::vector<uint8_t> exp = { 255, 128, 127, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint8_t w1 = convert<uint8_t, int32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int64_t
            {
                std::vector<int64_t> val = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1 };
                std::vector<uint8_t> exp = { 255, 128, 127, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint8_t w1 = convert<uint8_t, int64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // half
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<uint8_t> exp = { 255, 255, 252, 25, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint8_t w1 = convert<uint8_t, half>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // float
            {
                std::vector<float> val = { 2.0f, 1.0f, 0.99999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };
                std::vector<uint8_t> exp = { 255, 255, 255, 252, 26, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint8_t w1 = convert<uint8_t, float>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // double
            {
                std::vector<double> val = { 2.0, 1.0, 0.9999999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                std::vector<uint8_t> exp = { 255, 255, 255, 252, 26, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint8_t w1 = convert<uint8_t, double>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
        }

        TEST_METHOD(BigConversionTest_uint16_t)
        {
            // uint8_t
            {
                std::vector<uint8_t> val = { 255, 128, 127, 64, 63, 1, 0 };
                std::vector<uint16_t> exp = { 65535, 32896, 32639, 16448, 16191, 257, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint16_t w1 = convert<uint16_t, uint8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint16_t
            {
                std::vector<uint16_t> val = { 65535, 65280, 65279, 32768, 32767, 1, 0 };
                std::vector<uint16_t> exp = { 65535, 65280, 65279, 32768, 32767, 1, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint16_t w1 = convert<uint16_t, uint16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint32_t
            {
                std::vector<uint32_t> val = { 4294967295, 4278190080, 4278190079, 2147483648, 2147483647, 1, 0 };
                std::vector<uint16_t> exp = { 65535, 65280, 65279, 32768, 32767, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint16_t w1 = convert<uint16_t, uint32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint64_t
            {
                std::vector<uint64_t> val = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 9223372036854775808, 9223372036854775807, 1, 0 };
                std::vector<uint16_t> exp = { 65535, 65280, 65279, 32768, 32767, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint16_t w1 = convert<uint16_t, uint64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int8_t
            {
                std::vector<int8_t> val = { 127, 64, 63, 1, 0, -1, -63, -64, -128 };
                std::vector<uint16_t> exp = { 65535, 33153, 32382, 514, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint16_t w1 = convert<uint16_t, int8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int16_t
            {
                std::vector<int16_t> val = { 32767, 16384, 16383, 1, 0, -1, -16383, -16384, -32768 };
                std::vector<uint16_t> exp = { 65535, 32769, 32766, 2, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint16_t w1 = convert<uint16_t, int16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int32_t
            {
                std::vector<int32_t> val = { 0x7FFFFFFF, 0x40000000, 0x3FFFFFFF, 1, 0, -1 };
                std::vector<uint16_t> exp = { 65535, 32768, 32767, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint16_t w1 = convert<uint16_t, int32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int64_t
            {
                std::vector<int64_t> val = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1 };
                std::vector<uint16_t> exp = { 65535, 32768, 32767, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint16_t w1 = convert<uint16_t, int64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // half
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<uint16_t> exp = { 0xFFFF, 0xFFFF, 64863, 6552, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint16_t w1 = convert<uint16_t, half>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // float
            {
                std::vector<float> val = { 2.0f, 1.0f, 0.99999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };
                std::vector<uint16_t> exp = { 65535, 65535, 65535, 64880, 6554, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint16_t w1 = convert<uint16_t, float>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // double
            {
                std::vector<double> val = { 2.0, 1.0, 0.9999999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                std::vector<uint16_t> exp = { 65535, 65535, 65535, 64880, 6554, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint16_t w1 = convert<uint16_t, double>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
        }

        TEST_METHOD(BigConversionTest_uint32_t)
        {
            // uint8_t
            {
                std::vector<uint8_t> val = { 255, 128, 127, 64, 63, 1, 0 };
                std::vector<uint32_t> exp = { 0xFFFFFFFF, 0x80808080, 0x7F7F7F7F, 0x40404040, 0x3F3F3F3F, 0x01010101, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint32_t w1 = convert<uint32_t, uint8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint16_t
            {
                std::vector<uint16_t> val = { 65535, 65280, 65279, 32768, 32767, 1, 0 };
                std::vector<uint32_t> exp = { 0xFFFFFFFF, 0xFF00FF00, 0xFEFFFEFF, 0x80008000, 0x7FFF7FFF, 0x00010001, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint32_t w1 = convert<uint32_t, uint16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint32_t
            {
                std::vector<uint32_t> val = { 4294967295, 4278190080, 4278190079, 2147483648, 2147483647, 1, 0 };
                std::vector<uint32_t> exp = { 4294967295, 4278190080, 4278190079, 2147483648, 2147483647, 1, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint32_t w1 = convert<uint32_t, uint32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint64_t
            {
                std::vector<uint64_t> val = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 9223372036854775808, 9223372036854775807, 1, 0 };
                std::vector<uint32_t> exp = { 0xFFFFFFFF, 0xFF000000, 0xFEFFFFFF, 0x80000000, 0x7FFFFFFF, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint32_t w1 = convert<uint32_t, uint64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int8_t
            {
                std::vector<int8_t> val = { 127, 64, 63, 1, 0, -1, -63, -64, -128 };
                std::vector<uint32_t> exp = { 0xFFFFFFFF, 0x81818181, 0x7E7E7E7E, 0x02020202, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint32_t w1 = convert<uint32_t, int8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int16_t
            {
                std::vector<int16_t> val = { 32767, 16384, 16383, 1, 0, -1, -16383, -16384, -32768 };
                std::vector<uint32_t> exp = { 0xFFFFFFFF, 0x80018001, 0x7FFE7FFE, 0x00020002, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint32_t w1 = convert<uint32_t, int16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int32_t
            {
                std::vector<int32_t> val = { 0x7FFFFFFF, 0x40000000, 0x3FFFFFFF, 1, 0, -1 };
                std::vector<uint32_t> exp = { 0xFFFFFFFF, 0x80000001, 0x7FFFFFFE, 2, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint32_t w1 = convert<uint32_t, int32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int64_t
            {
                std::vector<int64_t> val = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1 };
                std::vector<uint32_t> exp = { 0xFFFFFFFF, 0x80000000, 0x7FFFFFFF, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint32_t w1 = convert<uint32_t, int64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // half
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<uint32_t> exp = { 0xFFFFFFFF, 0xFFFFFFFF, 4250927103, 429391872, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint32_t w1 = convert<uint32_t, half>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // float
            {
                std::vector<float> val = { 2.0f, 1.0f, 0.99999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };
                std::vector<uint32_t> exp = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 4252017663, 429496736, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint32_t w1 = convert<uint32_t, float>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // double
            {
                std::vector<double> val = { 2.0, 1.0, 0.9999999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                std::vector<uint32_t> exp = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 4252017622, 429496730, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint32_t w1 = convert<uint32_t, double>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
        }

        TEST_METHOD(BigConversionTest_uint64_t)
        {
            // uint8_t
            {
                std::vector<uint8_t> val = { 255, 128, 127, 64, 63, 1, 0 };
                std::vector<uint64_t> exp = { 0xFFFFFFFFFFFFFFFF, 0x8080808080808080, 0x7F7F7F7F7F7F7F7F, 0x4040404040404040, 0x3F3F3F3F3F3F3F3F, 0x0101010101010101, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint64_t w1 = convert<uint64_t, uint8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint16_t
            {
                std::vector<uint16_t> val = { 65535, 65280, 65279, 32768, 32767, 1, 0 };
                std::vector<uint64_t> exp = { 0xFFFFFFFFFFFFFFFF, 0xFF00FF00FF00FF00, 0xFEFFFEFFFEFFFEFF, 0x8000800080008000, 0x7FFF7FFF7FFF7FFF, 0x0001000100010001, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint64_t w1 = convert<uint64_t, uint16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint32_t
            {
                std::vector<uint32_t> val = { 4294967295, 4278190080, 4278190079, 2147483648, 2147483647, 1, 0 };
                std::vector<uint64_t> exp = { 0xFFFFFFFFFFFFFFFF, 0xFF000000FF000000, 0xFEFFFFFFFEFFFFFF, 0x8000000080000000, 0x7FFFFFFF7FFFFFFF, 0x100000001, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint64_t w1 = convert<uint64_t, uint32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint64_t
            {
                std::vector<uint64_t> val = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 9223372036854775808, 9223372036854775807, 1, 0 };
                std::vector<uint64_t> exp = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 9223372036854775808, 9223372036854775807, 1, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint64_t w1 = convert<uint64_t, uint64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int8_t
            {
                std::vector<int8_t> val = { 127, 64, 63, 1, 0, -1, -63, -64, -128 };
                std::vector<uint64_t> exp = { 0xFFFFFFFFFFFFFFFF, 0x8181818181818181, 0x7E7E7E7E7E7E7E7E, 0x0202020202020202, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint64_t w1 = convert<uint64_t, int8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int16_t
            {
                std::vector<int16_t> val = { 32767, 16384, 16383, 1, 0, -1, -16383, -16384, -32768 };
                std::vector<uint64_t> exp = { 0xFFFFFFFFFFFFFFFF, 0x8001800180018001, 0x7FFE7FFE7FFE7FFE, 0x0002000200020002, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint64_t w1 = convert<uint64_t, int16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int32_t
            {
                std::vector<int32_t> val = { 0x7FFFFFFF, 0x40000000, 0x3FFFFFFF, 1, 0, -1 };
                std::vector<uint64_t> exp = { 0xFFFFFFFFFFFFFFFF, 0x8000000180000001, 0x7FFFFFFE7FFFFFFE, 0x200000002, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint64_t w1 = convert<uint64_t, int32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int64_t
            {
                std::vector<int64_t> val = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1 };
                std::vector<uint64_t> exp = { 0xFFFFFFFFFFFFFFFF, 0x8000000000000001, 0x7FFFFFFFFFFFFFFE, 2, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint64_t w1 = convert<uint64_t, int64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // half
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<uint64_t> exp = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 18257592889359990762, 1844224047408216268, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint64_t w1 = convert<uint64_t, half>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // float
            {
                std::vector<float> val = { 2.0f, 1.0f, 0.99999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };
                std::vector<uint64_t> exp = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFD70A3FFFFFFFFEB, 0x1999999FFFFFF8CC, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint64_t w1 = convert<uint64_t, float>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // double
            {
                std::vector<double> val = { 2.0, 1.0, 0.99999999999999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                std::vector<uint64_t> exp = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFD70A3D70A3D6FEB, 0x19999999999998CC, 0, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    uint64_t w1 = convert<uint64_t, double>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
        }

        TEST_METHOD(BigConversionTest_int8_t)
        {
            // uint8_t
            {
                std::vector<uint8_t> val = { 255, 128, 127, 64, 63, 2, 1, 0 };
                std::vector<int8_t> exp = { 127, 64, 63, 32, 31, 1, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int8_t w1 = convert<int8_t, uint8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint16_t
            {
                std::vector<uint16_t> val = { 65535, 65280, 65279, 32768, 32767, 512, 511, 2, 1, 0 };
                std::vector<int8_t> exp = { 127, 127, 127, 64, 63, 1, 0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int8_t w1 = convert<int8_t, uint16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint32_t
            {
                std::vector<uint32_t> val = { 0xFFFFFFFF, 0xFF000000, 0xFE000000, 2147483648, 2147483647, 1, 0 };
                std::vector<int8_t> exp = { 127, 127, 127, 64, 63, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int8_t w1 = convert<int8_t, uint32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint64_t
            {
                std::vector<uint64_t> val = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 9223372036854775808, 9223372036854775807, 1, 0 };
                std::vector<int8_t> exp = { 127, 127, 127, 64, 63, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int8_t w1 = convert<int8_t, uint64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int8_t
            {
                std::vector<int8_t> val = { 127, 64, 63, 1, 0, -1, -63, -64, -128 };
                std::vector<int8_t> exp = { 127, 64, 63, 1, 0, -1, -63, -64, -128 };
                for (int i = 0; i != val.size(); ++i) {
                    int8_t w1 = convert<int8_t, int8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int16_t
            {
                std::vector<int16_t> val = { 32767, 16384, 16383, 1, 0, -1, -16128, -16129, -16383, -16384, -32767, -32768 };
                std::vector<int8_t> exp = { 127, 64, 63, 0, 0, -1, -63, -64, -64, -64, -128, -128 };
                for (int i = 0; i != val.size(); ++i) {
                    int8_t w1 = convert<int8_t, int16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int32_t
            {
                std::vector<int32_t> val = { 2147483647, 0x40000000, 0x3FFFFFFF, 1, 0, -1, -2147483648ll };
                std::vector<int8_t> exp = { 127, 64, 63, 0, 0, -1, -128 };
                for (int i = 0; i != val.size(); ++i) {
                    int8_t w1 = convert<int8_t, int32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int64_t
            {
                std::vector<int64_t> val = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1, static_cast<int64_t>(0x8000000000000000ull) };
                std::vector<int8_t> exp = { 127, 64, 63, 0, 0, -1, -128 };
                for (int i = 0; i != val.size(); ++i) {
                    int8_t w1 = convert<int8_t, int64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // half
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<int8_t> exp = { 127, 127, 126, 13, 0, -13, -126, -127, -127 };
                for (int i = 0; i != val.size(); ++i) {
                    int8_t w1 = convert<int8_t, half>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // float
            {
                std::vector<float> val = { 2.0f, 1.0f, 0.99999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };
                std::vector<int8_t> exp = { 127, 127, 127, 126, 13, 0, -13, -126, -127, -127 };
                for (int i = 0; i != val.size(); ++i) {
                    int8_t w1 = convert<int8_t, float>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // double
            {
                std::vector<double> val = { 2.0, 1.0, 0.9999999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                std::vector<int8_t> exp = { 127, 127, 127, 126, 13, 0, -13, -126, -127, -127 };
                for (int i = 0; i != val.size(); ++i) {
                    int8_t w1 = convert<int8_t, double>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
        }

        TEST_METHOD(BigConversionTest_int16_t)
        {
            // uint8_t
            {
                std::vector<uint8_t> val = { 255, 128, 127, 64, 63, 2, 1, 0 };
                std::vector<int16_t> exp = { 32767, 16512, 16383, 8256, 8127, 258, 129, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int16_t w1 = convert<int16_t, uint8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint16_t
            {
                std::vector<uint16_t> val = { 65535, 65280, 65279, 32768, 32767, 512, 511, 2, 1, 0 };
                std::vector<int16_t> exp = { 32767, 32640, 32639, 16384, 16383, 256, 255, 1, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int16_t w1 = convert<int16_t, uint16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint32_t
            {
                std::vector<uint32_t> val = { 0xFFFFFFFF, 0xFF000000, 0xFE000000, 0x80000000, 0x7FFFFFFF, 1, 0 };
                std::vector<int16_t> exp = { 0x7FFF, 0x7F80, 0x7F00, 0x4000, 0x3FFF, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int16_t w1 = convert<int16_t, uint32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint64_t
            {
                std::vector<uint64_t> val = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 0x8000000000000000, 0x7FFFFFFFFFFFFFFF, 1, 0 };
                std::vector<int16_t> exp = { 0x7FFF, 0x7F80, 0x7F7F, 0x4000, 0x3FFF, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int16_t w1 = convert<int16_t, uint64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int8_t
            {
                std::vector<int8_t> val = { 127, 64, 63, 1, 0, -1, -63, -64, -126, -127, -128 };
                std::vector<int16_t> exp = { 0x7FFF, 16513, 16254, 258, 0, -256, -16128, -16384, -32256, -32512, -32768 };
                for (int i = 0; i != val.size(); ++i) {
                    int16_t w1 = convert<int16_t, int8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int16_t
            {
                std::vector<int16_t> val = { 32767, 16384, 16383, 1, 0, -1, -16128, -16129, -16383, -16384, -32767, -32768 };
                std::vector<int16_t> exp = { 32767, 16384, 16383, 1, 0, -1, -16128, -16129, -16383, -16384, -32767, -32768 };
                for (int i = 0; i != val.size(); ++i) {
                    int16_t w1 = convert<int16_t, int16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int32_t
            {
                std::vector<int32_t> val = { 2147483647, 0x40000000, 0x3FFFFFFF, 1, 0, -1, -65536, -65537, -2147483648ll };
                std::vector<int16_t> exp = { 0x7FFF, 0x4000, 0x3FFF, 0, 0, -1, -1, -2, -32768 };
                for (int i = 0; i != val.size(); ++i) {
                    int16_t w1 = convert<int16_t, int32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int64_t
            {
                std::vector<int64_t> val = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1, -281474976710656ll, -281474976710657ll, static_cast<int64_t>(0x8000000000000000ull) };
                std::vector<int16_t> exp = { 0x7FFF, 0x4000, 0x3FFF, 0, 0, -1, -1, -2, -32768 };
                for (int i = 0; i != val.size(); ++i) {
                    int16_t w1 = convert<int16_t, int64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // half
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<int16_t> exp = { 32767, 32767, 32431, 3276, 0, -3276, -32431, -32767, -32767 };
                for (int i = 0; i != val.size(); ++i) {
                    int16_t w1 = convert<int16_t, half>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // float
            {
                std::vector<float> val = { 2.0f, 1.0f, 0.99999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };
                std::vector<int16_t> exp = { 32767, 32767, 32767, 32439, 3277, 0, -3277, -32439, -32767, -32767 };
                for (int i = 0; i != val.size(); ++i) {
                    int16_t w1 = convert<int16_t, float>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // double
            {
                std::vector<double> val = { 2.0, 1.0, 0.9999999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                std::vector<int16_t> exp = { 32767, 32767, 32767, 32439, 3277, 0, -3277, -32439, -32767, -32767 };
                for (int i = 0; i != val.size(); ++i) {
                    int16_t w1 = convert<int16_t, double>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
        }

        TEST_METHOD(BigConversionTest_int32_t)
        {
            // uint8_t
            {
                std::vector<uint8_t> val = { 255, 128, 127, 64, 63, 2, 1, 0 };
                std::vector<int32_t> exp = { 0x7FFFFFFF, 0x40404080, 0x3FBFBFFF, 0x20202040, 0x1F9F9FBF, 0x01010102, 0x00808081, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int32_t w1 = convert<int32_t, uint8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint16_t
            {
                std::vector<uint16_t> val = { 65535, 65280, 65279, 32768, 32767, 512, 511, 2, 1, 0 };
                std::vector<int32_t> exp = { 0x7FFFFFFF, 0x7F80FF00, 0x7F7FFEFF, 0x40008000, 0x3FFFFFFF, 0x1000200, 0xFF81FF, 0x10002, 0x8001, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int32_t w1 = convert<int32_t, uint16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint32_t
            {
                std::vector<uint32_t> val = { 0xFFFFFFFF, 0xFF000000, 0xFE000000, 0x80000000, 0x7FFFFFFF, 1, 0 };
                std::vector<int32_t> exp = { 0x7FFFFFFF, 0x7F800000, 0x7F000000, 0x40000000, 0x3FFFFFFF, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int32_t w1 = convert<int32_t, uint32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint64_t
            {
                std::vector<uint64_t> val = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 0x8000000000000000, 0x7FFFFFFFFFFFFFFF, 1, 0 };
                std::vector<int32_t> exp = { 0x7FFFFFFF, 0x7F800000, 0x7F7FFFFF, 0x40000000, 0x3FFFFFFF, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int32_t w1 = convert<int32_t, uint64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int8_t
            {
                std::vector<int8_t> val = { 127, 64, 63, 1, 0, -1, -63, -64, -126, -127, -128 };
                std::vector<int32_t> exp = { 0x7FFFFFFF, 0x40810204, 0x3F7EFDFB, 0x1020408, 0, -16777216, static_cast<int32_t>(0xC1000000), static_cast<int32_t>(0xC0000000), static_cast<int32_t>(0x82000000), static_cast<int32_t>(0x81000000), static_cast<int32_t>(0x80000000) };
                for (int i = 0; i != val.size(); ++i) {
                    int32_t w1 = convert<int32_t, int8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int16_t
            {
                std::vector<int16_t> val = { 32767, 16384, 16383, 1, 0, -1, -16128, -16129, -16383, -16384, -32767, -32768 };
                std::vector<int32_t> exp = { 0x7FFFFFFF, 0x40008001, 0x3FFF7FFE, 0x10002, 0, static_cast<int32_t>(0xFFFF0000), static_cast<int32_t>(0xC1000000), static_cast<int32_t>(0xC0FF0000), static_cast<int32_t>(0xC0010000), static_cast<int32_t>(0xC0000000), static_cast<int32_t>(0x80010000), static_cast<int32_t>(0x80000000) };
                for (int i = 0; i != val.size(); ++i) {
                    int32_t w1 = convert<int32_t, int16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int32_t
            {
                std::vector<int32_t> val = { 2147483647, 0x40000000, 0x3FFFFFFF, 1, 0, -1, -65536, -65537, -2147483648ll };
                std::vector<int32_t> exp = { 2147483647, 0x40000000, 0x3FFFFFFF, 1, 0, -1, -65536, -65537, -2147483648ll };
                for (int i = 0; i != val.size(); ++i) {
                    int32_t w1 = convert<int32_t, int32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int64_t
            {
                std::vector<int64_t> val = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1, -4294967296ll, -4294967297ll, static_cast<int64_t>(0x8000000000000000ull) };
                std::vector<int32_t> exp = { 0x7FFFFFFF, 0x40000000, 0x3FFFFFFF, 0, 0, -1, -1, -2, -2147483648ll };
                for (int i = 0; i != val.size(); ++i) {
                    int32_t w1 = convert<int32_t, int64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // half
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<int32_t> exp = { 0x7FFFFFFF, 0x7FFFFFFF, 2125463551, 214695936, 0, -214695936, -2125463551, -2147483647, -2147483647 };
                for (int i = 0; i != val.size(); ++i) {
                    int32_t w1 = convert<int32_t, half>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // float
            {
                std::vector<float> val = { 2.0f, 1.0f, 0.99999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };
                std::vector<int32_t> exp = { 2147483647, 2147483647, 2147483647, 2126008831, 214748368, 0, -214748368, -2126008831, -2147483647, -2147483647 };
                for (int i = 0; i != val.size(); ++i) {
                    int32_t w1 = convert<int32_t, float>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // double
            {
                std::vector<double> val = { 2.0, 1.0, 0.9999999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                std::vector<int32_t> exp = { 2147483647, 2147483647, 2147483647, 2126008811, 214748365, 0, -214748365, -2126008811, -2147483647, -2147483647 };
                for (int i = 0; i != val.size(); ++i) {
                    int32_t w1 = convert<int32_t, double>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
        }

        TEST_METHOD(BigConversionTest_int64_t)
        {
            // uint8_t
            {
                std::vector<uint8_t> val = { 255, 128, 127, 64, 63, 2, 1, 0 };
                std::vector<int64_t> exp = { 0x7FFFFFFFFFFFFFFF, 0x4040404040404080, 0x3FBFBFBFBFBFBFFF, 0x2020202020202040, 0x1F9F9F9F9F9F9FBF, 0x0101010101010102, 0x0080808080808081, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int64_t w1 = convert<int64_t, uint8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint16_t
            {
                std::vector<uint16_t> val = { 65535, 65280, 65279, 32768, 32767, 512, 511, 2, 1, 0 };
                std::vector<int64_t> exp = { 0x7FFFFFFFFFFFFFFF, 0x7F807F807F80FF00, 0x7F7FFF7FFF7FFEFF, 0x4000400040008000, 0x3FFFBFFFBFFFFFFF, 0x0100010001000200, 0x00FF80FF80FF81FF, 0x0001000100010002, 0x800080008001, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int64_t w1 = convert<int64_t, uint16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint32_t
            {
                std::vector<uint32_t> val = { 0xFFFFFFFF, 0xFF000000, 0xFE000000, 0x80000000, 0x7FFFFFFF, 1, 0 };
                std::vector<int64_t> exp = { 0x7FFFFFFFFFFFFFFF, 0x7F800000FF000000, 0x7F000000FE000000, 0x4000000080000000, 0x3FFFFFFFFFFFFFFF, 0x80000001, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int64_t w1 = convert<int64_t, uint32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint64_t
            {
                std::vector<uint64_t> val = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 0x8000000000000000, 0x7FFFFFFFFFFFFFFF, 2, 1, 0 };
                std::vector<int64_t> exp = { 0x7FFFFFFFFFFFFFFF, 0x7F80000000000000, 0x7F7FFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    int64_t w1 = convert<int64_t, uint64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int8_t
            {
                std::vector<int8_t> val = { 127, 64, 63, 1, 0, -1, -63, -64, -126, -127, -128 };
                std::vector<int64_t> exp = { 0x7FFFFFFFFFFFFFFF, 0x4081020408102040, 0x3F7EFDFBF7EFDFBF, 0x102040810204081, 0, static_cast<int64_t>(0xFF00000000000000), static_cast<int64_t>(0xC100000000000000), static_cast<int64_t>(0xC000000000000000), static_cast<int64_t>(0x8200000000000000), static_cast<int64_t>(0x8100000000000000), static_cast<int64_t>(0x8000000000000000) };
                for (int i = 0; i != val.size(); ++i) {
                    int64_t w1 = convert<int64_t, int8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int16_t
            {
                std::vector<int16_t> val = { 32767, 16384, 16383, 1, 0, -1, -16128, -16129, -16383, -16384, -32767, -32768 };
                std::vector<int64_t> exp = { 0x7FFFFFFFFFFFFFFF, 0x4000800100020004, 0x3FFF7FFEFFFDFFFB, 0x1000200040008, 0, static_cast<int64_t>(0xFFFF000000000000), static_cast<int64_t>(0xC100000000000000), static_cast<int64_t>(0xC0FF000000000000), static_cast<int64_t>(0xC001000000000000), static_cast<int64_t>(0xC000000000000000), static_cast<int64_t>(0x8001000000000000), static_cast<int64_t>(0x8000000000000000) };
                for (int i = 0; i != val.size(); ++i) {
                    int64_t w1 = convert<int64_t, int16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int32_t
            {
                std::vector<int32_t> val = { 2147483647, 0x40000000, 0x3FFFFFFF, 1, 0, -1, -65536, -65537, -2147483648ll };
                std::vector<int64_t> exp = { 0x7FFFFFFFFFFFFFFF, 0x4000000080000001, 0x3FFFFFFF7FFFFFFE, 0x100000002, 0, static_cast<int64_t>(0xFFFFFFFF00000000), static_cast<int64_t>(0xFFFF000000000000), static_cast<int64_t>(0xFFFEFFFF00000000), static_cast<int64_t>(0x8000000000000000) };
                for (int i = 0; i != val.size(); ++i) {
                    int64_t w1 = convert<int64_t, int32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int64_t
            {
                std::vector<int64_t> val = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1, -4294967296ll, -4294967297ll, static_cast<int64_t>(0x8000000000000000ull) };
                std::vector<int64_t> exp = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1, -4294967296ll, -4294967297ll, static_cast<int64_t>(0x8000000000000000ull) };
                for (int i = 0; i != val.size(); ++i) {
                    int64_t w1 = convert<int64_t, int64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // half
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<int64_t> exp = { 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 9128796444679995381, 922112023704109158, 0, -922112023704109159, -9128796444679995382, static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000000) };
                for (int i = 0; i != val.size(); ++i) {
                    int64_t w1 = convert<int64_t, half>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // float
            {
                std::vector<float> val = { 2.0f, 1.0f, 0.99999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };
                std::vector<int64_t> exp = { 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 9223372036854775807, 9131138404447158261, 922337217429373030, 0, -922337217429373031, -9131138404447158262, static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000000) };
                for (int i = 0; i != val.size(); ++i) {
                    int64_t w1 = convert<int64_t, float>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // double
            {
                std::vector<double> val = { 2.0, 1.0, 0.9999999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                std::vector<int64_t> exp = { 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 9223372035932438527, 9131138316486227957, 922337203685477478, 0, -922337203685477479, -9131138316486227958, static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000000) };
                for (int i = 0; i != val.size(); ++i) {
                    int64_t w1 = convert<int64_t, double>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
        }

        TEST_METHOD(BigConversionTest_float)
        {
            // uint8_t
            {
                std::vector<uint8_t> val = { 255, 128, 127, 64, 63, 2, 1, 0 };
                std::vector<float> exp = { 1.0f, 128 / 255.0f, 127 / 255.0f, 64 / 255.0f, 63 / 255.0f, 2 / 255.0f, 1 / 255.0f, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    float w1 = convert<float, uint8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint16_t
            {
                std::vector<uint16_t> val = { 65535, 65280, 65279, 32768, 32767, 512, 511, 2, 1, 0 };
                std::vector<float> exp = { 65535 / 65535.0f, 65280 / 65535.0f, 65279 / 65535.0f, 32768 / 65535.0f, 32767 / 65535.0f, 512 / 65535.0f, 511 / 65535.0f, 2 / 65535.0f, 1 / 65535.0f, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    float w1 = convert<float, uint16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint32_t
            {
                std::vector<uint32_t> val = { 0xFFFFFFFF, 0xFF000000, 0xFE000000, 0x80000000, 0x7FFFFFFF, 1, 0 };
                std::vector<float> exp = { 1.0, 0xFF000000 / 4294967295.0f, 0xFE000000 / 4294967295.0f, 0x80000000 / 4294967295.0f, 0x7FFFFFFF / 4294967295.0f, 1 / 4294967295.0f, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    float w1 = convert<float, uint32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint64_t
            {
                std::vector<uint64_t> val = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 0x8000000000000000, 0x7FFFFFFFFFFFFFFF, 2, 1, 0 };
                std::vector<float> exp = { 1.0, 0x0000000000FF0000 / 16777215.0f, 0x0000000000FEFFFF / 16777215.0f, 0x0000000000800000 / 16777215.0f, 0x00000000007FFFFF / 16777215.0f, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    float w1 = convert<float, uint64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int8_t
            {
                std::vector<int8_t> val = { 127, 64, 63, 1, 0, -1, -63, -64, -126, -127, -128 };
                std::vector<float> exp = { 1.0f, 64 / 127.0f, 63 / 127.0f, 1 / 127.0f, 0, -1 / 127.0f, -63 / 127.0f, -64 / 127.0f, -126 / 127.0f, -1.0f, -128 / 127.0f };
                for (int i = 0; i != val.size(); ++i) {
                    float w1 = convert<float, int8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int16_t
            {
                std::vector<int16_t> val = { 32767, 16384, 16383, 1, 0, -1, -16128, -16129, -16383, -16384, -32767, -32768 };
                std::vector<float> exp = { 1.0f, 16384 / 32767.0f, 16383 / 32767.0f, 1 / 32767.0f, 0, -1 / 32767.0f, -16128 / 32767.0f, -16129 / 32767.0f, -16383 / 32767.0f, -16384 / 32767.0f, -1.0f, -32768 / 32767.0f };
                for (int i = 0; i != val.size(); ++i) {
                    float w1 = convert<float, int16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int32_t
            {
                std::vector<int32_t> val = { 2147483647, 0x40000000, 0x3FFFFFFF, 1, 0, -1, -65536, -65537, -2147483647, -2147483648ll };
                std::vector<float> exp = { 1.0f, 0x40000000 / 2147483647.0f, 0x3FFFFFFF / 2147483647.0f, 1 / 2147483647.0f, 0, -1 / 2147483647.0f, -65536 / 2147483647.0f, -65537 / 2147483647.0f, -1.0f, -2147483648ll / 2147483647.0f };
                for (int i = 0; i != val.size(); ++i) {
                    float w1 = convert<float, int32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int64_t
            {
                std::vector<int64_t> val = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1, -4294967296ll, -4294967297ll, static_cast<int64_t>(0x8000000000000000ull) };
                std::vector<float> exp = { 1.0f, 0x4000000000000000 / static_cast<float>(0x7FFFFFFFFFFFFFFF), 0x3FFFFFFFFFFFFFFF / static_cast<float>(0x7FFFFFFFFFFFFFFF), 1 / static_cast<float>(0x7FFFFFFFFFFFFFFF), 0, -1 / static_cast<float>(0x7FFFFFFFFFFFFFFF), -4294967296ll / static_cast<float>(0x7FFFFFFFFFFFFFFF), -4294967297ll / static_cast<float>(0x7FFFFFFFFFFFFFFF), -1.0f };
                for (int i = 0; i != val.size(); ++i) {
                    float w1 = convert<float, int64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // half
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<float> exp = { 2.0f, 1.0f, static_cast<float>(0.99_h), static_cast<float>(0.1_h), 0, static_cast<float>(-0.1_h), static_cast<float>(-0.99_h), -1.0f, -2.0f };
                for (int i = 0; i != val.size(); ++i) {
                    float w1 = convert<float, half>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // float
            {
                std::vector<float> val = { 2.0f, 1.0f, 0.99999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };
                std::vector<float> exp = { 2.0f, 1.0f, 0.99999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };;
                for (int i = 0; i != val.size(); ++i) {
                    float w1 = convert<float, float>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // double
            {
                std::vector<double> val = { 2.0, 1.0, 0.9999999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                std::vector<float> exp = { 2.0f, 1.0f, 0.9999999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };
                for (int i = 0; i != val.size(); ++i) {
                    float w1 = convert<float, double>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
        }

        TEST_METHOD(BigConversionTest_double)
        {
            // uint8_t
            {
                std::vector<uint8_t> val = { 255, 128, 127, 64, 63, 2, 1, 0 };
                std::vector<double> exp = { 1.0, 0.50196078431372549019607843137255, 0.49803921568627450980392156862745, 0.25098039215686274509803921568627, 0.24705882352941176470588235294118, 0.0078431372549019607843137254902, 0.0039215686274509803921568627451, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    double w1 = convert<double, uint8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint16_t
            {
                std::vector<uint16_t> val = { 65535, 65280, 65279, 32768, 32767, 512, 511, 2, 1, 0 };
                std::vector<double> exp = { 65535 / 65535.0, 65280 / 65535.0, 65279 / 65535.0, 32768 / 65535.0, 32767 / 65535.0, 512 / 65535.0, 511 / 65535.0, 2 / 65535.0, 1 / 65535.0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    double w1 = convert<double, uint16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint32_t
            {
                std::vector<uint32_t> val = { 0xFFFFFFFF, 0xFF000000, 0xFE000000, 0x80000000, 0x7FFFFFFF, 1, 0 };
                std::vector<double> exp = { 1.0, 0xFF000000 / 4294967295.0, 0xFE000000 / 4294967295.0, 0x80000000 / 4294967295.0, 0x7FFFFFFF / 4294967295.0, 1 / 4294967295.0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    double w1 = convert<double, uint32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint64_t
            {
                std::vector<uint64_t> val = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 0x8000000000000000, 0x7FFFFFFFFFFFFFFF, 2, 1, 0 };
                std::vector<double> exp = { 1.0, 0x001FE00000000000 / 9007199254740991.0, 0x001FDFFFFFFFFFFF / 9007199254740991.0, 0x0010000000000000 / 9007199254740991.0, 0x000FFFFFFFFFFFFF / 9007199254740991.0, 0, 0, 0 };
                for (int i = 0; i != val.size(); ++i) {
                    double w1 = convert<double, uint64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int8_t
            {
                std::vector<int8_t> val = { 127, 64, 63, 1, 0, -1, -63, -64, -126, -127, -128 };
                std::vector<double> exp = { 1.0, 64 / 127.0, 63 / 127.0, 1 / 127.0, 0, -1 / 127.0, -63 / 127.0, -64 / 127.0, -126 / 127.0, -1.0, -128 / 127.0 };
                for (int i = 0; i != val.size(); ++i) {
                    double w1 = convert<double, int8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int16_t
            {
                std::vector<int16_t> val = { 32767, 16384, 16383, 1, 0, -1, -16128, -16129, -16383, -16384, -32767, -32768 };
                std::vector<double> exp = { 1.0, 16384 / 32767.0, 16383 / 32767.0, 1 / 32767.0, 0, -1 / 32767.0, -16128 / 32767.0, -16129 / 32767.0, -16383 / 32767.0, -16384 / 32767.0, -1.0, -32768 / 32767.0 };
                for (int i = 0; i != val.size(); ++i) {
                    double w1 = convert<double, int16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int32_t
            {
                std::vector<int32_t> val = { 2147483647, 0x40000000, 0x3FFFFFFF, 1, 0, -1, -65536, -65537, -2147483647, -2147483648ll };
                std::vector<double> exp = { 1.0, 0x40000000 / 2147483647.0, 0x3FFFFFFF / 2147483647.0, 1 / 2147483647.0, 0, -1 / 2147483647.0, -65536 / 2147483647.0, -65537 / 2147483647.0, -1.0, -2147483648ll / 2147483647.0 };
                for (int i = 0; i != val.size(); ++i) {
                    double w1 = convert<double, int32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int64_t
            {
                std::vector<int64_t> val = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1, -4294967296ll, -4294967297ll, static_cast<int64_t>(0x8000000000000000ull) };
                std::vector<double> exp = { 1.0, 0x4000000000000000 / static_cast<double>(0x7FFFFFFFFFFFFFFF), 0x3FFFFFFFFFFFFFFF / static_cast<double>(0x7FFFFFFFFFFFFFFF), 1 / static_cast<double>(0x7FFFFFFFFFFFFFFF), 0, -1 / static_cast<double>(0x7FFFFFFFFFFFFFFF), -4294967296ll / static_cast<double>(0x7FFFFFFFFFFFFFFF), -4294967297ll / static_cast<double>(0x7FFFFFFFFFFFFFFF), -1.0 };
                for (int i = 0; i != val.size(); ++i) {
                    double w1 = convert<double, int64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // half
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<double> exp = { 2.0, 1.0, static_cast<double>(0.99_h), static_cast<double>(0.1_h), 0.0, static_cast<double>(-0.1_h), static_cast<double>(-0.99_h), -1.0, -2.0 };
                for (int i = 0; i != val.size(); ++i) {
                    double w1 = convert<double, half>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // float 
            {
                std::vector<float> val = { 2.0f, 1.0f, 0.99999999f, 0.99f, 0.1f, 0, -0.1f, -0.99f, -1.0f, -2.0f };
                std::vector<double> exp = { 2.0, 1.0, 0.99999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                for (int i = 0; i != val.size(); ++i) {
                    double w1 = convert<double, float>(val[i]);
                    double deviation = exp[i] - w1;
                    bool isSame;
                    (abs(deviation) < 0.0000001) ? isSame = true : isSame = false;

                        
                    
                    Assert::AreEqual(isSame, true);
                }
            }
            // double
            {
                std::vector<double> val = { 2.0, 1.0, 0.9999999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                std::vector<double> exp = { 2.0, 1.0, 0.9999999999, 0.99, 0.1, 0, -0.1, -0.99, -1.0, -2.0 };
                for (int i = 0; i != val.size(); ++i) {
                    double w1 = convert<double, double>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
        }

        TEST_METHOD(BigConversionTest_half)
        {
            
            // uint8_t
            {
                std::vector<uint8_t> val = { 255, 128, 127, 64, 63, 2, 1, 0 };
                std::vector<half> exp = { 1.0_h, 0.502_h, 0.498_h, 0.251_h, 0.246948_h, 0.00784_h, 0.00392_h,  0.0_h};
                for (int i = 0; i != val.size(); ++i) {
                    half w1 = convert<half, uint8_t>(val[i]);
                      double deviation = exp[i] - w1;
                    bool isSame;
                    (abs(deviation) < 0.001) ? isSame = true : isSame = false; // deviaton only in 2 cases and is about 0,0004 
                    Assert::AreEqual(isSame, true);
                }
            }
            // uint16_t
            {
                std::vector<uint16_t> val = { 65535, 65280, 65279, 32768, 32767, 512, 511, 2, 1, 0 };
                std::vector<half> exp = { 65535.0_h / 65535.0_h, 65280.0_h / 65535.0_h, 65279.0_h / 65535.0_h, 32768.0_h / 65535.0_h, 32767.0_h / 65535.0_h, 512.0_h / 65535.0_h, 511.0_h / 65535.0_h, 2.0_h / 65535.0_h, 1.0_h / 65535.0_h, 0.0_h };
                for (int i = 0; i != val.size(); ++i) {
                    half w1 = convert<half, uint16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint32_t
            {
                std::vector<uint32_t> val = { 0xFFFFFFFF, 0xFF000000, 0xFE000000, 0x80000000, 0x7FFFFFFF, 1, 0 };
                std::vector<half> exp = { 1.0_h, half_cast<half>(4278190080 / 4294967295.0), half_cast<half>(4261412864 / 4294967295.0), half_cast<half>(2147483648 / 4294967295.0), half_cast<half>(2147483647 / 4294967295.0), half_cast<half>(1.0 / 4294967295.0), 0.0_h };
                for (int i = 0; i != val.size(); ++i) {
                    half w1 = convert<half, uint32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // uint64_t
            {
                std::vector<uint64_t> val = { 0xFFFFFFFFFFFFFFFF, 0xFF00000000000000, 0xFEFFFFFFFFFFFFFF, 0x8000000000000000, 0x7FFFFFFFFFFFFFFF, 2, 1, 0 };
                std::vector<half> exp = { 1.0_h, half_cast<half>(0x001FE00000000000 / 9007199254740991.0), half_cast<half>(0x001FDFFFFFFFFFFF / 9007199254740991.0), half_cast<half>(0x0010000000000000 / 9007199254740991.0), half_cast<half>(0x000FFFFFFFFFFFFF / 9007199254740991.0), 0.0_h, 0.0_h, 0.0_h };
                for (int i = 0; i != val.size(); ++i) {
                    half w1 = convert<half, uint64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int8_t
            {
                std::vector<int8_t> val = { 127, 64, 63, 1, 0, -1, -63, -64, -126, -127, -128 };
                std::vector<half> exp = { 1.0_h,  half_cast<half>(64 / 127.0),  half_cast<half>(63 / 127.0),  half_cast<half>(1 / 127.0), 0.0_h,  half_cast<half>(-1 / 127.0),  half_cast<half>(-63 / 127.0),  half_cast<half>(-64 / 127.0), -0.992_h, -1.0_h, half_cast<half>(-128 / 127.0) };
                for (int i = 0; i != val.size(); ++i) {
                    half w1 = convert<half, int8_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int16_t
            {
                std::vector<int16_t> val = { 32767, 16384, 16383, 1, 0, -1, -16128, -16129, -16383, -16384, -32767, -32768 };
                std::vector<half> exp = { 1.0_h, half_cast<half>(16384 / 32767.0), half_cast<half>(16383 / 32767.0), half_cast<half>(1 / 32767.0), 0.0_h, half_cast<half>(-1 / 32767.0), half_cast<half>(-16128 / 32767.0), half_cast<half>(-16129 / 32767.0), half_cast<half>(-16383 / 32767.0), half_cast<half>(-16384 / 32767.0), -1.0_h, half_cast<half>(-32768 / 32767.0) };
                for (int i = 0; i != val.size(); ++i) {
                    half w1 = convert<half, int16_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int32_t
            {
                std::vector<int32_t> val = { 2147483647, 0x40000000, 0x3FFFFFFF, 1, 0, -1, -65536, -65537, -2147483647, -2147483648ll };
                std::vector<half> exp = { 1.0_h, half_cast<half>(0x40000000 / 2147483647.0), half_cast<half>(0x3FFFFFFF / 2147483647.0), half_cast<half>(1 / 2147483647.0), 0.0_h, half_cast<half>(-1 / 2147483647.0), half_cast<half>(-65536 / 2147483647.0), half_cast<half>(-65537 / 2147483647.0), -1.0_h, half_cast<half>(-2147483648ll / 2147483647.0) };
                for (int i = 0; i != val.size(); ++i) {
                    half w1 = convert<half, int32_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // int64_t
            {
                std::vector<int64_t> val = { 0x7FFFFFFFFFFFFFFF, 0x4000000000000000, 0x3FFFFFFFFFFFFFFF, 1, 0, -1, -4294967296ll, -4294967297ll, static_cast<int64_t>(0x8000000000000000ull) };
                std::vector<half> exp = { 1.0_h,  half_cast<half>(0x4000000000000000 / static_cast<double>(0x7FFFFFFFFFFFFFFF)),  half_cast<half>(0x3FFFFFFFFFFFFFFF / static_cast<double>(0x7FFFFFFFFFFFFFFF)),  half_cast<half>(1 / static_cast<double>(0x7FFFFFFFFFFFFFFF)), 0.0_h,  half_cast<half>(-1 / static_cast<double>(0x7FFFFFFFFFFFFFFF)),  half_cast<half>(-4294967296ll / static_cast<double>(0x7FFFFFFFFFFFFFFF)),  half_cast<half>(-4294967297ll / static_cast<double>(0x7FFFFFFFFFFFFFFF)), -1.0_h };
                for (int i = 0; i != val.size(); ++i) {
                    half w1 = convert<half, int64_t>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }
            // half
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<half> exp = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                for (int i = 0; i != val.size(); ++i) {
                    half w1 = convert<half, half>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }

            // float 
            {
                std::vector<float> val = { 2.0f, 1.0f, static_cast<float>(0.99_h), static_cast<float>(0.1_h), 0, static_cast<float>(-0.1_h), static_cast<float>(-0.99_h), -1.0f, -2.0f };
                std::vector<half> exp = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                for (int i = 0; i != val.size(); ++i) {
                    half w1 = convert<half, float>(val[i]);
                    Assert::AreEqual(exp[i], w1);
                }
            }

            // double
            {
                std::vector<half> val = { 2.0_h, 1.0_h, 0.99_h, 0.1_h, 0.0_h, -0.1_h, -0.99_h, -1.0_h, -2.0_h };
                std::vector<double> exp = { 2.0, 1.0, static_cast<double>(0.99_h), static_cast<double>(0.1_h), 0.0, static_cast<double>(-0.1_h), static_cast<double>(-0.99_h), -1.0, -2.0 };
                for (int i = 0; i != exp.size(); ++i) {
                    half w1 = convert<half, double>(val[i]);
                    Assert::AreEqual(val[i], w1);
                }
            }
        
        
            
        
        }
    };
}
