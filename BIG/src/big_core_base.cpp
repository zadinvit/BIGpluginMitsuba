#include "../include/big_core_base.hpp"

namespace big
{
    //constructor
    BigCoreBase::BigCoreBase(){}

    BigCoreBase::~BigCoreBase(){}

    uint64_t BigCoreBase::getDimension(uint64_t index)
    {
        if (dimensions.empty()) initSupportingStructures();

        if (index >= dimensions.size())
            throw "Index out of bounds!";

        return dimensions[index];
    }

    uint64_t BigCoreBase::getEntitySize(uint64_t index)
    {
        if (entitySizes.size() == 0) initSupportingStructures();

        if (index >= entitySizes.size())
            throw "Index out of bounds!";

        return entitySizes[index];
    }

    uint64_t BigCoreBase::getEntityBaseSize()
    {
        if (subSizes.size() == 0) initSupportingStructures();
        return subSizes[0];
    }

    DataTypes BigCoreBase::getEntityDataType(uint64_t index)
    {
        if (dataType.size() == 1) return dataType[0];

        if (index + 1 >= dataType.size())
            throw "Index out of bounds!";

        return dataType[index + 1];
    }

    uint64_t BigCoreBase::getEntityTypeSize(uint64_t index)
    {
        if (entityTypeSizes.size() == 0) initSupportingStructures();

        if (index >= entityTypeSizes.size())
            throw "Index out of bounds!";

        return entityTypeSizes[index];

    }

    uint64_t BigCoreBase::getDataTypeSize(DataTypes dataType)
    {
        uint64_t index = static_cast<uint64_t>(dataType);

        if (index >= typeSizes.size())
            throw "Unknown data type!";

        return typeSizes[index];
    }

    uint64_t BigCoreBase::getNumberOfEntities()
    {
        if (dimensions.size() == 0) return 0;
        return dimensions[0];
    }
    

    void BigCoreBase::initSupportingStructures()
    {
        initDimensions();
        initSubSizes();
        initEntityDataTypes();
        initEntityTypeSizes();
        initEntitySizes();
        initDataSize();
        initOrderMap();
        initDataPositions();
    }

    void BigCoreBase::initDimensions()
    {
        dimensions.clear();
        for (const auto id : dataOrder) {
            switch (id) {
            case ChunkIds::NUMBER_OF_IMAGES:
                dimensions.push_back(numberOfImages);
                break;
            case ChunkIds::IMAGE_HEIGHT:
                dimensions.push_back(imageHeight);
                break;
            case ChunkIds::IMAGE_WIDTH:
                dimensions.push_back(imageWidth);
                break;
            case ChunkIds::NUMBER_OF_PLANES:
                dimensions.push_back(numberOfPlanes);
                break;
            }
        }
    }

    void BigCoreBase::initSubSizes()
    {
        uint64_t n = dimensions.size();
        subSizes.resize(n);
        subSizes[n - 1] = 1;
        for (uint64_t i = n - 1; i != 0; --i) {
            subSizes[i - 1] = subSizes[i] * dimensions[i];
        }
    }

    void BigCoreBase::initEntityDataTypes()
    {
        entityDataTypes.clear();
        if (dataType.size() == 1) {
            entityDataTypes.resize(dimensions[0], dataType[0]);
        }
        else {
            entityDataTypes.assign(dataType.begin() , dataType.end());
        }
    }

    void BigCoreBase::initEntityTypeSizes()
    {
        entityTypeSizes.clear();
        for (const auto &type : entityDataTypes) entityTypeSizes.push_back(typeSizes[static_cast<uint64_t>(type)]);
        //if (dataType.size() == 1) {
        //    entityTypeSizes.resize(dimensions[0], typeSizes[static_cast<uint64_t>(dataType[0])]);
        //}
        //else {
        //    for (uint64_t i = 1; i != dataType.size(); ++i) {
        //        entityTypeSizes.push_back(typeSizes[static_cast<uint64_t>(dataType[i])]);
        //    }
        //}
    }


    void BigCoreBase::initEntitySizes()
    {
        entitySizes.clear();
        for (const auto& ts : entityTypeSizes) {
            entitySizes.push_back(ts * subSizes[0]);
        }
    }

    void BigCoreBase::initDataSize()
    {
        dataSize = 0;
        for (const auto& es : entitySizes) {
            dataSize += es;
        }
    }

    void BigCoreBase::initOrderMap()
    {
        //orderMap.clear();
        orderMap.resize(100); // todo: max chunk number
        for (uint64_t i = 0; i != dataOrder.size(); ++i) {
            orderMap[static_cast<uint16_t>(dataOrder[i])] = i;
        }

        numberOfImagesOrder = orderMap[static_cast<uint64_t>(ChunkIds::NUMBER_OF_IMAGES)];
        imageHeightOrder = orderMap[static_cast<uint64_t>(ChunkIds::IMAGE_HEIGHT)];
        imageWidthOrder = orderMap[static_cast<uint64_t>(ChunkIds::IMAGE_WIDTH)];
        numberOfPlanesOrder = orderMap[static_cast<uint64_t>(ChunkIds::NUMBER_OF_PLANES)];


        //i0 = indices[numberOfImagesOrder];
        //i1 = indices[imageHeightOrder];
        //i2 = indices[imageWidthOrder];
        //i3 = indices[numberOfPlanesOrder];

    }

    void BigCoreBase::initDataPositions()
    {
        dataPositions.resize(dimensions[0], 0);
    }
    void BigCoreBase::setEntitySizes(DataTypes dataType)
    {
        entityDataTypes.resize(numberOfImages, dataType);
        entityTypeSizes.push_back(typeSizes[static_cast<uint64_t>(dataType)]);
        uint64_t n = dimensions.size();
        subSizes.resize(n);
        subSizes[n - 1] = 1;
        for (uint64_t i = n - 1; i != 0; --i) {
            subSizes[i - 1] = subSizes[i] * dimensions[i];
        }
        entitySizes.push_back(typeSizes[static_cast<uint64_t>(dataType)] * subSizes[0]);

    }
}
