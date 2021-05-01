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
* Version History of class ElementDirections:
*  1.0              First stable version.
*
* TODO List of class ElementDirections:
*   - implement unit tests
*   - add noexcept specifiers
*/

#include "../include/ElementDirections.hpp"

namespace mif
{
    ElementDirections::ElementDirections(const std::string & id, const std::vector<ElementDirectionsItem> & items, const Attributes & attributes, const elements_t & elements)
        : Element("directions", elements, attributes), items(items)
    {
        setAttribute("type", "list");
        setId(id);
    }

    ElementDirections::ElementDirections(const Element & element, const std::string & default_units)
        : Element(element)
    {
        if (element.getElementName() != "directions" || element.getAttribute("type") != "list") {
            clear();
            return;
        }
        std::string units = default_units;
        if (element.isAttribute("units")) units = element.getAttribute("units");
        for (const auto & element : getChildElements()) {
            if (element.getElementName() == "directions-item") {
                ElementDirectionsItem item(element, units);
                items.push_back(item);
            }
        }
    }

    Element ElementDirections::toElement(bool reference_only) const
    {
        Element e = *this;
        bool addUnits = true;
        if (sameUnits()) {
            e.setAttribute("units", getUnits());
            addUnits = false;
        }
        if (reference_only) {
            e.setAttribute("reference", e.getId());
            e.removeId();
            e.removeChildElements();
        }
        else {
            for (const auto & item : items) {
                e.pushChildElement(item.toElement(addUnits));
            }
        }
        e.setAttribute("count", sizeToString(items.size()));
        return e;
    }

    void ElementDirections::setDirectionsItem(const ElementDirectionsItem & item)
    {
        for (auto & i : items) {
            if (i.getIndex() == item.getIndex()) {
                i = item;
                return;
            }
        }
        items.push_back(item);
    }

    bool ElementDirections::isDirectionsItem(const index_t & index) const
    {
        for (const auto & item : items) {
            if (item.getIndex() == index) return true;
        }
        return false;
    }

    ElementDirectionsItem & ElementDirections::getDirectionsItem(const index_t & index)
    {
        for (auto & item : items) {
            if (item.getIndex() == index) return item;
        }
        items.push_back(ElementDirectionsItem(index, {}));
        return items.back();
    }

    const ElementDirectionsItem & ElementDirections::getDirectionsItem(const index_t & index) const
    {
        for (const auto & item : items) {
            if (item.getIndex() == index) return item;
        }
        static ElementDirectionsItem empty;
        return empty;
    }

    bool ElementDirections::sameUnits() const
    {
        std::string units = getUnits();
        for (const auto & item : items) {
            if (item.getUnits() != units) return false;
        }
        return true;
    }

    std::string ElementDirections::getUnits() const
    {
        if (!items.empty()) return items.front().getUnits();
        return "deg";
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
