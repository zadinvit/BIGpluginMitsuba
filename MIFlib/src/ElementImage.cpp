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
* Version History of class ElementImage:
*  1.0              First stable version.
*  1.1              Added new constructor, tests for empty parameters.
*
* TODO List of class ElementImage:
*   - implement unit tests
*   - add noexcept specifiers
*/

#include "../include/ElementImage.hpp"

namespace mif
{
    ElementImage::ElementImage(const index_t & index, const Attributes & attributes, const elements_t & elements)
        : Element("image", elements, attributes)
    {
        setIndex(index);
    }

    ElementImage::ElementImage(const index_t & index, const std::string & material_id, const std::string & device_id, const std::string & area_id, const std::string & colors_id, const Attributes & attributes, const elements_t & elements)
        : ElementImage(index, attributes, elements)
    {
        if (!material_id.empty()) setMaterialId(material_id);
        if (!device_id.empty()) setDeviceId(device_id);
        if (!area_id.empty()) setAreaId(area_id);
        if (!colors_id.empty()) setColorsId(colors_id);
    }

    ElementImage::ElementImage(const index_t & index, const ElementMaterial & material, const ElementDevice & device, const ElementArea & area, const ElementColors & colors, const Attributes & attributes, const elements_t & elements)
        : ElementImage(index, attributes, elements)
    {
        if (!material.getId().empty()) setMaterialId(material.getId());
        if (!device.getId().empty()) setDeviceId(device.getId());
        if (!area.getId().empty()) setAreaId(area.getId());
        if (!colors.getId().empty()) setColorsId(colors.getId());
    }

    ElementImage::ElementImage(const Element & element) : Element(element)
    {
        if (element.getElementName() != "image") clear();
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
