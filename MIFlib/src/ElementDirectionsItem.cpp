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
* Version History of class ElementDirectionsItem:
*  1.0              First stable version.
*
* TODO List of class ElementDirectionsItem:
*   - implement unit tests
*   - add noexcept specifiers
*/

#include "../include/ElementDirectionsItem.hpp"

namespace mif
{
    ElementDirectionsItem::ElementDirectionsItem(const index_t & index, const std::vector<ElementDirection> & directions, const Attributes & attributes, const elements_t & elements)
        : Element("directions-item", elements, attributes), directions(directions)
    {
        setIndex(index);
    }

    ElementDirectionsItem::ElementDirectionsItem(const Element & element, const std::string & default_units)
        : Element(element)
    {
        if (element.getElementName() != "directions-item") {
            clear();
            return;
        }
        std::string units = default_units;
        if (element.isAttribute("units")) units = element.getAttribute("units");
        for (const auto & element : getChildElements()) {
            if (element.getElementName() == "direction") {
                ElementDirection direction(element, units);
                directions.push_back(direction);
            }
        }
    }

    Element ElementDirectionsItem::toElement(bool addUnits) const
    {
        Element e = *this;
        if (sameUnits()) {
            if (addUnits) e.setAttribute("units", getUnits());
            addUnits = false;
        }
        for (const auto & direction : directions) {
            e.pushChildElement(direction.toElement(addUnits));
        }
        return e;
    }

    void ElementDirectionsItem::setDirection(const ElementDirection & direction)
    {
        for (auto & d : directions) {
            if (d.getIndex() == direction.getIndex()) {
                d = direction;
                return;
            }
        }
        directions.push_back(direction);
    }

    bool ElementDirectionsItem::isDirection(const index_t & index) const
    {
        for (const auto & direction : directions) {
            if (direction.getIndex() == index) return true;
        }
        return false;
    }

    ElementDirection & ElementDirectionsItem::getDirection(const index_t & index)
    {
        for (auto & direction : directions) {
            if (direction.getIndex() == index) return direction;
        }
        directions.push_back(ElementDirection(index, DirectionType::sensor, 0, 0));
        return directions.back();
    }

    const ElementDirection & ElementDirectionsItem::getDirection(const index_t & index) const
    {
        for (const auto & direction : directions) {
            if (direction.getIndex() == index) return direction;
        }
        static ElementDirection empty;
        return empty;
    }

    bool ElementDirectionsItem::sameUnits() const
    {
        std::string units = getUnits();
        for (const auto & direction : directions) {
            if (direction.getUnits() != units) return false;
        }
        return true;
    }

    std::string ElementDirectionsItem::getUnits() const
    {
        if (!directions.empty()) return directions.front().getUnits();
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
