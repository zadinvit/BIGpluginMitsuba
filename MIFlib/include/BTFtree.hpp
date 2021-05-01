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

#include <string>
#include "Element.hpp"
#include "ElementMaterial.hpp"
#include "ElementDevice.hpp"
#include "ElementArea.hpp"
#include "ElementColors.hpp"
#include "ElementBtf.hpp"
#include "ElementImage.hpp"
#include "ElementDirections.hpp"

namespace mif
{
    class BTFtree : public Element
    {
    public: // Constructors section.

        // Default constructor. Creates empty tree with root element named by given root_name.
        explicit BTFtree(const std::string & root_name = "root") : Element(root_name) {}

    public: // Convenient data access section - material.

        // Returns IDs of all materials.
        std::vector<std::string> getMaterialIDs() const { return getChildElementsIDs("material"); }

        // Returns true if material element with given id exists.
        bool isMaterial(const std::string & id) const { return isChildElement({ { "material", id } }); }

        // Adds top-level material element.
        void setMaterial(const ElementMaterial & material) { getChildElement({ { "material", material.getId() } }) = material.toElement(); }

        // Returns top-level material element.
        ElementMaterial getMaterial(const std::string & id) const { return ElementMaterial(getChildElement({ { "material", id } })); }

    public: // Convenient data access section - device.

        // Returns IDs of all devices.
        std::vector<std::string> getDeviceIDs() const { return getChildElementsIDs("device"); }

        // Returns true if device element with given id exists.
        bool isDevice(const std::string & id) const { return isChildElement({ { "device", id } }); }

        // Adds top-level device element.
        void setDevice(const ElementDevice & device) { getChildElement({ { "device", device.getId() } }) = device.toElement(); }

        // Returns top-level material element.
        ElementDevice getDevice(const std::string & id) const { return ElementDevice(getChildElement({ { "device", id } })); }

    public: // Convenient data access section - area.

        // Returns IDs of all areas.
        std::vector<std::string> getAreaIDs() const { return getChildElementsIDs("area"); }

        // Returns true if area element with given id exists.
        bool isArea(const std::string & id) const { return isChildElement({ { "area", id } }); }

        // Adds top-level area element.
        void setArea(const ElementArea & area) { getChildElement({ { "area", area.getId() } }) = area.toElement(); }

        // Returns top-level area element.
        ElementArea getArea(const std::string & id) const { return ElementArea(getChildElement({ { "area", id } })); }

    public: // Convenient data access section - colors.

        // Returns IDs of all colors elements.
        std::vector<std::string> getColorsIDs() const { return getChildElementsIDs("colors"); }

        // Returns true if colors element with given id exists.
        bool isColors(const std::string & id) const { return isChildElement({ { "colors", id } }); }

        // Adds top-level colors element.
        void setColors(const ElementColors & colors) { getChildElement({ { "colors", colors.getId() } }) = colors.toElement(); }

        // Returns top-level colors element.
        ElementColors getColors(const std::string & id) const { return ElementColors(getChildElement({ { "colors", id } })); }

    public: // Convenient data access section - btf.

        // Returns IDs of all btf.
        std::vector<std::string> getBtfIDs() const { return getChildElementsIDs("btf"); }

        // Returns true if btf element with given id exists.
        bool isBtf(const std::string & id) const { return isChildElement({ { "btf", id } }); }

        // Adds btf element.
        void setBtf(const ElementBtf & btf) { getChildElement({ { "btf", btf.getId() } }) = btf.toElement(); }

        // Returns top-level colors element.
        ElementBtf getBtf(const std::string & id) const { return ElementBtf(getChildElement({ { "btf", id } })); }

    public: // Convenient data access section - images.

        // Returns indices of all images.
        std::vector<index_t> getImageElementsIndices() const { return getChildElementsIndices("image", { { "images",{} } }); }

        // Returns true if image with given index exists.
        bool isImageElement(const index_t & index) const { return isChildElement("image", index, { { "images", "" } }); }

        // Adds image element.
        void setImageElement(const ElementImage & image) { getChildElement("images").getChildElement("image", image.getIndex()) = image.toElement(); }

        // Returns top-level colors element.
        ElementImage getImageElement(const index_t & index) const { return ElementImage(getChildElement("images").getChildElement("image", index)); }

    public: // Convenient data access section - directions.

        // Returns IDs of all directions.
        std::vector<std::string> getDirectionsIDs() const { return getChildElementsIDs("directions"); }

        // Returns true if directions element with given id exists.
        bool isDirections(const std::string & id) const { return isChildElement({ { "directions", id } }); }

        // Adds top-level directions element.
        void setDirections(const ElementDirections & directions) { getChildElement({ { "directions", directions.getId() } }) = directions.toElement(); }

        // Returns top-level directions element.
        ElementDirections getDirections(const std::string & id) const { return ElementDirections(getChildElement({ { "directions", id } })); }
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
