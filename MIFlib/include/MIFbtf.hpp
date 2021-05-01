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

#include "BTFtree.hpp"
#include "MIF.hpp"
#include "ext/Array4D.hpp"

namespace mif
{
    class MIFbtf : public BTFtree, public MIF
    {
    public:
        // Constructor. If fileName is not empty, opens file. If it is new file then writes header, else reads parameters of all images.
        explicit MIFbtf(const std::string & fileName = {}, bool read_only = false);

        // Opens file. If it is new file then writes header, else reads parameters of all images. Returns true if succeeds.
        bool open(const std::string & fileName, bool read_only = false) override;

        // Saves XML into data file.
        void saveXML();

        // Saves XML into separate file with given name.
        void saveXMLtoSeparateFile(const std::string & fileName);

        // Loads XML from separate file with given name.
        void loadXMLfromSeparateFile(const std::string & fileName);

        // Returns true if all images associated with btf specified by given id are of the same size.
        bool isBtfImagesSameSize(const std::string & btf_id);

        class CacheBase
        {
        public:
            // Constructor.
            CacheBase(MIFbtf & mif, const std::string & btf_id) : mif(mif), indices(mif.getBtf(btf_id).getImageIndices()) {}

            // Returns number of contained images.
            size_t getNumberOfImages() const { return indices.size(); }

            // Returns number of rows of image with given index.
            size_t getImageHeight(size_t index) const { return index < indices.size() ? mif.getImageHeight(indices[index]) : 0; }

            // Returns number of columns of image with given index.
            size_t getImageWidth(size_t index) const { return index < indices.size() ? mif.getImageWidth(indices[index]) : 0; }

            // Returns number of (color) planes of image with given index.
            size_t getImageNumberOfPlanes(size_t index) const { return index < indices.size() ? mif.getImageNumberOfPlanes(indices[index]) : 0; }

            // Returns size of image with given index.
            size_t getImageSize(size_t index) const { return index < indices.size() ? mif.getImageSize(indices[index]) : 0; };

            // Returns whole image with given index.
            virtual image::Image<float> getImage(size_t index) const = 0;

            // Returns element from the given image at given row, column and (color) plane. Does not check bounds.
            virtual const float & get(size_t index, size_t row, size_t col, size_t plane) const = 0;

            // Returns element from the given image at given row, column and (color) plane. Checks bounds.
            virtual const float & at(size_t index, size_t row, size_t col, size_t plane) const = 0;

            // Returns pixel (all its planes) from the given image at given row and column. Does not check bounds.
            virtual void getPixel(size_t index, size_t row, size_t col, float * values) const = 0;

            // Returns pixel (all its planes) from the given image at given row and column. Checks bounds.
            virtual void atPixel(size_t index, size_t row, size_t col, float * values) const = 0;

        protected:
            MIFbtf & mif;
            std::vector<size_t> indices;
        };

        class CacheNone : public CacheBase
        {
        public:
            // Constructor.
            CacheNone(MIFbtf & mif, const std::string & btf_id) : CacheBase(mif, btf_id) { }

            // Returns whole image with given index. Does not use cache.
            image::Image<float> getImage(size_t index) const { if (index < indices.size()) return mif.getImage<float>(indices[index]); return image::Image<float>(); }

            // Returns element from the given image at given row, column and (color) plane. Does not check bounds.
            const float & get(size_t index, size_t row, size_t col, size_t plane) const { return mif.get<float>(indices[index], row, col, plane); }

            // Returns element from the given image at given row, column and (color) plane. Checks bounds.
            const float & at(size_t index, size_t row, size_t col, size_t plane) const { if (index >= indices.size()) throw std::out_of_range("index out of range"); return mif.at<float>(indices[index], row, col, plane); }

            // Returns pixel (all its planes) from the given image at given row and column. Does not check bounds.
            void getPixel(size_t index, size_t row, size_t col, float * values) const { mif.getPixel(indices[index], row, col, values); }

            // Returns pixel (all its planes) from the given image at given row and column. Checks bounds.
            void atPixel(size_t index, size_t row, size_t col, float * values) const { if (index >= indices.size()) throw std::out_of_range("index out of range"); mif.atPixel(indices[index], row, col, values); }
        };

        //// todo
        //class CachePart : public CacheBase
        //{
        //public:
        //    // Constructor.
        //    CachePart(MIFbtf & mif, const std::string & btf_id) : CacheBase(mif, btf_id) { }
        //};




        class CacheBaseSame : public CacheBase
        {
        public:
            // Constructor.
            CacheBaseSame(MIFbtf & mif, const std::string & btf_id) : CacheBase(mif, btf_id)
            {
                // check all images same size
                if (!mif.isBtfImagesSameSize(btf_id)) indices.clear();
            }

            // Returns number of rows of image.
            virtual size_t getImageHeight() const { return indices.size() ? CacheBase::getImageHeight(0) : 0; }

            // Returns number of columns of image.
            virtual size_t getImageWidth() const { return indices.size() ? CacheBase::getImageWidth(0) : 0; }

            // Returns number of (color) planes of image.
            virtual size_t getImageNumberOfPlanes() const { return indices.size() ? CacheBase::getImageNumberOfPlanes(0) : 0; }

            // Returns size of image.
            virtual size_t getImageSize() const { return getImageHeight() * getImageWidth() * getImageNumberOfPlanes(); };
        };

        class CacheNoneSame : public CacheBaseSame
        {
        public:
            // Constructor.
            CacheNoneSame(MIFbtf & mif, const std::string & btf_id) : CacheBaseSame(mif, btf_id) { }

            // Returns whole image with given index. Does not use cache.
            image::Image<float> getImage(size_t index) const { if (index < indices.size()) return mif.getImage<float>(indices[index]); return image::Image<float>(); }

            // Returns element from the given image at given row, column and (color) plane. Does not check bounds.
            const float & get(size_t index, size_t row, size_t col, size_t plane) const { return mif.get<float>(indices[index], row, col, plane); }

            // Returns element from the given image at given row, column and (color) plane. Checks bounds.
            const float & at(size_t index, size_t row, size_t col, size_t plane) const { if (index >= indices.size()) throw std::out_of_range("index out of range"); return mif.at<float>(indices[index], row, col, plane); }

            // Returns pixel (all its planes) from the given image at given row and column. Does not check bounds.
            void getPixel(size_t index, size_t row, size_t col, float * values) const { mif.getPixel(indices[index], row, col, values); }

            // Returns pixel (all its planes) from the given image at given row and column. Checks bounds.
            void atPixel(size_t index, size_t row, size_t col, float * values) const { if (index >= indices.size()) throw std::out_of_range("index out of range"); mif.atPixel(indices[index], row, col, values); }
        };


        class CacheWholeSame : public CacheBaseSame
        {
        public:
            CacheWholeSame(MIFbtf & mif, const std::string & btf_id = {});

            // Returns number of rows of image.
            size_t getImageHeight() const override { return data.second(); }

            // Returns number of columns of image.
            size_t getImageWidth() const override { return data.third(); }

            // Returns number of (color) planes of image.
            size_t getImageNumberOfPlanes() const override { return data.fourth(); }

            // Returns size of image.
            size_t getImageSize() const override { return data.second() * data.third() * data.fourth(); };

            // Returns whole image with given index. TODO: optimize.
            image::Image<float> getImage(size_t index) const override { return image::Image<float>(&data(index, 0, 0, 0), getImageHeight(), getImageWidth(), getImageNumberOfPlanes()); }

            // Returns element from the given image at given row, column and (color) plane. Does not check bounds.
            const float & get(size_t index, size_t row, size_t col, size_t plane) const override { return data(index, row, col, plane); }

            // Returns element from the given image at given row, column and (color) plane. Checks bounds.
            const float & at(size_t index, size_t row, size_t col, size_t plane) const override { return data.at(index, row, col, plane); }

            // Returns pixel (all its planes) from the given image at given row and column. Does not check bounds.
            void getPixel(size_t index, size_t row, size_t col, float * values) const override { const float * p = getPixel(index, row, col); for (size_t i = 0; i != data.fourth(); ++i) values[i] = p[i]; }

            // Non-virtual version of getPixel method for faster access. Returns pixel (all its planes) from the given image at given row and column. Does not check bounds.
            // todo: evaluate and possibly add other direct methods (getDirect, atDirect, atPixelDirect).
            void getPixelDirect(size_t index, size_t row, size_t col, float * values) const { const float * p = getPixel(index, row, col); for (size_t i = 0; i != data.fourth(); ++i) values[i] = p[i]; }

            // Returns pointer to the first (color) plane of a pixel from the given image at given row and column. Does not check bounds.
            // TODO: optimize
            const float * getPixel(size_t index, size_t row, size_t col) const { return &data(index, row, col, 0); }

            // Returns pixel (all its planes) from the given image at given row and column. Checks bounds.
            void atPixel(size_t index, size_t row, size_t col, float * values) const override { const float * p = atPixel(index, row, col); for (size_t i = 0; i != data.fourth(); ++i) values[i] = p[i]; }

            // Returns pointer to the first (color) plane of a pixel from the given image at given row and column. Checks bounds.
            const float * atPixel(size_t index, size_t row, size_t col) const { return &data.at(index, row, col, 0); }

        protected:
            image::Array4D<float> data;
        };


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
