#include "../include/big_core_read_info.hpp"

namespace big
{
    BigCoreReadInfo::BigCoreReadInfo(const std::string& fileName)
    {
        // open file
        file.open(fileName, std::ios_base::binary);
        if (!file) throw "Unable to open file for reading!";

        // check header
        std::string buffer(CHUNK_LENGTH, ' ');
        file.read(&buffer[0], CHUNK_LENGTH);
        if (buffer != MAGIC) throw "File format is not BIG format!";

        // load meta-data and remember position of the data
        while (file) {
            ChunkIds id;
            uint64_t length;
            if (!readChunk(id, length)) {
                if (file.eof()) break;
                else throw "Error while reading file!";
            }
            if (!readData(id, length)) {
                throw "Error while reading file!";
            }
        }
        file.clear();

        // prepare supporting structures
        initSupportingStructures();
    }

    BigCoreReadInfo::~BigCoreReadInfo()
    {
        if (file.is_open()) file.close();
    }

    bool BigCoreReadInfo::readChunk(ChunkIds& id, uint64_t& length)
    {
        file.read(reinterpret_cast<char*>(&id), CHUNK_LENGTH);
        file.read(reinterpret_cast<char*>(&length), CHUNK_LENGTH);
        return !file.fail();
    }

    bool BigCoreReadInfo::readData(ChunkIds id, uint64_t length)
    {
      switch (id) {
      case ChunkIds::NUMBER_OF_IMAGES:
        file.read(reinterpret_cast<char*>(&numberOfImages), sizeof(numberOfImages));
        file.seekg(length - sizeof(numberOfImages), std::ios_base::cur);
        break;
      case ChunkIds::IMAGE_HEIGHT:
        file.read(reinterpret_cast<char*>(&imageHeight), sizeof(imageHeight));
        file.seekg(length - sizeof(imageHeight), std::ios_base::cur);
        break;
      case ChunkIds::IMAGE_WIDTH:
        file.read(reinterpret_cast<char*>(&imageWidth), sizeof(imageWidth));
        file.seekg(length - sizeof(imageWidth), std::ios_base::cur);
        break;
      case ChunkIds::NUMBER_OF_PLANES:
        file.read(reinterpret_cast<char*>(&numberOfPlanes), sizeof(numberOfPlanes));
        file.seekg(length - sizeof(numberOfPlanes), std::ios_base::cur);
        break;
      case ChunkIds::DATA_ORDER:
      {
        dataOrder.clear();
        uint64_t n = length / sizeof(ChunkIds);
        for (uint64_t i = 0; i != n; ++i) {
          ChunkIds order;
          file.read(reinterpret_cast<char*>(&order), sizeof(ChunkIds));
          dataOrder.push_back(order);
        }
        if (dataOrder != defaultDataOrder)
        {
          throw "This data order is not supported!";

        }
        file.seekg(length - n * sizeof(ChunkIds), std::ios_base::cur);
        break;
      }
      /* case ChunkIds::DATA_TYPE:
       {
           dataType.clear();
           uint64_t n = length / sizeof(DataTypes);
           for (uint64_t i = 0; i != n; ++i) {
               DataTypes type;
               file.read(reinterpret_cast<char*>(&type), sizeof(DataTypes));
               dataType.push_back(type);
           }
           file.seekg(length - n * sizeof(DataTypes), std::ios_base::cur);
           break;
       }*/
        case ChunkIds::DATA:{
          uint64_t index;
          DataTypes type;
          file.read(reinterpret_cast<char*>(&index), sizeof(index));
          file.read(reinterpret_cast<char*>(&type), sizeof(DataTypes));
          dataType.push_back(type);
          if (dataPositions.size() <= index) dataPositions.resize(index + 1, 0);
          dataPositions[index] = file.tellg();
          file.seekg(length - sizeof(index) - sizeof(type), std::ios_base::cur);
          break;
        }
        case ChunkIds::XML: {
          //read xml filename
          xmlFileName.resize(length);
          file.read(reinterpret_cast<char*>(&xmlFileName[0]), length);
          xmlFileName.resize(xmlFileName.find_first_of('\0')); //resize string to \0 end symbol
          doc.load_string(xmlFileName.c_str());
          break;
        }
        default:
            file.seekg(length, std::ios_base::cur);
        }
        
       
    
        return !file.fail();
    }
}
