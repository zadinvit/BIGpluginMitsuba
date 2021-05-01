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
    class ElementDevice : public Element
    {
    public: // Types section.
        typedef Element::elements_t elements_t;

    public: // Constructors section.

        // Empty constructor.
        ElementDevice() : Element("device") {}

        // Fill constructor.
        explicit ElementDevice(const std::string & id, const std::string & type = "goniometer", const std::string & name = {}, const std::string & serial = {}, const std::string & description = {}, const Attributes & attributes = {}, const elements_t & elements = {});

        // Converting constructor. Converts Element into ElementDevice.
        explicit ElementDevice(const Element & element);

    public: // Conversions sections.

        // Converts ElementDevice into Element.
        Element toElement() const { return *this; }

    public: // Set property section.

        // Sets type.
        void setType(const std::string & type) { setAttribute("type", type); }

        // Sets name.
        void setName(const std::string & name) { getChildElement("name").setElementText(name); }

        // Sets serial.
        void setSerial(const std::string & serial) { getChildElement("serial").setElementText(serial); }

        // Sets description.
        void setDescription(const std::string & description) { getChildElement("description").setElementText(description); }

        // todo: Sets illumination source element inside illumination-sources element. The source element has specified id, type, attributes and child elements.
        // void setSource(const std::string & id, const std::string & type = {}, const Attributes & attributes = {}, const elements_t & elements = {}) {}

        // todo: Sets sensor element inside sensors element. The sensor element has specified id, type, attributes and child elements.
        // void setSensor(const std::string & id, const std::string & type = {}, const Attributes & attributes = {}, const elements_t & elements = {}) {}

    public: // Get property section.

        // Returns type.
        const std::string & getType() const { return getAttribute("type"); }

        // Returns name.
        std::string & getName() { return getChildElement("name").getElementText(); }

        // Returns name.
        const std::string & getName() const { return getChildElement("name").getElementText(); }

        // Returns serial.
        std::string & getSerial() { return getChildElement("serial").getElementText(); }

        // Returns serial.
        const std::string & getSerial() const { return getChildElement("serial").getElementText(); }

        // Returns description.
        std::string & getDescription() { return getChildElement("description").getElementText(); }

        // Returns description.
        const std::string & getDescription() const { return getChildElement("description").getElementText(); }

        //// todo: Returns IDs of all illumination sources.
        //std::vector<std::string> getSourcesIDs() const { return std::vector<std::string>(); }

        //// todo: Returns IDs of all sensor.
        //std::vector<std::string> getSensorsIDs() const { return std::vector<std::string>(); }

        //// todo: Returns illumination source with given id.
        //Element & getSource(const std::string & id) { static Element empty;  return empty; }

        //// todo: Returns illumination source with given id.
        //const Element & getSource(const std::string & id) const { static Element empty;  return empty; }

        //// todo: Returns sensor with given id.
        //Element & getSensor(const std::string & id) { static Element empty;  return empty; }

        //// todo: Returns sensor with given id.
        //const Element & getSensor(const std::string & id) const { static Element empty;  return empty; }
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
