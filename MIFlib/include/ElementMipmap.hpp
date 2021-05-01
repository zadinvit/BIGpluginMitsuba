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
#include "ElementMipmapItem.hpp"
#include "ext/Image.hpp"

namespace mif
{
    class ElementMipmap : public Element
    {
    public: // Types section.
        typedef Element::elements_t elements_t;

    public: // Constructors section.

        // Empty constructor.
        ElementMipmap() : Element("mipmap") {}

        // Isotropic mipmap constructor.
        explicit ElementMipmap(const std::vector<ElementMipmapItem> & items, const Attributes & attributes = {}, const elements_t & elements = {});

        // Anisotropic mipmap constructor.
        explicit ElementMipmap(const image::Image<ElementMipmapItem> & items, const Attributes & attributes = {}, const elements_t & elements = {});

        // Converting constructor. Converts Element into ElementMipmap.
        explicit ElementMipmap(const Element & element);

    public: // Conversions sections.

        // Converts ElementMipmap into Element.
        Element toElement() const;

    public: // Set property section.

        // Sets isotropic mipmap.
        void setItems(const std::vector<ElementMipmapItem> & items) { isotropic = true; isotropic_items = items; }

        // Sets anisotropic mipmap.
        void setItems(const image::Image<ElementMipmapItem> & items) { isotropic = false; anisotropic_items = items; }

        // Sets mipmap item.
        void setItem(const ElementMipmapItem & item);

    public: // Get property section.

        // Returns true if this is isotropic mipmap.
        bool isIsotropic() const { return isotropic; }

        // Returns isotropic mipmap item.
        ElementMipmapItem & getItem(size_t level);

        // Returns isotropic mipmap item.
        const ElementMipmapItem & getItem(size_t level) const { return isotropic_items.at(level); }

        // Returns anisotropic mipmap item.
        ElementMipmapItem & getItem(size_t level_x, size_t level_y);

        // Returns anisotropic mipmap item.
        const ElementMipmapItem & getItem(size_t level_x, size_t level_y) const { return anisotropic_items.at(level_y, level_x); }

        // Returns isotropic mipmap items.
        const std::vector<ElementMipmapItem> & getIsotropicItems() const { return isotropic_items; }

        // Returns anisotropic mipmap items.
        const image::Image<ElementMipmapItem> & getAnisotropicItems() const { return anisotropic_items; }

    private:
        bool isotropic = true;
        std::vector<ElementMipmapItem> isotropic_items;
        image::Image<ElementMipmapItem> anisotropic_items;
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
