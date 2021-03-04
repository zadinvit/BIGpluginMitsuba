#pragma once
#ifndef _BIG_CACHE_READ_H_
#define _BIG_CACHE_READ_H_

#include "big_consts.hpp"

#include <cstdint>
#include <fstream>
#include <vector>
#include <list>
#include <memory>
#include <numeric>

namespace big
{

    class BigCacheRead
    {
    private:

        struct Entity
        {
            std::shared_ptr<char> data;
            std::list<uint64_t>::iterator it;
            uint64_t time;
        };

        std::vector<Entity> entities_8;         // entities 8bit currently stored in a memory - uint8_t, int8_t (can change for unordered_map but we lost speed)
        std::vector<Entity> entities_16;		// entities 16bit currently stored in a memory - half, uint16_t, int16_t
        std::vector<Entity> entities_32;		// entities 32bit currently stored in a memory - float, uint32_t, int32_t
        std::vector<Entity> entities_64;	    // entities 64bit currently stored in a memory - double, uint64_t, int64_t
        std::list<uint64_t> lru_list_8;         // list of least recently used 8 bit entities, starting with the least recently used and ending with the most recently used entity
        std::list<uint64_t> lru_list_16;		// list of least recently used 16 bit entities, starting with the least recently used and ending with the most recently used entity
        std::list<uint64_t> lru_list_32;		// list of least recently used 32 bit entities, starting with the least recently used and ending with the most recently used entity
        std::list<uint64_t> lru_list_64;		// list of least recently used 64 bit entities, starting with the least recently used and ending with the most recently used entity


        uint64_t currentSize = 0;               // size of the used memory
        uint64_t maxSize = DEFAULT_CACHE_SIZE;  // the maximal size of usable memory
        uint64_t time = 0;                      // changed everytime i work with data

        std::vector<uint64_t>& entitySizes;// = std::vector<uint64_t>();    // sizes of the entities
        std::vector<uint64_t>& dataPositions;// = std::vector<uint64_t>();  // positions of the entities in a file
        std::vector<DataTypes>& dataTypes;
        std::ifstream& file;                        // attached file

    public:

        BigCacheRead(std::ifstream &file, std::vector<uint64_t> &entitySizes, std::vector<uint64_t> &dataPositions, std::vector<DataTypes> &dataTypes);

        //set max cachce size
        void setSize(uint64_t maxSize);

        //return size of used memoryy
        uint64_t getSize();

        //load data until cache isn't full, totalSize = size all data in file
        void load(std::ifstream &file, uint64_t & totalSize);
        
        //clear list of recently used entities
        void clear();
        
        //shrink cache data to max size
        void shrink();
        
        // Returns pointer to the entity specified by its index.
        std::shared_ptr<const char> operator[] (uint64_t index);
        
        // Returns pointer to the entity specified by its index.
        template<typename T>
        std::vector<T> getEntity(uint64_t index);
        
        // return element by entity ID and her index
        template<typename T>
        T getElement(uint64_t entityID, uint64_t index);

        //return element from cache by entity ID and her index
        template<typename T>
        T getElementFromMemory(uint64_t entityID, uint64_t index);

    private:

        //coverts datatypes
        template<typename Tdst, typename Tsrc>
        Tdst convert(Tsrc value);

        //load and save element from file to cache, select by entity ID and her index
        template<typename T>
        T getElementFromFile(uint64_t entityID, uint64_t index);

        //delete least used entity 
        void pop(DataTypes dataType);

        //delete least used 8 bit entity
        void pop8();

        //delete least used 16 bit entity 
        void pop16();

        //delete least used 32 bit entity 
        void pop32();

        //delete least used 64 bit entity 
        void pop64();
        
        //pull 8 bit entity
        void pull8(uint64_t index);
        
        //pull 16 bit entity
        void pull16(uint64_t index);

        //pull 32 bit entity
        void pull32(uint64_t index);
        
        //pull 64bit entity
        void pull64(uint64_t index);

        void insert(const uint64_t& index);
        
    };
}

#endif // _BIG_CACHE_READ_H_
