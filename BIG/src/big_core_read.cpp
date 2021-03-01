#include "../include/big_core_read.hpp"

namespace big
{
    using half_float::half;
    using half_float::half_cast;
    using namespace half_float::literal;
    BigCoreRead::BigCoreRead(const std::string& fileName, bool loadToCache, uint64_t cacheSizeBytes)
        : BigCoreReadInfo(fileName), cache(file, entitySizes, dataPositions, entityDataTypes)
    {
        cache.setSize(cacheSizeBytes);
        uint64_t totalSize = 0;
        for (const auto & entitySize : entitySizes) totalSize += entitySize;        if (loadToCache || totalSize <= cacheSizeBytes) { //if we change to and &&, fix problem when user want load to cache but cache it to small and this slow algorithm
          cache.load(file, totalSize);
        }
    }

    template<typename T>
    T BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane)
    {
        uint64_t indices[4];

        indices[numberOfImagesOrder] = imageNum;
        indices[imageHeightOrder] = row;
        indices[imageWidthOrder] = col;
        indices[numberOfPlanesOrder] = plane;

        uint64_t index = indices[1] * subSizes[1] + indices[2] * subSizes[2] + indices[3];

        return cache.getElement<T>(indices[0], index);
    }

    template uint8_t BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template uint16_t BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template uint32_t BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template uint64_t BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template int8_t BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template int16_t BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template int32_t BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template int64_t BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template half BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template float BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template double BigCoreRead::get(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);

    template<typename T>
    T BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane)
    {
        if (imageNum >= numberOfImages)
            throw "Image number out of bounds!";

        if (row >= imageHeight)
            throw "Row out of bounds!";

        if (col >= imageWidth)
            throw "Column out of bounds!";

        if (plane >= numberOfPlanes)
            throw "Plane number out of bounds!";

        return get<T>(imageNum, row, col, plane);
    }

    template uint8_t BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template uint16_t BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template uint32_t BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template uint64_t BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template int8_t BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template int16_t BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template int32_t BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template int64_t BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template half BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template float BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);
    template double BigCoreRead::at(uint64_t imageNum, uint64_t row, uint64_t col, uint64_t plane);

    std::shared_ptr<const char> BigCoreRead::operator[](uint64_t index)
    {
        if (index >= dimensions[0])
            throw "Index out of bound!";

        return cache[index];
    }

    template<typename T>
    std::vector<T> BigCoreRead::getEntity(uint64_t index)
    {
        if (index >= dimensions[0])
            throw "Index out of bound!";

        return cache.getEntity<T>(index);
    }

    template std::vector<uint8_t> BigCoreRead::getEntity(uint64_t index);
    template std::vector<uint16_t> BigCoreRead::getEntity(uint64_t index);
    template std::vector<uint32_t> BigCoreRead::getEntity(uint64_t index);
    template std::vector<uint64_t> BigCoreRead::getEntity(uint64_t index);
    template std::vector<int8_t> BigCoreRead::getEntity(uint64_t index);
    template std::vector<int16_t> BigCoreRead::getEntity(uint64_t index);
    template std::vector<int32_t> BigCoreRead::getEntity(uint64_t index);
    template std::vector<int64_t> BigCoreRead::getEntity(uint64_t index);
    template std::vector<half> BigCoreRead::getEntity(uint64_t index);
    template std::vector<float> BigCoreRead::getEntity(uint64_t index);
    template std::vector<double> BigCoreRead::getEntity(uint64_t index);

    template<typename T>
    std::vector<T> BigCoreRead::getImage(uint64_t imageNum)
    {
        if (imageNum >= numberOfImages)
            throw "Image number out of bound!";

        if (isAllInCache()) {

            uint64_t indices[4];

            uint64_t &i0 = indices[numberOfImagesOrder];
            uint64_t &i1 = indices[imageHeightOrder];
            uint64_t &i2 = indices[imageWidthOrder];
            uint64_t &i3 = indices[numberOfPlanesOrder];

            const uint64_t &r0 = indices[0];
            const uint64_t &r1 = indices[1];
            const uint64_t &r2 = indices[2];
            const uint64_t &r3 = indices[3];





            uint64_t S[4];
            S[0] = 0;
            S[1] = subSizes[1];
            S[2] = subSizes[2];
            S[3] = 1;

            const uint64_t S0 = S[numberOfImagesOrder];
            const uint64_t S1 = S[imageHeightOrder];
            const uint64_t S2 = S[imageWidthOrder];
            const uint64_t S3 = S[numberOfPlanesOrder];


            i0 = imageNum;
            std::vector<T> data(imageHeight * imageWidth * numberOfPlanes);
            std::vector<T>::size_type index = 0;

            uint64_t i = imageNum * S0;
            uint64_t ii2, ii3;

            const uint64_t ih = imageHeight;
            const uint64_t iw = imageWidth;
            const uint64_t nop = numberOfPlanes;

            for (i1 = 0; i1 != ih; ++i1, i += S1) {
                ii2 = i;
                for (i2 = 0; i2 != iw; ++i2, ii2 += S2) {
                    ii3 = ii2;
                    for (i3 = 0; i3 != nop; ++i3, ii3 += S3) {
                        data[index++] = cache.getElementFromMemory<T>(r0, ii3);
                    }
                }
            }
            return data;
        }
		else {

			std::array<uint64_t, 4> indices;
			//indices[orderMap[static_cast<uint64_t>(ChunkIds::NUMBER_OF_IMAGES)]] = imageNum;
			indices[numberOfImagesOrder] = imageNum;
			uint64_t index = 0;
			std::vector<T> data(imageHeight * imageWidth * numberOfPlanes);
			for (uint64_t row = 0; row != imageHeight; ++row) {
				//indices[orderMap[static_cast<uint64_t>(ChunkIds::IMAGE_HEIGHT)]] = row;
				indices[imageHeightOrder] = row;
				for (uint64_t col = 0; col != imageWidth; ++col) {
					//indices[orderMap[static_cast<uint64_t>(ChunkIds::IMAGE_WIDTH)]] = col;
					indices[imageWidthOrder] = col;
					for (uint64_t plane = 0; plane != numberOfPlanes; ++plane) {
						//indices[orderMap[static_cast<uint64_t>(ChunkIds::NUMBER_OF_PLANES)]] = plane;
						indices[numberOfPlanesOrder] = plane;
						uint64_t i = indices[1] * subSizes[1] + indices[2] * subSizes[2] + indices[3];
						data[index++] = cache.getElement<T>(indices[0], i);
					}
				}
			}
			return data;
		}
	

    }

    bool BigCoreRead::isAllInCache()
    { 
      return cache.getSize() == dataSize; 
    }

    template std::vector<uint8_t> BigCoreRead::getImage(uint64_t imageNum);
    template std::vector<uint16_t> BigCoreRead::getImage(uint64_t imageNum);
    template std::vector<uint32_t> BigCoreRead::getImage(uint64_t imageNum);
    template std::vector<uint64_t> BigCoreRead::getImage(uint64_t imageNum);
    template std::vector<int8_t> BigCoreRead::getImage(uint64_t imageNum);
    template std::vector<int16_t> BigCoreRead::getImage(uint64_t imageNum);
    template std::vector<int32_t> BigCoreRead::getImage(uint64_t imageNum);
    template std::vector<int64_t> BigCoreRead::getImage(uint64_t imageNum);
    template std::vector<half> BigCoreRead::getImage(uint64_t imageNum);
    template std::vector<double> BigCoreRead::getImage(uint64_t imageNum);
    template std::vector<float> BigCoreRead::getImage(uint64_t imageNum);

    int BigCoreRead::readXMLInt(std::string nameOfNode)
    {
      pugi::xml_node root = doc.document_element();

      return root.child(nameOfNode.c_str()).attribute("intVal").as_int();
    }

    double BigCoreRead::readXMLDouble(std::string nameOfNode)
    {
      pugi::xml_node root = doc.document_element();

      return root.child(nameOfNode.c_str()).attribute("doubleVal").as_double();
    }
    bool BigCoreRead::readXMLBool(std::string nameOfNode)
    {
     
      pugi::xml_node root = doc.document_element();

      return root.child(nameOfNode.c_str()).attribute("boolVal").as_bool();
    }


    std::string BigCoreRead::readXMLString(std::string nameOfNode)
    {
      pugi::xml_node root = doc.document_element();

      return root.child(nameOfNode.c_str()).attribute("stringVal").as_string();
    }
}
