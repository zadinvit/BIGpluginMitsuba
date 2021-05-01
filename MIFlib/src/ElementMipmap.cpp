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
* Version History of class ElementMipmap:
*  1.0              First stable version.
*
* TODO List of class ElementMipmap:
*   - implement resize image without loosing data (it should work like vector resize)
*   - implement unit tests
*   - add noexcept specifiers
*/

#include "../include/ElementMipmap.hpp"

namespace mif
{
    ElementMipmap::ElementMipmap(const std::vector<ElementMipmapItem> & items, const Attributes & attributes, const elements_t & elements)
        : Element("mipmap", elements, attributes), isotropic(true), isotropic_items(items) {}

    ElementMipmap::ElementMipmap(const image::Image<ElementMipmapItem> & items, const Attributes & attributes, const elements_t & elements)
        : Element("mipmap", elements, attributes), isotropic(false), anisotropic_items(items) {}

    ElementMipmap::ElementMipmap(const Element & element) : Element(element)
    {
        if (element.getElementName() != "mipmap" || !element.isAttribute("type")) {
            clear();
            return;
        }
        if (getAttribute("type") == "isotropic") {
            isotropic = true;
            for (const Element & item : element.getChildElements()) {
                if (item.getElementName() == "item") {
                    if (!item.isAttribute("level")) continue;
                    size_t level = stringToSize(item.getAttribute("level"));
                    if (level >= isotropic_items.size()) isotropic_items.resize(level + 1);
                    isotropic_items[level] = ElementMipmapItem(item);
                }
            }
        }
        else {
            isotropic = false;
            // find max level
            size_t n_level_x = 0;
            size_t n_level_y = 0;
            for (const Element & item : element.getChildElements()) {
                if (item.getElementName() == "item") {
                    if (!item.isAttribute("level-x") || !item.isAttribute("level-y")) continue;
                    size_t level_x = stringToSize(item.getAttribute("level-x"));
                    if (level_x >= n_level_x) n_level_x = level_x + 1;
                    size_t level_y = stringToSize(item.getAttribute("level-y"));
                    if (level_y >= n_level_y) n_level_y = level_y + 1;
                }
            }
            // store items
            anisotropic_items.resize(n_level_y, n_level_x);
            for (const Element & item : element.getChildElements()) {
                if (item.getElementName() == "item") {
                    if (!item.isAttribute("level-x") || !item.isAttribute("level-y")) continue;
                    size_t level_x = stringToSize(item.getAttribute("level-x"));
                    size_t level_y = stringToSize(item.getAttribute("level-y"));
                    anisotropic_items(level_y, level_x) = ElementMipmapItem(item);
                }
            }
        }
    }

    Element ElementMipmap::toElement() const
    {
        Element e = *this;
        if (isotropic) {
            e.setAttribute("type", "isotropic");
            for (const auto & item : isotropic_items) {
                e.pushChildElement(item.toElement());
            }
        }
        else {
            e.setAttribute("type", "anisotropic");
            for (size_t level_y = 0; level_y != anisotropic_items.height(); ++level_y) {
                for (size_t level_x = 0; level_x != anisotropic_items.width(); ++level_x) {
                    e.pushChildElement(anisotropic_items(level_y, level_x).toElement());
                }
            }
        }
        return e;
    }

    void ElementMipmap::setItem(const ElementMipmapItem & item)
    {
        if (item.isIsotropic() && !isotropic) return;
        if (isotropic) {
            size_t level = item.getLevel();
            if (level >= isotropic_items.size()) isotropic_items.resize(level + 1);
            isotropic_items[level] = item;
        }
        else {
            size_t level_x = item.getLevelX();
            size_t level_y = item.getLevelY();
            if (level_x >= anisotropic_items.width() || level_y >= anisotropic_items.height()) {
                // todo: resize anisotropic_items and do not lose data
                throw "not implemented yet";
            }
            anisotropic_items(level_y, level_x) = item;
        }
    }

    ElementMipmapItem & ElementMipmap::getItem(size_t level)
    {
        if (level >= isotropic_items.size()) isotropic_items.resize(level + 1);
        return isotropic_items[level];
    }

    ElementMipmapItem & ElementMipmap::getItem(size_t level_x, size_t level_y)
    {
        if (level_x >= anisotropic_items.width() || level_y >= anisotropic_items.height()) {
            // todo: resize anisotropic_items and do not lose data
            throw "not implemented yet";
        }
        return anisotropic_items.at(level_y, level_x);
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
