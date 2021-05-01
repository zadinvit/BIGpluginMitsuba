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
* Version History of class MIFbtf:
*  0.1              First stable version.
*
* TODO List of class MIFbtf:
*   - use templates instead of float
*   - implement image type conversions
*   - review code
*   - implement unit tests
*   - implement MIFcachePart
*/

#include "../include/MIFbtf.hpp"

#include <sstream>
#include "../include/TreeXML.hpp"
#include <array>

using image::Image;

namespace mif
{
    MIFbtf::MIFbtf(const std::string & fileName, bool read_only) : BTFtree("mif"), MIF(fileName, read_only)
    {
        if (isXML()) *(static_cast<Element*>(this)) = TreeXML::xmlStringToDataTree(xml.str);
    }

    bool MIFbtf::open(const std::string & fileName, bool read_only)
    {
        if (mif::MIF::open(fileName, read_only)) {
            if (isXML()) *(static_cast<Element*>(this)) = TreeXML::xmlStringToDataTree(xml.str);
            return true;
        }
        return false;
    }

    void MIFbtf::saveXML()
    {
        addXML(TreeXML::dataTreeToXMLstring(*this));
    }

    void MIFbtf::saveXMLtoSeparateFile(const std::string & fileName)
    {
        std::ofstream file(fileName, std::ios_base::out);
        file << TreeXML::dataTreeToXMLstring(*this);
        file.close();
    }

    void MIFbtf::loadXMLfromSeparateFile(const std::string & fileName)
    {
        std::ifstream file(fileName, std::ios_base::in);
        std::stringstream input;
        input << file.rdbuf();
        *(static_cast<Element*>(this)) = TreeXML::xmlStringToDataTree(input.str());
    }

    bool MIFbtf::isBtfImagesSameSize(const std::string & btf_id)
    {
        auto indices = getBtf(btf_id).getImageIndices();
        if (indices.size() == 0) return true;
        size_t h = getImageHeight(indices.front());
        size_t w = getImageWidth(indices.front());
        size_t p = getImageNumberOfPlanes(indices.front());
        for (const auto & index : indices) {
            if (getImageHeight(index) != h) return false;
            if (getImageWidth(index) != w) return false;
            if (getImageNumberOfPlanes(index) != p) return false;
        }
        return true;
    }

    MIFbtf::CacheWholeSame::CacheWholeSame(MIFbtf & mif, const std::string & btf_id) : CacheBaseSame(mif, btf_id)
    {
        // done if no data
        if (indices.empty()) return;

        // create data structure
        size_t h = mif.getImageHeight(indices.front());
        size_t w = mif.getImageWidth(indices.front());
        size_t p = mif.getImageNumberOfPlanes(indices.front());
        data.resize(indices.size(), h, w, p);

        // save all images
        size_t i = 0;
        for (const auto & index : indices) {
            auto b1 = &data(i++, 0, 0, 0);
            Image<float> image = mif.getImage<float>(index);
            std::memcpy(b1, image.data(), getImageSize() * sizeof(float));
            //std::copy(b1, b1 + getImageSize(), image.data());
        }
    }
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
