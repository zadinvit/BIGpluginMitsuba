#pragma once
#ifndef _BIG_CORE_READ_H_
#ifdef  _BIG_CORE_READ_H_
#define BIGDLL  __declspec(dllexport)  
#else
#define BIGDLL  __declspec(dllimport)   
#endif
#define _BIG_CORE_READ_H_

#include "big_core_read_info.hpp"
#include "big_cache_read.hpp"
#include "pugixml.hpp"

#include <fstream>
#include <array>
#include <memory>
#include "half.hpp"

namespace big
{
    class BIGDLL BigCoreRead : public BigCoreReadInfo
    {
    public:

        // Constructor. Opens file, reads parameters of the container, sets size of cache and reads all data into cache if requested and possible.
        BigCoreRead(const std::string& fileName, bool loadToCache = false, uint64_t cacheSizeBytes = DEFAULT_CACHE_SIZE);

        // Copy constructor forbidden.
        BigCoreRead(const BigCoreRead &) = delete;

        // Move constructor.
        BigCoreRead(BigCoreRead &&other) = default;

        // Destructor.
        ~BigCoreRead() = default;

        // Copy-assignment operator forbidden.
        BigCoreRead &operator=(const BigCoreRead &) = delete;

        // Move-assignment operator.
        BigCoreRead &operator=(BigCoreRead &&rhs) = default;

    public:

        // Returns element in the given image at given row, column and (color) plane. Does not check bounds.
        template<typename T>
        T get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane = 0);

        // Returns element in the given image at given row, column and (color) plane. Checks bounds.
        template<typename T>
        T at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane = 0);

        // Returns pointer to the entity specified by its index.
        std::shared_ptr<const char> operator[] (uint64_t index);

        // Returns the entity specified by its index. Values are converted to the specified data type.
        template<typename T>
        std::vector<T> getEntity(uint64_t index);

        // Returns the image specified by its number. Values are converted to the specified data type.
        template<typename T>
        std::vector<T> getImage(uint64_t imageNum);

       

    public:

        //loadToCache methods are not used, in code use only cache.load(file, totalSize)
        // use only for testing, and cache.load was changed so we can delete this
        // Loads all data into cache, if possible. Does nothing if data are bigger than cache size.
        //void loadToCache() { cache.load(file); }

        // Sets maximal possible size of cache that can be used and loads all data into cache, if possible.
        // Does nothing if data are bigger than cache size.
        //void loadToCache(uint64_t bytes) { cache.setSize(bytes); cache.load(file); }

        // Releases all memory allocated by cache.
        void clearCache() { cache.clear(); }

        // Reduces memory usage by freeing least recently used entities.
        void shrinkCache() { cache.shrink(); }

        // Reduces memory usage by freeing least recently used entities.
        void shrinkCache(uint64_t bytes) { cache.setSize(bytes); cache.shrink(); }

        // Sets maximal possible size of cache that can be used. Does not free memory if already allocated.
        void setCacheSize(uint64_t bytes) { cache.setSize(bytes); }

        // Returns size of the used memory.
        uint64_t getCacheSize() { return cache.getSize(); }

        // Returns true if all data are in cache.
        bool isAllInCache();

        //xml read atributes
        int readXMLInt(std::string nameOfNode);

        //xml read atributes
        double readXMLDouble(std::string nameOfNode);

        //xml read atributes
        bool readXMLBool(std::string nameOfNode);

        //xml read atributes String
        std::string readXMLString(std::string nameOfNode);
        

    protected:
        BigCacheRead cache;

    };
}

#endif // _BIG_CORE_READ_H_
