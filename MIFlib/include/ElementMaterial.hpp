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
    class ElementMaterial : public Element
    {
    public: // Types section.
        typedef Element::elements_t elements_t;

    public: // Constructors section.

        // Empty constructor.
        ElementMaterial() : Element("material") {}

        // Fill constructor.
        explicit ElementMaterial(const std::string & id, const std::string & name = {}, const std::string & reference = {}, const std::string & description = {}, const Attributes & attributes = {}, const elements_t & elements = {});

        // Converting constructor. Converts Element into ElementMaterial.
        explicit ElementMaterial(const Element & element);

    public: // Conversions sections.

        // Converts ElementMaterial into Element.
        Element toElement() const { return *this; }

    public: // Set property section.

        // Sets name.
        void setName(const std::string & name) { getChildElement("name").setElementText(name); }

        // Sets reference.
        void setReference(const std::string & reference) { getChildElement("reference").setElementText(reference); }

        // Sets description.
        void setDescription(const std::string & description) { getChildElement("description").setElementText(description); }

    public: // Get property section.

        // Returns name.
        std::string & getName() { return getChildElement("name").getElementText(); }

        // Returns name.
        const std::string & getName() const { return getChildElement("name").getElementText(); }

        // Returns reference.
        std::string & getReference() { return getChildElement("reference").getElementText(); }

        // Returns reference.
        const std::string & getReference() const { return getChildElement("reference").getElementText(); }

        // Returns description.
        std::string & getDescription() { return getChildElement("description").getElementText(); }

        // Returns description.
        const std::string & getDescription() const { return getChildElement("description").getElementText(); }
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
