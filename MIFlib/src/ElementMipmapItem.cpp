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
* Version History of class ElementMipmapItem:
*  1.0              First stable version.
*  1.1              Fixed bug in attribute names (level_x, level_y -> level-x, level-y).
*
* TODO List of class ElementMipmapItem:
*   - implement unit tests
*   - add noexcept specifiers
*/

#include "../include/ElementMipmapItem.hpp"

namespace mif
{
    ElementMipmapItem::ElementMipmapItem(size_t level, size_t x, size_t y, size_t cols, size_t rows, const Attributes & attributes, const elements_t & elements)
        : Element("item", elements, attributes), isotropic(true), level(level), x(x), y(y), cols(cols), rows(rows) {}

    ElementMipmapItem::ElementMipmapItem(size_t level_x, size_t level_y, size_t x, size_t y, size_t cols, size_t rows, const Attributes & attributes, const elements_t & elements)
        : Element("item", elements, attributes), isotropic(false), level_x(level_x), level_y(level_y), x(x), y(y), cols(cols), rows(rows) {}

    ElementMipmapItem::ElementMipmapItem(const Element & element) : Element(element)
    {
        if (element.getElementName() != "item") {
            clear();
            return;
        }
        if (isAttribute("level")) {
            isotropic = true;
            level = stringToSize(getAttribute("level"));
        }
        else {
            isotropic = false;
            level_x = stringToSize(getAttribute("level-x"));
            level_y = stringToSize(getAttribute("level-y"));
        }
        x = stringToSize(getAttribute("x"));
        y = stringToSize(getAttribute("y"));
        cols = stringToSize(getAttribute("cols"));
        rows = stringToSize(getAttribute("rows"));
    }

    Element ElementMipmapItem::toElement() const
    {
        Element e = *this;
        if (isotropic) e.setAttribute("level", sizeToString(level));
        else {
            e.setAttribute("level-x", sizeToString(level_x));
            e.setAttribute("level-y", sizeToString(level_y));
        }
        e.setAttribute("x", sizeToString(x));
        e.setAttribute("y", sizeToString(y));
        e.setAttribute("cols", sizeToString(cols));
        e.setAttribute("rows", sizeToString(rows));
        return e;
    }

    void ElementMipmapItem::setItem(size_t level, size_t x, size_t y, size_t cols, size_t rows)
    {
        isotropic = true;
        this->level = level;
        this->x = x;
        this->y = y;
        this->cols = cols;
        this->rows = rows;
    }

    void ElementMipmapItem::setItem(size_t level_x, size_t level_y, size_t x, size_t y, size_t cols, size_t rows)
    {
        isotropic = false;
        this->level_x = level_x;
        this->level_y = level_y;
        this->x = x;
        this->y = y;
        this->cols = cols;
        this->rows = rows;
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
