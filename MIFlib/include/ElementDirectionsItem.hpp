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
#include "ElementDirection.hpp"

namespace mif
{
    class ElementDirectionsItem : public Element
    {
    public: // Types section.
        typedef Element::elements_t elements_t;

    public: // Constructors section.

        // Empty constructor.
        ElementDirectionsItem() : Element("directions-item") {}

        // Fill constructor.
        ElementDirectionsItem(const index_t & index, const std::vector<ElementDirection> & directions, const Attributes & attributes = {}, const elements_t & elements = {});

        // Converting constructor. Converts Element into ElementDirectionsItem.
        explicit ElementDirectionsItem(const Element & element, const std::string & default_units = "deg");

    public: // Conversions sections.

        // Converts ElementDirectionsItem into Element.
        Element toElement(bool addUnits = true) const;

    public: // Set property section.

        // Sets direction.
        void setDirection(const ElementDirection & direction);

    public: // Get property section.

        // Returns true if direction with given index is present.
        bool isDirection(const index_t & index) const;

        // Returns direction with given index. Creates new direction if nothing found.
        ElementDirection & getDirection(const index_t & index);

        // Returns direction with given index. Returns empty direction if nothing found.
        const ElementDirection & getDirection(const index_t & index) const;

        // Returns true if units of all underlying directions are the same.
        bool sameUnits() const;

        // Returns units of underlying directions.
        std::string getUnits() const;

    private:
        std::vector<ElementDirection> directions;
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
