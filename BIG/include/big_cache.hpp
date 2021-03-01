//#pragma once
//#ifndef _BIG_CACHE_H_
//#define _BIG_CACHE_H_
//
//#include "big_consts.hpp"
//
//#include <cstdint>
//#include <fstream>
//#include <vector>
//#include <list>
//#include <memory>
//#include <numeric>
//
//namespace big
//{
//
//    class BigCache
//    {
//    private:
//
//        struct Entity
//        {
//            std::shared_ptr<char> data;
//            uint64_t dataSize = 0;
//            std::list<uint64_t>::iterator it;
//            bool dirty = false;
//        };
//
//        std::vector<Entity> entities;           // entities currently stored in a memory
//        std::list<uint64_t> lru_list;           // list of least recently used entities, starting with the least recently used and ending with the most recently used entity
//
//        uint64_t currentSize = 0;               // size of the used memory
//        uint64_t maxSize = DEFAULT_CACHE_SIZE;  // the maximal size of usable memory
//
//        std::vector<uint64_t>& entitySizes = std::vector<uint64_t>();   // sizes of the entities
//        std::vector<uint64_t>& dataPositions = std::vector<uint64_t>(); // positions of the entities in a file
//
//    public:
//
//        void initialize(std::vector<uint64_t>& entitySizes, std::vector<uint64_t>& dataPositions) { this->entitySizes = entitySizes; this->dataPositions = dataPositions; entities.resize(entitySizes.size()); }
//
//        void setSize(uint64_t maxSize) { this->maxSize = maxSize; }
//
//        uint64_t getSize() { return currentSize; }
//
//        void load(std::ifstream &file);
//
//        void clear();
//
//        // save dirty entities from the cache to a file
//        void flush(std::ofstream &file);
//
//        std::shared_ptr<char> getEntity(uint64_t index, std::ifstream &file);
//
//        void addEntity(std::shared_ptr<char> data, uint64_t index, std::ofstream &file);
//
//        template<typename T>
//        T getElement(uint64_t entityID, uint64_t index, std::ifstream &file);
//
//        template<typename T>
//        void addElement(const T& value, uint64_t entityID, uint64_t index, std::ofstream &file);
//
//    private:
//
//        std::shared_ptr<char> insert(const uint64_t& index, std::ifstream &file);
//
//        void save(std::shared_ptr<char> data, uint64_t index, std::ofstream &file);
//    };
//
//    template<typename T>
//    T BigCache::getElement(uint64_t entityID, uint64_t index, std::ifstream &file)
//    {
//        Entity& entity = entities[entityID];
//        if (entity.data == nullptr) {
//            if (entitySizes[entityID] <= maxSize) {
//                return reinterpret_cast<T*>(insert(entityID, file).get())[index];
//            }
//            else {
//                // read data directly
//                file.seekg(dataPositions[entityID] + index * sizeof(T));
//                T val;
//                file.read(reinterpret_cast<char*>(&val), sizeof(T));
//                return val;
//            }
//        }
//        lru_list.splice(lru_list.end(), lru_list, entity.it);
//        return reinterpret_cast<T*>(entity.data.get())[index];
//    }
//
//    template<typename T>
//    void BigCache::addElement(const T& value, uint64_t entityID, uint64_t index, std::ofstream &file)
//    {
//        Entity& entity = entities[entityID];
//        if (entity.data == nullptr) {
//            if (entitySizes[entityID] <= maxSize) {
//                insert(entityID, file);
//            }
//            else {
//                // write data directly
//                if (dataPositions[entityID] == 0) {
//                    file.seekp(0, std::ios_base::end);
//                    const auto id = ChunkIds::DATA;
//                    file.write(reinterpret_cast<const char*>(&id), CHUNK_LENGTH);
//                    uint64_t length = entitySizes[entityID] + (entitySizes[entityID] % CHUNK_LENGTH > 0 ? CHUNK_LENGTH - entitySizes[entityID] % CHUNK_LENGTH : 0);
//                    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
//                    file.write(reinterpret_cast<const char*>(&entityID), sizeof(entityID));
//                    dataPositions[entityID] = file.tellp();
//                    const uint64_t zero = 0;
//                    for (uint64_t i = 0; i != length / CHUNK_LENGTH; ++i) {
//                        file.write(reinterpret_cast<const char*>(&zero), CHUNK_LENGTH);
//                    }
//                }
//                file.seekp(dataPositions[entityID] + index * sizeof(T));
//                file.write(reinterpret_cast<char*>(&value), sizeof(T));
//                return;
//            }
//        }
//        else {
//            lru_list.splice(lru_list.end(), lru_list, entity.it);
//        }
//        entity.data[index] = value;
//        entity.dirty = true;
//    }
//
//}
//
//#endif // _BIG_CACHE_H_
