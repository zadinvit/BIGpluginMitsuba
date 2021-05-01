/**
* MIFlib - Multi-Image Format Library - version 0.4
* --------------------------------------------------------
* Copyright (C) 2021, Radomir Vavra (vavra.radomir@gmail.com) and Jiri Filip
* Report bugs and download new versions at https://github.com/RadomirVavra/MIFlib/
*
* This library is distributed under the 3-clause BSD license. See notice at the end
* of this file.
*
* This library uses parts of Image Library, which are:
* Copyright (C) 2019-2021, by Radomir Vavra (vavra.radomir@gmail.com)
*/

#pragma once

#include <cstdint>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "ext/Image.hpp"

namespace mif
{
    class MIF
    {
    public: // Types section.

        static const uint64_t version = 1;     // current MIF file format version

        // types of data chunks and their identifiers
        static const uint64_t type_magic = 0x454C49462046494D;
        static const uint64_t type_image = 0;
        static const uint64_t type_xml = 1;
        static const uint64_t type_empty = -1;

        static const size_t highestFastIndex = 0xFFFF;   // highest image index accessible through fast access

        // data type ids
        enum class DataType : uint64_t
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

        // sizes of data types
        const std::vector<size_t> typeSizes = { 0, 1, 2, 4, 8, 1, 2, 4, 8, 2, 4, 8 };

    public: // Constructors, destructor and copy/move assignment operator section.

        // Constructor. If fileName is not empty, opens file. If it is new file then writes header, else reads parameters of all images.
        explicit MIF(const std::string & fileName = {}, bool read_only = false);

        // Copy constructor forbidden.
        MIF(const MIF &) = delete;

        // Move constructor default.
        MIF(MIF &&other) = default;

        // Destructor.
        ~MIF() = default;

        // Copy-assignment operator forbidden.
        MIF &operator=(const MIF &) = delete;

        // Move-assignment operator default.
        MIF &operator=(MIF &&rhs) = default;

    public: // Open and close data file section.

        // Opens file. If it is new file then writes header, else reads parameters of all images. Returns true if succeeds.
        virtual bool open(const std::string & fileName, bool read_only = false);

        // Closes file and clears memory.
        void close();

        // Returns true if file is open.
        bool isOpen() const { return file.is_open(); }

    public: // Get image information section.

        // Returns number of contained images.
        size_t getNumberOfImages() const { return images.size(); }

        // Returns the highest index used for some image.
        size_t getHighestIndex() const { if (images.rbegin() != images.rend()) return images.rbegin()->first; return -1; }
        
        // Returns true if image is present in file.
        bool isImagePresent(size_t image_index) const { return getImageInfo(image_index).length != 0; }

        // Returns flags of image.
        uint64_t getImageFlags(size_t image_index) const { return getImageInfo(image_index).flags; }

        // Returns data-type of image.
        DataType getImageDataType(size_t image_index) const { return getImageInfo(image_index).dataType; }

        // Returns number of rows of image.
        size_t getImageHeight(size_t image_index) const { return getImageInfo(image_index).height; }

        // Returns number of columns of image.
        size_t getImageWidth(size_t image_index) const { return getImageInfo(image_index).width; }

        // Returns number of (color) planes of image.
        size_t getImageNumberOfPlanes(size_t image_index) const { return getImageInfo(image_index).np; }

        // Returns size of image.
        size_t getImageSize(size_t image_index) const;

    public: // Update image information section.

        // Updates index of image.
        void updateImageIndex(size_t image_index, size_t new_index);

        // Update flags of image.
        void updateImageFlags(size_t image_index, uint64_t new_flags);

        // Reshapes size of image while overall size remains constant.
        void reshapeImage(size_t image_index, size_t new_height, size_t new_width, size_t new_np = 1);

    public: // Image data section.

        // Writes image into file, together with its unique index and flags.
        template<typename T>
        void addImage(size_t image_index, const image::Image<T> & image, uint64_t flags = 0);

        // Writes image into file, together with its flags. Returns assigned image index, which equals to the highest index in the file plus one.
        template<typename T>
        size_t pushImage(const image::Image<T> & image, uint64_t flags = 0);

        // Updates data of an existing image. Same effect as calling removeImage and addImage.
        template<typename T>
        void updateImage(size_t image_index, const image::Image<T> & image, uint64_t flags = 0);

        // Returns whole image.
        template<typename T>
        image::Image<T> getImage(size_t image_index);

        // Removes image from file.
        void removeImage(size_t image_index);

    public: // Read image element section.

        // Returns element from the given image at given row, column and (color) plane. Does not check bounds.
        template<typename T>
        T get(size_t image_index, size_t row, size_t col, size_t plane = 0);

        // Returns element from the given image at given row, column and (color) plane. Checks bounds.
        template<typename T>
        T at(size_t image_index, size_t row, size_t col, size_t plane = 0);

        // Returns pixel (all its planes) from the given image at given row and column. Does not check bounds.
        template<typename T>
        void getPixel(size_t image_index, size_t row, size_t col, T * values);

        // Returns pixel (all its planes) from the given image at given row and column. Checks bounds.
        template<typename T>
        void atPixel(size_t image_index, size_t row, size_t col, T * values);

    public: // Modify image element section.

        // Updates value of single element inside an existing image.
        template<typename T>
        void updateValue(size_t image_index, size_t row, size_t col, size_t plane, const T & new_value);

        // Updates values (all planes) of one pixel inside an existing image.
        template<typename T>
        void updatePixel(size_t image_index, size_t row, size_t col, T * new_values);

    public: // XML section.

        // Returns true if XML already present in file.
        bool isXML() const { return xml.pos != 0; }

        // Writes XML string into file. Can be used once only. Returns true if succeeds.
        void addXML(const std::string & XMLstring);

        // Updates XML string in file. Same effect as calling removeXML and addXML.
        void updateXML(const std::string & XMLstring);

        // Returns XML data included in file.
        const std::string & getXML() const { return xml.str; }

        // Removes XML from file.
        void removeXML();

    protected: // Image info section.

        // Image information structure
        struct ImageInfo
        {
            uint64_t pos = 0;
            uint64_t length = 0;
            uint64_t flags = 0;
            DataType dataType = DataType::UINT8_T;
            size_t height = 0;
            size_t width = 0;
            size_t np = 0;
        };

        // Returns ImageInfo for given image_index or returns empty ImageInfo structure.
        const ImageInfo & MIF::getImageInfo(size_t image_index) const;

        // Inserts ImageInfo structure with given index into data structures.
        void insertImageInfo(size_t image_index, const ImageInfo & info);

        // Removes ImageInfo with given image_index from data structures.
        void removeImageInfo(size_t image_index);

    protected: // XML info section.

        struct XMLinfo
        {
            uint64_t pos = 0;
            uint64_t length = 0;
            std::string str;
            void clear() { pos = 0; length = 0; str.clear(); }
        };

    protected: // Open file and read data section.

        // Opens file and returns true if succeeds.
        bool openFile(const std::string & fileName);

        // Scans through file and returns true if at least magic chunk is ok.
        bool scanFile();

        // Reads header of chunk (type and length). Returns true if succeeds.
        bool readChunkHead(uint64_t & type, uint64_t & length);

        // Reads magic chunk and returns true if succeeds.
        bool readChunkMagic(uint64_t pos, uint64_t length);

        // Reads image chunk meta-data and returns true if succeeds.
        bool readChunkImage(uint64_t pos, uint64_t length);

        // Reads XML chunk and returns true if succeeds.
        bool readChunkXML(uint64_t pos, uint64_t length);

        // Reads one data element from file and returns it.
        template<typename T>
        T readImageElement(const ImageInfo & info, size_t row, size_t col, size_t plane);

        // Reads data of one image pixel (all color planes) and returns them
        template<typename T>
        void readImagePixel(const ImageInfo & info, size_t row, size_t col, T * data);

    protected: // Write data section.

        // Writes magic chunk into file.
        void writeChunkMagic();

        // Writes image chunk info file.
        void writeChunkImage(size_t index, const ImageInfo & info, const char * data);

        // Writes XML chunk into file.
        void writeChunkXML();

    protected: // Free space section.

        // Adds free space into list of all free spaces and joins consecutive free spaces
        void addFreeSpace(uint64_t pos, uint64_t length);

        // Find free space for new chunk. If exact match not found, finds the smallest space where chunk fits.
        uint64_t findAndPrepareFreeSpace(uint64_t length);

    protected: // Other methods section.

        // Returns length of image data itself.
        size_t computeImageDataLength(const ImageInfo & info) const { return typeSizes[static_cast<size_t>(info.dataType)] * info.height * info.width * info.np; }

        // Returns length of image chunk excluding chunk head.
        size_t computeImageChunkLength(const ImageInfo & info) const { return 6 * sizeof(uint64_t) + computeImageDataLength(info); }

        // Returns data type based on data type of image.
        DataType getDataType(const image::Image<uint8_t> &) const { return DataType::UINT8_T; }
        DataType getDataType(const image::Image<uint16_t> &) const { return DataType::UINT16_T; }
        DataType getDataType(const image::Image<uint32_t> &) const { return DataType::UINT32_T; }
        DataType getDataType(const image::Image<uint64_t> &) const { return DataType::UINT64_T; }
        DataType getDataType(const image::Image<int8_t> &) const { return DataType::INT8_T; }
        DataType getDataType(const image::Image<int16_t> &) const { return DataType::INT16_T; }
        DataType getDataType(const image::Image<int32_t> &) const { return DataType::INT32_T; }
        DataType getDataType(const image::Image<int64_t> &) const { return DataType::INT64_T; }
        // todo: DataType getDataType(const image::Image<half> &) const { return DataType::HALF; }
        DataType getDataType(const image::Image<float> &) const { return DataType::FLOAT; }
        DataType getDataType(const image::Image<double> &) const { return DataType::DOUBLE; }

        // Returns true if data type in argument equals to template parameter
        template<typename T> bool checkType(DataType dataType) const { return false; }
        template<> bool checkType<uint8_t>(DataType dataType) const { return dataType == DataType::UINT8_T; }
        template<> bool checkType<uint16_t>(DataType dataType) const { return dataType == DataType::UINT16_T; }
        template<> bool checkType<uint32_t>(DataType dataType) const { return dataType == DataType::UINT32_T; }
        template<> bool checkType<uint64_t>(DataType dataType) const { return dataType == DataType::UINT64_T; }
        template<> bool checkType<int8_t>(DataType dataType) const { return dataType == DataType::INT8_T; }
        template<> bool checkType<int16_t>(DataType dataType) const { return dataType == DataType::INT16_T; }
        template<> bool checkType<int32_t>(DataType dataType) const { return dataType == DataType::INT32_T; }
        template<> bool checkType<int64_t>(DataType dataType) const { return dataType == DataType::INT64_T; }
        // todo: template<> bool checkType<half>(DataType dataType) const { return dataType == DataType::HALF; }
        template<> bool checkType<float>(DataType dataType) const { return dataType == DataType::FLOAT; }
        template<> bool checkType<double>(DataType dataType) const { return dataType == DataType::DOUBLE; }

    protected:
        std::fstream file;                  // opened file stream
        bool read_only = true;              // read only flag

        std::map<size_t, ImageInfo> images; // map from image_index to ImageInfo for all present images
        std::vector<ImageInfo> imvec;       // part of images map for indices lower than highestFastIndex

        XMLinfo xml;                        // XML information

        std::map<uint64_t, uint64_t> freeSpaces; // list of all free spaces inside file including end of file
        uint64_t pos_end = 0;              // position of end of file
    };
}

/**
* 3-Clause BSD License
*
* Copyright (c) 2021, Radomir Vavra (vavra.radomir@gmail.com) and Jiri Filip.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
