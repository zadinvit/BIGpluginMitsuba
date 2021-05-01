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
*
*
* Version History of class MIF:
*  0.1              First stable version, awaits further testing
*
* TODO List of class MIF:
*   - implement image compression
*   - add iterators to go through all images (use map iterators)
*   - add write protection if read_only
*   - add half support
*   - implement updateValue and updatePixel
*   - try to optimize imvec speed by storing only necessary info (all info is in images)
*   - optimize speed of get and getPixel methods
*   - implement unit tests
*/

#include "../include/MIF.hpp"

using image::Image;

namespace mif
{

    MIF::MIF(const std::string & fileName, bool read_only) : read_only(read_only)
    {
        if (!fileName.empty() && (!openFile(fileName) || !scanFile())) close();
    }

    bool MIF::open(const std::string & fileName, bool read_only)
    {
        // close opened file first
        close();

        // check fileName
        if (fileName.empty()) return false;

        // open and scan file
        this->read_only = read_only;
        if (openFile(fileName) && scanFile()) return true;

        // error - close file and return false
        close();
        return false;
    }

    void MIF::close()
    {
        if (file.is_open()) file.close();
        read_only = true;
        images.clear();
        imvec.clear();
        xml.clear();
        freeSpaces.clear();
        pos_end = 0;
    }

    size_t MIF::getImageSize(size_t image_index) const
    {
        const auto & info = getImageInfo(image_index);
        return info.height * info.width * info.np;
    }

    void MIF::updateImageIndex(size_t image_index, size_t new_index)
    {
        if (!isImagePresent(image_index)) throw std::out_of_range("image not found");
        file.clear();
        file.seekp(static_cast<std::streampos>(getImageInfo(image_index).pos + 2 * sizeof(uint64_t)));
        uint64_t val = static_cast<uint64_t>(new_index);
        file.write(reinterpret_cast<const char*>(&val), sizeof(uint64_t));
        file.flush();
        if (!file) throw std::ios_base::failure("error during writing");
    }

    void MIF::updateImageFlags(size_t image_index, uint64_t new_flags)
    {
        if (!isImagePresent(image_index)) throw std::out_of_range("image not found");
        file.clear();
        file.seekp(static_cast<std::streampos>(getImageInfo(image_index).pos + 3 * sizeof(uint64_t)));
        file.write(reinterpret_cast<const char*>(&new_flags), sizeof(uint64_t));
        file.flush();
        if (!file) throw std::ios_base::failure("error during writing");
    }

    void MIF::reshapeImage(size_t image_index, size_t new_height, size_t new_width, size_t new_np)
    {
        if (!isImagePresent(image_index)) throw std::out_of_range("image not found");
        if (new_height * new_width * new_np != getImageSize(image_index)) throw std::length_error("size cannot be changed");
        file.clear();
        file.seekp(static_cast<std::streampos>(getImageInfo(image_index).pos + 5 * sizeof(uint64_t)));
        uint64_t val;
        val = static_cast<uint64_t>(new_height);
        file.write(reinterpret_cast<const char*>(&val), sizeof(uint64_t));
        val = static_cast<uint64_t>(new_width);
        file.write(reinterpret_cast<const char*>(&val), sizeof(uint64_t));
        val = static_cast<uint64_t>(new_np);
        file.write(reinterpret_cast<const char*>(&val), sizeof(uint64_t));
        file.flush();
        if (!file) throw std::ios_base::failure("error during writing");
    }

    template<typename T>
    void MIF::addImage(size_t image_index, const Image<T> & image, uint64_t flags)
    {
        // check index
        if (isImagePresent(image_index)) throw std::out_of_range("index already used");

        // collect image information
        ImageInfo info;
        info.flags = flags;
        info.dataType = getDataType(image);
        info.height = image.height();
        info.width = image.width();
        info.np = image.np();
        info.length = computeImageChunkLength(info);
        info.pos = findAndPrepareFreeSpace(info.length);

        // write image
        writeChunkImage(image_index, info, reinterpret_cast<const char*>(image.data()));

        // save index
        insertImageInfo(image_index, info);
    }

    template<typename T>
    size_t MIF::pushImage(const Image<T> & image, uint64_t flags)
    {
        size_t index = getHighestIndex() + 1;
        addImage(index, image, flags);
        return index;
    }

    template<typename T>
    void MIF::updateImage(size_t image_index, const Image<T> & image, uint64_t flags)
    {
        removeImage(image_index);
        addImage(image_index, image, flags);
    }

    template<typename T>
    Image<T> MIF::getImage(size_t image_index)
    {
        // check index
        if (!isImagePresent(image_index)) throw std::out_of_range("image not found");

        // get image info
        const auto & info = getImageInfo(image_index);

        // check types
        if (!checkType<T>(info.dataType)) throw std::invalid_argument("invalid data type");

        // create image
        Image<T> img(info.height, info.width, info.np);

        // go to position and read data
        file.seekg(info.pos + 8 * sizeof(uint64_t));
        file.read(reinterpret_cast<char*>(img.data()), computeImageDataLength(info));
        if (!file) throw std::ios_base::failure("reading error");

        return img;
    }

    void MIF::removeImage(size_t image_index)
    {
        // check index
        if (!isImagePresent(image_index)) throw std::out_of_range("image not found");

        // get image info
        const auto & info = getImageInfo(image_index);

        // go to position and write data
        file.seekp(info.pos);
        file.write(reinterpret_cast<const char*>(&type_empty), sizeof(uint64_t));
        file.flush();
        if (!file) throw std::ios_base::failure("reading error");

        // add into freeSpaces structure
        addFreeSpace(info.pos, info.length);

        // remove from images and imvec
        removeImageInfo(image_index);
    }

    template<typename T>
    T MIF::get(size_t image_index, size_t row, size_t col, size_t plane)
    {
        return readImageElement<T>(getImageInfo(image_index), row, col, plane);
    }

    template<typename T>
    T MIF::at(size_t image_index, size_t row, size_t col, size_t plane)
    {
        // check index
        if (!isImagePresent(image_index)) throw std::out_of_range("image not found");

        // get image info
        const auto & info = getImageInfo(image_index);

        // check size
        if (row >= info.height || col >= info.width || plane >= info.np) throw std::out_of_range("wrong index");

        return readImageElement<T>(info, row, col, plane);
    }

    template<typename T>
    void MIF::getPixel(size_t image_index, size_t row, size_t col, T * values)
    {
        readImagePixel(getImageInfo(image_index), row, col, values);
    }

    template<typename T>
    void MIF::atPixel(size_t image_index, size_t row, size_t col, T * values)
    {
        // check index
        if (!isImagePresent(image_index)) throw std::out_of_range("image not found");

        // get image info
        const auto & info = getImageInfo(image_index);

        // check size
        if (row >= info.height || col >= info.width) throw std::out_of_range("wrong index");

        // read data
        readImagePixel(info, row, col, values);
    }

    // Updates value of single element inside an existing image.
    template<typename T>
    void MIF::updateValue(size_t image_index, size_t row, size_t col, size_t plane, const T & new_value)
    {
        // todo
        throw "not implemented yet";
    }

    // Updates values (all planes) of one pixel inside an existing image.
    template<typename T>
    void MIF::updatePixel(size_t image_index, size_t row, size_t col, T * new_values)
    {
        // todo
        throw "not implemented yet";
    }

    void MIF::addXML(const std::string & XMLstring)
    {
        // check index
        if (isXML()) throw std::out_of_range("XML already present");

        // collect image information
        xml.str = XMLstring;
        xml.length = XMLstring.length();
        xml.pos = findAndPrepareFreeSpace(xml.length);

        // write image
        writeChunkXML();
    }

    void MIF::updateXML(const std::string & XMLstring)
    {
        // remove current XML
        removeXML();

        // add new XML
        addXML(XMLstring);
    }

    void MIF::removeXML()
    {
        // if nothing to remove, its done
        if (!isXML()) return;

        // go to position and write data
        file.seekp(xml.pos);
        file.write(reinterpret_cast<const char*>(&type_empty), sizeof(uint64_t));
        file.flush();
        if (!file) throw std::ios_base::failure("reading error");

        // add into freeSpaces structure
        addFreeSpace(xml.pos, xml.length);

        // clear record
        xml.clear();
    }

    const MIF::ImageInfo & MIF::getImageInfo(size_t image_index) const
    {
        if (image_index < imvec.size()) return imvec[image_index];
        auto it = images.find(image_index);
        if (it != images.end()) return it->second;
        static ImageInfo info;
        return info;
    }

    void MIF::insertImageInfo(size_t image_index, const ImageInfo & info)
    {
        images.insert({ image_index, info });
        if (image_index <= highestFastIndex) {
            if (image_index >= imvec.size()) imvec.resize(image_index + 1);
            imvec[image_index] = info;
        }
    }

    void MIF::removeImageInfo(size_t image_index)
    {
        auto it = images.find(image_index);
        if (it == images.end()) return;
        images.erase(it);
        if (image_index < imvec.size()) imvec[image_index] = ImageInfo();
    }

    bool MIF::openFile(const std::string & fileName)
    {
        // set mode and try to open file
        unsigned int mode = std::ios_base::binary | std::ios_base::in;
        if (!read_only) mode |= std::ios_base::out;
        file.open(fileName, mode);

        if (!file) {
            // file cannot be opened for reading (probably does not exist)
            if (read_only) return false;

            // try to create new file
            file.open(fileName, std::ios_base::binary | std::ios_base::out);
            if (!file) return false;
            writeChunkMagic();
            file.close();

            // open new file for reading too
            file.open(fileName, mode);
            if (!file) return false;
        }
        return true;
    }

    bool MIF::scanFile()
    {
        // find end of file
        file.seekg(0, std::ios_base::end);
        uint64_t pos_end = static_cast<uint64_t>(file.tellg());
        file.seekg(0, std::ios_base::beg);

        // go through file
        bool magic_ok = false;
        uint64_t pos = 0;
        while (file) {
            // get current position
            pos = static_cast<uint64_t>(file.tellg());
            // if head of chunk cannot be complete then break
            if (pos_end < pos + 2 * sizeof(uint64_t)) break;
            // read head of chunk
            uint64_t type, length;
            if (readChunkHead(type, length)) {
                // if data of chunk cannot be complete then break
                if (pos_end < pos + 2 * sizeof(uint64_t) + length) break;
                switch (type) {
                case type_magic:
                    // read data of magic chunk
                    if (!readChunkMagic(pos, length)) return false;
                    magic_ok = true;
                    break;
                case type_image:
                    // read data of image chunk
                    if (!readChunkImage(pos, length)) return false;
                    break;
                case type_xml:
                    // read data of xml chunk
                    if (!readChunkXML(pos, length)) return false;
                    break;
                case type_empty:
                    // add into freeSpaces, jump over
                    addFreeSpace(pos, length);
                    file.seekg(length, std::ios_base::cur);
                    break;
                default:
                    // unknown type, skip
                    file.seekg(length, std::ios_base::cur);
                }
            }
            else return false;
            if (!magic_ok) return false;
        }
        file.clear();

        // save last position where we can write
        addFreeSpace(pos, -1);

        return !file.fail();
    }

    bool MIF::readChunkHead(uint64_t & type, uint64_t & length)
    {
        file.read(reinterpret_cast<char*>(&type), sizeof(uint64_t));
        file.read(reinterpret_cast<char*>(&length), sizeof(uint64_t));
        return !file.fail();
    }


    bool MIF::readChunkMagic(uint64_t pos, uint64_t length)
    {
        // check that at begin and nowhere else
        if (pos != 0) return false;

        // read version
        uint64_t version;
        file.read(reinterpret_cast<char*>(&version), sizeof(uint64_t));

        // jump over data
        file.seekg(length - 1 * sizeof(uint64_t), std::ios_base::cur);

        return !file.fail();
    }

    bool MIF::readChunkImage(uint64_t pos, uint64_t length)
    {
        // save information of this image
        ImageInfo info;
        info.pos = pos;
        info.length = length;

        // read image chunk head - index
        uint64_t index;
        file.read(reinterpret_cast<char*>(&index), sizeof(uint64_t));

        // read flags
        file.read(reinterpret_cast<char*>(&info.flags), sizeof(uint64_t));

        // read DataType
        uint64_t dt;
        file.read(reinterpret_cast<char*>(&dt), sizeof(uint64_t));
        info.dataType = static_cast<DataType>(dt);

        // read image size
        uint64_t val;
        file.read(reinterpret_cast<char*>(&val), sizeof(uint64_t));
        info.height = static_cast<size_t>(val);
        file.read(reinterpret_cast<char*>(&val), sizeof(uint64_t));
        info.width = static_cast<size_t>(val);
        file.read(reinterpret_cast<char*>(&val), sizeof(uint64_t));
        info.np = static_cast<size_t>(val);

        // insert into map and possibly vector
        insertImageInfo(index, info);

        // jump over data
        file.seekg(length - 6 * sizeof(uint64_t), std::ios_base::cur);

        return !file.fail();
    }

    bool MIF::readChunkXML(uint64_t pos, uint64_t length)
    {
        char *data = new char[length];
        file.read(data, length);
        xml.pos = pos;
        xml.length = length;
        xml.str = data;
        delete[] data;
        return !file.fail();
    }

    template<typename T>
    T MIF::readImageElement(const ImageInfo & info, size_t row, size_t col, size_t plane)
    {
        file.seekg(info.pos + 8 * sizeof(uint64_t) + ((row * info.width + col) * info.np + plane) * typeSizes[static_cast<size_t>(info.dataType)]);
        T data;
        file.read(reinterpret_cast<char*>(&data), sizeof(T));
        return data;
    }

    template<typename T>
    void MIF::readImagePixel(const ImageInfo & info, size_t row, size_t col, T * data)
    {
        file.seekg(info.pos + 8 * sizeof(uint64_t) + ((row * info.width + col) * info.np) * typeSizes[static_cast<size_t>(info.dataType)]);
        file.read(reinterpret_cast<char*>(data), info.np * sizeof(T));
    }

    void MIF::writeChunkMagic()
    {
        if (read_only) return;
        file.write(reinterpret_cast<const char*>(&type_magic), sizeof(uint64_t));
        uint64_t length = sizeof(uint64_t);
        file.write(reinterpret_cast<const char*>(&length), sizeof(uint64_t));
        file.write(reinterpret_cast<const char*>(&version), sizeof(uint64_t));
        file.flush();
    }

    void MIF::writeChunkImage(size_t index, const ImageInfo & info, const char * data)
    {
        // check that file is open
        if (!file.is_open()) throw std::ios_base::failure("file not open");

        // go to position and check that stream is ok
        file.clear();
        file.seekp(info.pos);
        if (file.fail()) throw std::ios_base::failure("file not ready");

        // write all data
        // write type: image
        file.write(reinterpret_cast<const char*>(&type_image), sizeof(uint64_t));
        // write length
        file.write(reinterpret_cast<const char*>(&info.length), sizeof(uint64_t));
        // write index
        uint64_t val = static_cast<size_t>(index);
        file.write(reinterpret_cast<const char*>(&val), sizeof(uint64_t));
        // write flags
        file.write(reinterpret_cast<const char*>(&info.flags), sizeof(uint64_t));
        // write dataType
        file.write(reinterpret_cast<const char*>(&info.dataType), sizeof(uint64_t));
        // write height
        val = static_cast<uint64_t>(info.height);
        file.write(reinterpret_cast<const char*>(&val), sizeof(uint64_t));
        // write width
        val = static_cast<uint64_t>(info.width);
        file.write(reinterpret_cast<const char*>(&val), sizeof(uint64_t));
        // write np
        val = static_cast<uint64_t>(info.np);
        file.write(reinterpret_cast<const char*>(&val), sizeof(uint64_t));
        // write image data
        file.write(data, computeImageDataLength(info));
        file.flush();

        // check that stream is ok
        if (file.fail()) throw std::ios_base::failure("error during writing");
    }

    void MIF::writeChunkXML()
    {
        // check that file is open
        if (!file.is_open()) throw std::ios_base::failure("file not open");

        // go to position and check that stream is ok
        file.clear();
        file.seekp(xml.pos);
        if (file.fail()) throw std::ios_base::failure("file not ready");

        // write all data
        // write type: image
        file.write(reinterpret_cast<const char*>(&type_xml), sizeof(uint64_t));
        // write length
        file.write(reinterpret_cast<const char*>(&xml.length), sizeof(uint64_t));
        // write string
        file.write(xml.str.data(), xml.length);
        file.flush();

        // check that stream is ok
        if (file.fail()) throw std::ios_base::failure("error during writing");
    }

    void MIF::addFreeSpace(uint64_t pos, uint64_t length)
    {
        // add free space
        uint64_t len_chunk = -1;
        if (length != -1) len_chunk = 2 * sizeof(uint64_t) + length; // adding 2 for chunk head
        freeSpaces.insert({ pos, len_chunk });

        // join consecutive free spaces
        for (auto it1 = freeSpaces.begin(), it2 = freeSpaces.begin(); it2 != freeSpaces.end();) {
            if (it2 == freeSpaces.begin()) {
                ++it2;
                continue;
            }
            if (it1->first + it1->second > it2->first) {
                if (it2->second == -1) it1->second = -1;
                else it1->second = std::max(it1->second, (it2->first - it1->first) + it2->second);
                it2 = freeSpaces.erase(it2);
            }
            else {
                ++it2;
            }
            ++it1;
        }
    }

    uint64_t MIF::findAndPrepareFreeSpace(uint64_t length)
    {
        // try to find exact match
        uint64_t len_chunk = 2 * sizeof(uint64_t) + length; // adding 2 for chunk head
        for (auto it = freeSpaces.begin(); it != freeSpaces.end(); ++it) {
            if (it->second == length) return it->first;
        }
        // find the smallest usable space
        size_t len_chunk2 = len_chunk + 2 * sizeof(uint64_t); // adding 2 for chunk head plus 2 for head of empty chunk that must follow
        for (auto it = freeSpaces.begin(); it != freeSpaces.end(); ++it) {
            if (it->second >= len_chunk2) {
                // save this position and length
                uint64_t pos = it->first;
                uint64_t len = it->second;
                // compute new position and length of following space
                uint64_t pos2 = pos + len_chunk;
                uint64_t len2 = -1;
                if (len != -1) len2 = len - len_chunk;
                // remove current free space and insert new one
                freeSpaces.erase(it);
                freeSpaces.insert({ pos2, len2 });
                // if not after end of file
                if (pos2 < pos_end) {
                    // write head of empty chunk after selected space
                    if (read_only) return 0;
                    file.seekp(pos);
                    file.write(reinterpret_cast<const char*>(&type_empty), sizeof(uint64_t));
                    uint64_t len_empty = -1;
                    // if not end of file free space, compute new length
                    if (it->second != -1) it->second -= len_chunk;
                    file.write(reinterpret_cast<const char*>(&(it->second)), sizeof(uint64_t));
                    file.flush();
                }
                else {
                    pos_end = pos2;
                }
                return pos;
            }
        }
        // never comes here
        return 0;
    }

    template void MIF::addImage(size_t image_index, const Image<uint8_t> & image, uint64_t flags);
    template void MIF::addImage(size_t image_index, const Image<uint16_t> & image, uint64_t flags);
    template void MIF::addImage(size_t image_index, const Image<uint32_t> & image, uint64_t flags);
    template void MIF::addImage(size_t image_index, const Image<uint64_t> & image, uint64_t flags);
    template void MIF::addImage(size_t image_index, const Image<int8_t> & image, uint64_t flags);
    template void MIF::addImage(size_t image_index, const Image<int16_t> & image, uint64_t flags);
    template void MIF::addImage(size_t image_index, const Image<int32_t> & image, uint64_t flags);
    template void MIF::addImage(size_t image_index, const Image<int64_t> & image, uint64_t flags);
    // todo: template void MIF::addImage(size_t image_index, const Image<half> & image, uint64_t flags);
    template void MIF::addImage(size_t image_index, const Image<float> & image, uint64_t flags);
    template void MIF::addImage(size_t image_index, const Image<double> & image, uint64_t flags);

    template uint64_t MIF::pushImage(const Image<uint8_t> & image, uint64_t flags);
    template uint64_t MIF::pushImage(const Image<uint16_t> & image, uint64_t flags);
    template uint64_t MIF::pushImage(const Image<uint32_t> & image, uint64_t flags);
    template uint64_t MIF::pushImage(const Image<uint64_t> & image, uint64_t flags);
    template uint64_t MIF::pushImage(const Image<int8_t> & image, uint64_t flags);
    template uint64_t MIF::pushImage(const Image<int16_t> & image, uint64_t flags);
    template uint64_t MIF::pushImage(const Image<int32_t> & image, uint64_t flags);
    template uint64_t MIF::pushImage(const Image<int64_t> & image, uint64_t flags);
    // todo: template uint64_t MIF::pushImage(const Image<half> & image, uint64_t flags);
    template uint64_t MIF::pushImage(const Image<float> & image, uint64_t flags);
    template uint64_t MIF::pushImage(const Image<double> & image, uint64_t flags);

    template void MIF::updateImage(size_t image_index, const Image<uint8_t> & image, uint64_t flags);
    template void MIF::updateImage(size_t image_index, const Image<uint16_t> & image, uint64_t flags);
    template void MIF::updateImage(size_t image_index, const Image<uint32_t> & image, uint64_t flags);
    template void MIF::updateImage(size_t image_index, const Image<uint64_t> & image, uint64_t flags);
    template void MIF::updateImage(size_t image_index, const Image<int8_t> & image, uint64_t flags);
    template void MIF::updateImage(size_t image_index, const Image<int16_t> & image, uint64_t flags);
    template void MIF::updateImage(size_t image_index, const Image<int32_t> & image, uint64_t flags);
    template void MIF::updateImage(size_t image_index, const Image<int64_t> & image, uint64_t flags);
    // todo: template void MIF::updateImage(size_t image_index, const Image<half> & image, uint64_t flags);
    template void MIF::updateImage(size_t image_index, const Image<float> & image, uint64_t flags);
    template void MIF::updateImage(size_t image_index, const Image<double> & image, uint64_t flags);

    template Image<uint8_t> MIF::getImage(size_t image_index);
    template Image<uint16_t> MIF::getImage(size_t image_index);
    template Image<uint32_t> MIF::getImage(size_t image_index);
    template Image<uint64_t> MIF::getImage(size_t image_index);
    template Image<int8_t> MIF::getImage(size_t image_index);
    template Image<int16_t> MIF::getImage(size_t image_index);
    template Image<int32_t> MIF::getImage(size_t image_index);
    template Image<int64_t> MIF::getImage(size_t image_index);
    // todo: template Image<half> MIF::getImage(size_t image_index);
    template Image<float> MIF::getImage(size_t image_index);
    template Image<double> MIF::getImage(size_t image_index);

    template uint8_t MIF::get(size_t image_index, size_t row, size_t col, size_t plane);
    template uint16_t MIF::get(size_t image_index, size_t row, size_t col, size_t plane);
    template uint32_t MIF::get(size_t image_index, size_t row, size_t col, size_t plane);
    template uint64_t MIF::get(size_t image_index, size_t row, size_t col, size_t plane);
    template int8_t MIF::get(size_t image_index, size_t row, size_t col, size_t plane);
    template int16_t MIF::get(size_t image_index, size_t row, size_t col, size_t plane);
    template int32_t MIF::get(size_t image_index, size_t row, size_t col, size_t plane);
    template int64_t MIF::get(size_t image_index, size_t row, size_t col, size_t plane);
    // todo: template half MIF::get(size_t image_index, size_t row, size_t col, size_t plane);
    template float MIF::get(size_t image_index, size_t row, size_t col, size_t plane);
    template double MIF::get(size_t image_index, size_t row, size_t col, size_t plane);

    template uint8_t MIF::at(size_t image_index, size_t row, size_t col, size_t plane);
    template uint16_t MIF::at(size_t image_index, size_t row, size_t col, size_t plane);
    template uint32_t MIF::at(size_t image_index, size_t row, size_t col, size_t plane);
    template uint64_t MIF::at(size_t image_index, size_t row, size_t col, size_t plane);
    template int8_t MIF::at(size_t image_index, size_t row, size_t col, size_t plane);
    template int16_t MIF::at(size_t image_index, size_t row, size_t col, size_t plane);
    template int32_t MIF::at(size_t image_index, size_t row, size_t col, size_t plane);
    template int64_t MIF::at(size_t image_index, size_t row, size_t col, size_t plane);
    // todo: template half MIF::at(size_t image_index, size_t row, size_t col, size_t plane);
    template float MIF::at(size_t image_index, size_t row, size_t col, size_t plane);
    template double MIF::at(size_t image_index, size_t row, size_t col, size_t plane);

    template void MIF::getPixel(size_t image_index, size_t row, size_t col, uint8_t * values);
    template void MIF::getPixel(size_t image_index, size_t row, size_t col, uint16_t * values);
    template void MIF::getPixel(size_t image_index, size_t row, size_t col, uint32_t * values);
    template void MIF::getPixel(size_t image_index, size_t row, size_t col, uint64_t * values);
    template void MIF::getPixel(size_t image_index, size_t row, size_t col, int8_t * values);
    template void MIF::getPixel(size_t image_index, size_t row, size_t col, int16_t * values);
    template void MIF::getPixel(size_t image_index, size_t row, size_t col, int32_t * values);
    template void MIF::getPixel(size_t image_index, size_t row, size_t col, int64_t * values);
    // todo: template void MIF::getPixel(size_t image_index, size_t row, size_t col, half * values);
    template void MIF::getPixel(size_t image_index, size_t row, size_t col, float * values);
    template void MIF::getPixel(size_t image_index, size_t row, size_t col, double * values);

    template void MIF::atPixel(size_t image_index, size_t row, size_t col, uint8_t * values);
    template void MIF::atPixel(size_t image_index, size_t row, size_t col, uint16_t * values);
    template void MIF::atPixel(size_t image_index, size_t row, size_t col, uint32_t * values);
    template void MIF::atPixel(size_t image_index, size_t row, size_t col, uint64_t * values);
    template void MIF::atPixel(size_t image_index, size_t row, size_t col, int8_t * values);
    template void MIF::atPixel(size_t image_index, size_t row, size_t col, int16_t * values);
    template void MIF::atPixel(size_t image_index, size_t row, size_t col, int32_t * values);
    template void MIF::atPixel(size_t image_index, size_t row, size_t col, int64_t * values);
    // todo: template void MIF::atPixel(size_t image_index, size_t row, size_t col, half * values);
    template void MIF::atPixel(size_t image_index, size_t row, size_t col, float * values);
    template void MIF::atPixel(size_t image_index, size_t row, size_t col, double * values);
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
