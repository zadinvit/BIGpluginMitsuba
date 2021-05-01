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
    class ElementColors : public Element
    {
    public: // Types section.
        typedef Element::elements_t elements_t;

    public: // Constructors section.

        // Empty constructor.
        ElementColors() : Element("colors") {}

        // Fill constructor.
        explicit ElementColors(const std::string & id, const std::string & color_space = "XYZ", const std::string & illuminant = "D65", const Attributes & attributes = {}, const elements_t & elements = {});

        // Converting constructor. Converts Element into ElementColors.
        explicit ElementColors(const Element & element);

    public: // Conversions sections.

        // Converts ElementColors into Element.
        Element toElement() const { return *this; }

    public: // Set property section.

        // Sets color space.
        void setColorSpace(const std::string & color_space) { getChildElement("color_space").setElementText(color_space); }

        // Sets illuminant.
        void setIlluminant(const std::string & illuminant) { getChildElement("illuminant").setElementText(illuminant); }

        // Sets observer.
        void setObserver(double value, const std::string & unit = "deg") { Element & observer = getChildElement("observer"); observer.setElementText(doubleToString(value)); observer.setAttribute("unit", unit); }

        // Sets white point attributes.
        void setWhitePoint(const Attributes & attributes) { getChildElement("white_point").setAttributes(attributes); }

        // Sets label of color channel with given index.
        void setColorChannel(const index_t & index, const std::string & label) { Element & channels = getChildElement("channels"); channels.getChildElement("channel", index).setAttribute("label", label); }

    public: // Get property section.

        // Returns color space.
        std::string & getColorSpace() { return getChildElement("color_space").getElementText(); }

        // Returns color space.
        const std::string & getColorSpace() const { return getChildElement("color_space").getElementText(); }

        // Returns illuminant.
        std::string & getIlluminant() { return getChildElement("illuminant").getElementText(); }

        // Returns illuminant.
        const std::string & getIlluminant() const { return getChildElement("illuminant").getElementText(); }

        // Returns observer.
        double getObserver() const { return stringToDouble(getChildElement("observer").getElementText()); }

        // Returns unit of observer.
        const std::string & getObserverUnit() const { return getChildElement("observer").getAttribute("unit"); }

        // Returns white point attributes.
        const Attributes & getWhitePoint() const { return getChildElement("white_point").getAttributes(); }

        // Returns color channel with given index.
        const std::string & getColorChannel(const index_t & index) const { return getChildElement("channels").getChildElement("channel", index).getAttribute("label"); }

        // Returns number of present color channels.
        size_t getColorChannelsCount() const;

        // Returns indices of present color channels.
        std::vector<index_t> getColorChannelsIndices() const;

        // Returns all present color channels.
        std::vector<std::string> getColorChannels() const;
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
