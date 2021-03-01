//#include "../include/big_cache.hpp"
//
//void BigCache::load(std::ifstream &file)
//{
//    uint64_t dataSize = std::accumulate(entitySizes.begin(), entitySizes.end(), 0ull);
//    if (dataSize <= maxSize)
//    {
//        for (uint64_t i = 0, n = entitySizes.size(); i != n; ++i) {
//            getEntity(i, file);
//        }
//    }
//}
//
//void BigCache::clear()
//{
//    for (auto& e : entities) {
//        e.data.reset();
//        e.dataSize = 0;
//    }
//    lru_list.clear();
//    currentSize = 0;
//}
//
//void BigCache::flush(std::ofstream &file)
//{
//    for (uint64_t i = 0; i != entities.size(); ++i) {
//        if (entities[i].dirty) {
//            save(entities[i].data, i, file);
//            entities[i].dirty = false;
//        }
//    }
//}
//
//std::shared_ptr<char> BigCache::getEntity(uint64_t index, std::ifstream &file)
//{
//    Entity& entity = entities[index];
//    if (entity.data == nullptr) {
//        return insert(index, file);
//    }
//    lru_list.splice(lru_list.end(), lru_list, entity.it);
//    return entity.data;
//}
//
//void BigCache::addEntity(std::shared_ptr<char> data, uint64_t index, std::ofstream &file)
//{
//
//    Entity& entity = entities[index];
//    if (entity.data == nullptr) {
//        entities[index].it = lru_list.insert(lru_list.end(), index);
//    }
//    else {
//        lru_list.splice(lru_list.end(), lru_list, entity.it);
//        entity.data.reset();
//        currentSize -= entity.dataSize;
//    }
//    entity.data = data;
//    entity.dataSize = entitySizes[index];
//    currentSize += entitySizes[index];
//    entity.dirty = true;
//
//    while (currentSize > maxSize) {
//        Entity& e = entities[lru_list.front()];
//        lru_list.pop_front();
//        if (e.dirty) save(data, index, file);
//        e.data.reset();
//        e.dataSize = 0;
//        e.dirty = false;
//        currentSize -= e.dataSize;
//    }
//}
//
//std::shared_ptr<char> BigCache::insert(const uint64_t& index, std::ifstream &file)
//{
//    // make space in cache
//    if (entitySizes[index] <= maxSize) {
//        while (currentSize + entitySizes[index] > maxSize) {
//            Entity& e = entities[lru_list.front()];
//            lru_list.pop_front();
//            e.data.reset();
//            e.dataSize = 0;
//            currentSize -= e.dataSize;
//        }
//    }
//
//    // load new entity
//    file.seekg(dataPositions[index]);
//    std::shared_ptr<char> data = std::shared_ptr<char>(new char[entitySizes[index]], [](char *p) { delete[] p; });
//    file.read(data.get(), entitySizes[index]);
//
//    // save into cache
//    if (entitySizes[index] <= maxSize) {
//        entities[index].data = data;
//        entities[index].dataSize = entitySizes[index];
//        entities[index].it = lru_list.insert(lru_list.end(), index);
//        currentSize += entitySizes[index];
//    }
//
//    return data;
//}
//
//void BigCache::save(std::shared_ptr<char> data, uint64_t index, std::ofstream &file)
//{
//    if (dataPositions[index] == 0) {
//        file.seekp(0, std::ios_base::end);
//        const auto id = ChunkIds::DATA;
//        file.write(reinterpret_cast<const char*>(&id), CHUNK_LENGTH);
//        uint64_t length = entitySizes[index] + (entitySizes[index] % CHUNK_LENGTH > 0 ? CHUNK_LENGTH - entitySizes[index] % CHUNK_LENGTH : 0);
//        file.write(reinterpret_cast<const char*>(&length), sizeof(length));
//        file.write(reinterpret_cast<const char*>(&index), sizeof(index));
//        dataPositions[index] = file.tellp();
//    }
//    else {
//        file.seekp(dataPositions[index]);
//    }
//    file.write(data.get(), entitySizes[index]);
//    const uint64_t zero = 0;
//    file.write(reinterpret_cast<const char*>(&zero), CHUNK_LENGTH - entitySizes[index] % CHUNK_LENGTH);
//}