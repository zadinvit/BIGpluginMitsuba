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
    class ElementTimestamps : public Element
    {
    public: // Types section.
        typedef Element::elements_t elements_t;

    public: // Constructors section.

        // Empty constructor.
        ElementTimestamps() : Element("timestamps") {}

        // Converting constructor. Converts Element into ElementTimestamps.
        explicit ElementTimestamps(const Element & element);

    public: // Conversions sections.

        // Converts ElementTimestamps into Element.
        Element toElement() const { return *this; }

    public: // Set property section.

        // Sets timestamp with given label.
        void setTimestamp(const std::string & label, const std::string & datetime);

    public: // Get property section.

        // Returns timestamp with given label. Creates new timestamp if nothing found.
        std::string & getTimestamp(const std::string & label);

        // Returns timestamp with given label. Returns empty string if nothing found.
        const std::string & getTimestamp(const std::string & label) const;

        // Returns number of present timestamps.
        size_t getTimestampsCount() const;

        // Returns all present timestamps labels.
        std::vector<std::string> getTimestampsLabels() const;
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
