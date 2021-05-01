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

#include <vector>
#include "Element.hpp"
#include "ElementMaterial.hpp"
#include "ElementDevice.hpp"
#include "ElementArea.hpp"
#include "ElementColors.hpp"
#include "ElementTimestamps.hpp"
#include "ElementDirectionsItem.hpp"

namespace mif
{
    class ElementImage : public Element
    {
    public: // Types section.
        typedef Element::elements_t elements_t;

    public: // Constructors section.

        // Empty constructor.
        ElementImage() : Element("image") {}

        // Fill constructor.
        explicit ElementImage(const index_t & index, const Attributes & attributes = {}, const elements_t & elements = {});

        // Fill constructor.
        explicit ElementImage(const index_t & index, const std::string & material_id, const std::string & device_id = {}, const std::string & area_id = {}, const std::string & colors_id = {}, const Attributes & attributes = {}, const elements_t & elements = {});

        // Fill constructor.
        explicit ElementImage(const index_t & index, const ElementMaterial & material, const ElementDevice & device = {}, const ElementArea & area = {}, const ElementColors & colors = {}, const Attributes & attributes = {}, const elements_t & elements = {});

        // Converting constructor. Converts Element into ElementImage.
        explicit ElementImage(const Element & element);

    public: // Conversions sections.

        // Converts ElementImage into Element.
        Element toElement() const { return *this; }

    public: // Set property section.

        // Sets reference to material with given id.
        void setMaterialId(const std::string & id) { getChildElement("material").setAttribute("reference", id); }

        // Sets reference to device with given id.
        void setDeviceId(const std::string & id) { getChildElement("device").setAttribute("reference", id); }

        // Sets reference to area with given id.
        void setAreaId(const std::string & id) { getChildElement("area").setAttribute("reference", id); }

        // Sets reference to colors with given id.
        void setColorsId(const std::string & id) { getChildElement("colors").setAttribute("reference", id); }

        // Sets timestamps.
        void setTimestamps(const ElementTimestamps & timestamps) { getChildElement("timestamps") = timestamps.toElement(); }

        // Sets directions item.
        void setDirectionsItem(const ElementDirectionsItem & item) { getChildElement("directions-item") = item.toElement(); }

    public: // Get property section.

        // Returns id of referenced material.
        const std::string & getMaterialId() const { return getChildElement("material").getAttribute("reference"); }

        // Returns id of referenced device.
        const std::string & getDeviceId() const { return getChildElement("device").getAttribute("reference"); }

        // Returns id of referenced area.
        const std::string & getAreaId() const { return getChildElement("area").getAttribute("reference"); }

        // Returns id of referenced colors.
        const std::string & getColorsId() const { return getChildElement("colors").getAttribute("reference"); }

        // Returns timestamps.
        ElementTimestamps getTimestamps() const { return ElementTimestamps(getChildElement("timestamps")); }

        // Returns directions item.
        ElementDirectionsItem getDirectionsItem() const { return ElementDirectionsItem(getChildElement("directions-item")); }
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
