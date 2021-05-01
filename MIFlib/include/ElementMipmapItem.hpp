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

namespace mif
{
    class ElementMipmapItem : public Element
    {
    public: // Types section.
        typedef Element::elements_t elements_t;

    public: // Constructors section.

        // Empty constructor.
        ElementMipmapItem() : Element("item") {}

        // Isotropic mipmap item constructor.
        ElementMipmapItem(size_t level, size_t x, size_t y, size_t cols, size_t rows, const Attributes & attributes = {}, const elements_t & elements = {});

        // Anisotropic mipmap item constructor.
        ElementMipmapItem(size_t level_x, size_t level_y, size_t x, size_t y, size_t cols, size_t rows, const Attributes & attributes = {}, const elements_t & elements = {});

        // Converting constructor. Converts Element into ElementMipmapItem.
        explicit ElementMipmapItem(const Element & element);

    public: // Conversions sections.

        // Converts ElementMipmapItem into Element.
        Element toElement() const;

    public: // Set property section.

        // Sets isotropic mipmap item.
        void setItem(size_t level, size_t x, size_t y, size_t cols, size_t rows);

        // Sets anisotropic mipmap item.
        void setItem(size_t level_x, size_t level_y, size_t x, size_t y, size_t cols, size_t rows);

    public: // Get property section.

        // Returns true if this is isotropic mipmap item.
        bool isIsotropic() const { return isotropic; }

        // Returns isotropic mipmap level.
        size_t & getLevel() { return level; }

        // Returns isotropic mipmap level.
        const size_t & getLevel() const { return level; }

        // Returns anisotropic mipmap level x.
        size_t & getLevelX() { return level_x; }

        // Returns anisotropic mipmap level x.
        const size_t & getLevelX() const { return level_x; }

        // Returns anisotropic mipmap level y.
        size_t & getLevelY() { return level_y; }

        // Returns anisotropic mipmap level y.
        const size_t & getLevelY() const { return level_y; }

        // Returns x.
        size_t & getX() { return x; }

        // Returns x.
        const size_t & getX() const { return x; }

        // Returns y.
        size_t & getY() { return y; }

        // Returns y.
        const size_t & getY() const { return y; }

        // Returns cols.
        size_t & getCols() { return cols; }

        // Returns cols.
        const size_t & getCols() const { return cols; }

        // Returns rows.
        size_t & getRows() { return rows; }

        // Returns rows.
        const size_t & getRows() const { return rows; }

    private:
        bool isotropic = true;
        size_t level = 0;
        size_t level_x = 0;
        size_t level_y = 0;

        size_t x = 0;
        size_t y = 0;
        size_t cols = 0;
        size_t rows = 0;
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
