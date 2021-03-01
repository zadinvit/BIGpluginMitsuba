#pragma once
#ifndef _BIG_CONSTS_H_
#ifdef _BIG_CONSTS_H_
#define BIGDLL  __declspec(dllexport)  
#else
#define BIGDLL  __declspec(dllimport)   
#endif
#define _BIG_CONSTS_H_

#include <cstdint>
#include <string>
#include <vector>

namespace big
{

    // default size of usable memory
    const uint64_t DEFAULT_CACHE_SIZE = 1024ull * 1024ull * 1024ull;

    // length of chunk
    const  uint64_t CHUNK_LENGTH = 8;

    // big magic number
    const std::string MAGIC = { 0x42, 0x49, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00 };

    // chunk ids
    enum class ChunkIds : uint64_t
    {
        NUMBER_OF_IMAGES = 1,
        IMAGE_HEIGHT = 2,
        IMAGE_WIDTH = 3,
        NUMBER_OF_PLANES = 4,
        DATA_ORDER = 5,
        DATA = 6,
        XML = 7
    };

    // core chunk ids in vector
    const std::vector<ChunkIds> CoreChunkIds = { ChunkIds::NUMBER_OF_IMAGES, ChunkIds::IMAGE_HEIGHT, ChunkIds::IMAGE_WIDTH, ChunkIds::NUMBER_OF_PLANES, ChunkIds::DATA_ORDER, ChunkIds::DATA }; //ChunkIds::XML,

    // data order ids
    //enum class DataOrderIds : uint64_t
    //{
    //    NUMBER_OF_IMAGES = 1,
    //    IMAGE_HEIGHT = 2,
    //    IMAGE_WIDTH = 3,
    //    NUMBER_OF_PLANES = 4,
    //};

    // default order of data
    const std::vector<ChunkIds> defaultDataOrder = { ChunkIds::NUMBER_OF_IMAGES, ChunkIds::IMAGE_HEIGHT, ChunkIds::IMAGE_WIDTH, ChunkIds::NUMBER_OF_PLANES };

    // data type ids
    enum class DataTypes : uint64_t
    {
        UINT8_T = 1,
        UINT16_T = 2,
        UINT32_T = 3,
        UINT64_T = 4,
        INT8_T = 5,
        INT16_T = 6,
        INT32_T = 7,
        INT64_T = 8,
        HALF = 9,
        FLOAT = 10,
        DOUBLE = 11
    };

    // default type of data
    const std::vector<DataTypes> defaultDataType = { DataTypes::UINT8_T };

    // sizes of data types
    const std::vector<uint64_t> typeSizes = { 0, 1, 2, 4, 8, 1, 2, 4, 8, 2, 4, 8 };
    //const uint64_t maxTypeSize = 8;

}

#endif // _BIG_CONSTS_H_
