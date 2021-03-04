#pragma once
#ifndef _BIG_CORE_BASE_H_
#define _BIG_CORE_BASE_H_

#include "big_consts.hpp"
#include "pugixml.hpp"

#include <cstdint>
#include <string>
#include <map>
#include <vector>

namespace big
{
    class BigCoreBase
    {
    protected:

        // Default constructor.
        BigCoreBase() = default;

        // Copy constructor forbidden.
        BigCoreBase(const BigCoreBase &) = delete;

        // Move constructor.
        BigCoreBase(BigCoreBase &&other) = default;

        // Copy-assignment operator forbidden.
        BigCoreBase &operator=(const BigCoreBase &) = delete;

        // Move-assignment operator.
        BigCoreBase &operator=(BigCoreBase &&rhs) = default;

        // Destructor.
        ~BigCoreBase() = default;

    public:

        // Returns number of contained images.
        uint64_t getNumberOfImages() { return numberOfImages; }

        // Returns height of images.
        uint64_t getImageHeight() { return imageHeight; }

        // Returns width of images.
        uint64_t getImageWidth() { return imageWidth; }

        // Returns number of (color) planes.
        uint64_t getNumberOfPlanes() { return numberOfPlanes; }

        // Returns name of XML file
        std::string getXMLFileName() { return xmlFileName; }

        // Returns order in which the data are serialized into memory.
        const std::vector<ChunkIds>& getDataOrder() { return dataOrder; }

        // Returns type(s) of data.
        const std::vector<DataTypes>& getDataType() { return dataType; }


        // Returns i_th dimension of the container.
        uint64_t getDimension(uint64_t i);

        // Returns size of the entity specified by its number.
        uint64_t getEntitySize(uint64_t index);

        // Returns the number of elements of each entity.
        uint64_t getEntityBaseSize();

        // Returns data type of the entity specified by its number.
        DataTypes getEntityDataType(uint64_t index);

        // Returns size of data type of the entity specified by its number.
        uint64_t getEntityTypeSize(uint64_t index);

        // Returns size of the given data type.
        static uint64_t getDataTypeSize(DataTypes dataType);

        // Returns number of entities.
        uint64_t getNumberOfEntities();

        // Returns total size of the data.
        uint64_t getSize() { return dataSize; }

        //Set size of entity 
        void setEntitySizes(DataTypes dataType);


    protected:

        // Initializes supporting data structures.
        void initSupportingStructures();

    private:

        // Initialize dimensions of the data according to dataOrder.
        void initDimensions();

        // Initialize sizes of sub-blocks of the data according to dataOrder.
        void initSubSizes();

        void initEntityDataTypes();

        // Initialize size of the data type for each entity.
        void initEntityTypeSizes();

        // Initialize size of each entity.
        void initEntitySizes();

        // Initialize entire size of the data.
        void initDataSize();

        // Initialize mapping of data order to the current indices.
        void initOrderMap();

        // Initialize positions of the data entities to zero.
        void initDataPositions();

        
    protected:

        uint64_t numberOfImages = 0;
        uint64_t imageHeight = 0;
        uint64_t imageWidth = 0;
        uint64_t numberOfPlanes = 1;
        std::string fileName;
        std::string xmlFileName;

        std::vector<ChunkIds> dataOrder = defaultDataOrder;
        std::vector<DataTypes> dataType;

        std::vector<uint64_t> dimensions;                   // dimensions of the data according to dataOrder
        std::vector<uint64_t> subSizes;                     // sizes of sub-blocks of data according to dataOrder
        std::vector<uint64_t> entityTypeSizes;              // sizes of data types of the entities
        std::vector<uint64_t> entitySizes;                  // sizes of the entities
        std::vector<DataTypes> entityDataTypes;
        uint64_t dataSize = 0;                              // size of the data according to given dimensions and data types
        //std::map<ChunkIds, uint64_t> orderMap;              // mapping of data order to the current indices
        std::vector<uint64_t> orderMap;              // mapping of data order to the current indices
        std::vector<uint64_t> dataPositions;                // positions of the data entities in a file

        uint64_t numberOfImagesOrder;
        uint64_t imageHeightOrder;
        uint64_t imageWidthOrder;
        uint64_t numberOfPlanesOrder;

        pugi::xml_document doc;         //XML file

        //uint64_t indices[4];

        //uint64_t &i0;
        //uint64_t &i1;
        //uint64_t &i2;
        //uint64_t &i3;

        //const uint64_t &r0 = indices[0];
        //const uint64_t &r1 = indices[1];
        //const uint64_t &r2 = indices[2];
        //const uint64_t &r3 = indices[3];


    };
}

#endif // _BIG_CORE_BASE_H_
