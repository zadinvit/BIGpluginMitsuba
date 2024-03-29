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
* Version History of class ElementColors:
*  1.0              First stable version.
*
* TODO List of class ElementColors:
*   - implement getColorChannels method
*   - implement unit tests
*   - add noexcept specifiers
*/

#include "../include/ElementColors.hpp"

namespace mif
{
    ElementColors::ElementColors(const std::string & id, const std::string & color_space, const std::string & illuminant, const Attributes & attributes, const elements_t & elements)
        : Element("colors", elements, attributes)
    {
        setId(id);
        setColorSpace(color_space);
        setIlluminant(illuminant);
    }

    ElementColors::ElementColors(const Element & element) : Element(element)
    {
        if (element.getElementName() != "colors") clear();
    }

    size_t ElementColors::getColorChannelsCount() const
    {
        size_t count = 0;
        for (const auto & element : getChildElement("channels").getChildElements()) {
            if (element.getElementName() == "channel" && element.isIndex()) ++count;
        }
        return count;
    }

    std::vector<ElementColors::index_t> ElementColors::getColorChannelsIndices() const
    {
        std::vector<index_t> indices;
        for (const auto & element : getChildElement("channels").getChildElements()) {
            if (element.getElementName() == "channel" && element.isIndex()) indices.push_back(element.getIndex());
        }
        return indices;
    }

    std::vector<std::string> ElementColors::getColorChannels() const
    {
        std::vector<std::string> labels;
        for (const auto & element : getChildElement("channels").getChildElements()) {
            if (element.getElementName() == "channel" && element.isIndex()) {
                index_t index = element.getIndex();
                if (index >= labels.size()) labels.resize(index + 1);
                labels.push_back(element.getAttribute("label"));
            }
        }
        return labels;
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
