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
* Version History of class ElementArea:
*  1.0              First stable version.
*
* TODO List of class ElementArea:
*   - implement unit tests
*   - add noexcept specifiers
*/

#include "../include/ElementArea.hpp"

namespace mif
{
    ElementArea::ElementArea(const std::string & id, double top, double left, double height, double width, double whole_height, double whole_width, const std::string & units, const Attributes & attributes, const elements_t & elements)
        : Element("area", elements, attributes), top(top), left(left), height(height), width(width), whole_height(whole_height), whole_width(whole_width), units(units)
    {
        setId(id);
    }

    ElementArea::ElementArea(const Element & element) : Element(element)
    {
        if (element.getElementName() != "area") {
            clear();
            return;
        }
        top = stringToDouble(element.getChildElement("top").getElementText());
        left = stringToDouble(element.getChildElement("left").getElementText());
        height = stringToDouble(element.getChildElement("height").getElementText());
        width = stringToDouble(element.getChildElement("width").getElementText());
        whole_height = stringToDouble(element.getChildElement("whole").getAttribute("height"));
        whole_width = stringToDouble(element.getChildElement("whole").getAttribute("width"));
        units = element.getAttribute("units");
        const Element & resolution = element.getChildElement("resolution");
        DPI = stringToDouble(resolution.getChildElement("dpi").getElementText());
        ppmm = stringToDouble(resolution.getChildElement("ppmm").getElementText());
        pixel_size = stringToDouble(resolution.getChildElement("pixel_size").getElementText());
        pixel_size_unit = resolution.getChildElement("pixel_size").getAttribute("unit");
    }

    Element ElementArea::toElement() const
    {
        Element e = *this;
        // store top, left, height and width
        e.getChildElement("top").setElementText(doubleToString(top));
        e.getChildElement("left").setElementText(doubleToString(left));
        e.getChildElement("height").setElementText(doubleToString(height));
        e.getChildElement("width").setElementText(doubleToString(width));
        // store whole height and width
        if (whole_height > 0 && whole_width > 0) {
            Element & whole = e.getChildElement("whole");
            whole.setAttribute("height", doubleToString(whole_height));
            whole.setAttribute("width", doubleToString(whole_width));
        }
        else {
            e.removeChildElement("whole");
        }
        // set units
        e.setAttribute("units", units);
        // store resolution
        Element & resolution = e.getChildElement("resolution");
        if (DPI > 0) resolution.getChildElement("dpi").setElementText(doubleToString(DPI));
        else resolution.removeChildElement("dpi");
        if (ppmm > 0) resolution.getChildElement("ppmm").setElementText(doubleToString(ppmm));
        else resolution.removeChildElement("ppmm");
        if (pixel_size > 0) {
            resolution.getChildElement("pixel_size").setElementText(doubleToString(pixel_size));
            resolution.getChildElement("pixel_size").setAttribute("unit", pixel_size_unit);
        }
        else resolution.removeChildElement("pixel_size");
        // return final element
        return e;
    }

    void ElementArea::setArea(double top, double left, double height, double width, double whole_height, double whole_width, const std::string & units)
    {
        this->top = top;
        this->left = left;
        this->height = height;
        this->width = width;
        this->whole_height = whole_height;
        this->whole_width = whole_width;
        this->units = units;
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
