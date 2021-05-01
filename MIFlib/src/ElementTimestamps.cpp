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
* Version History of class ElementTimestamps:
*  1.0              First stable version.
*
* TODO List of class ElementTimestamps:
*   - implement unit tests
*   - add noexcept specifiers
*/

#include "../include/ElementTimestamps.hpp"

namespace mif
{
    ElementTimestamps::ElementTimestamps(const Element & element) : Element(element)
    {
        if (element.getElementName() != "timestamps") clear();
    }

    void ElementTimestamps::setTimestamp(const std::string & label, const std::string & datetime)
    {
        for (auto & element : getChildElements()) {
            if (element.getElementName() == "datetime" && element.getAttribute("label") == label) {
                element.setElementText(datetime);
                return;
            }
        }
        pushChildElement(Element("datetime", datetime, Attributes("label", label)));
    }

    std::string & ElementTimestamps::getTimestamp(const std::string & label)
    {
        for (auto & element : getChildElements()) {
            if (element.getElementName() == "datetime" && element.getAttribute("label") == label) {
                return element.getElementText();
            }
        }
        return pushChildElement(Element("datetime", Attributes("label", label))).getElementText();
    }

    const std::string & ElementTimestamps::getTimestamp(const std::string & label) const
    {
        for (const auto & element : getChildElements()) {
            if (element.getElementName() == "datetime" && element.getAttribute("label") == label) {
                return element.getElementText();
            }
        }
        const static std::string empty;
        return empty;
    }

    size_t ElementTimestamps::getTimestampsCount() const
    {
        size_t count = 0;
        for (const auto & element : getChildElements()) {
            if (element.getElementName() == "datetime" && element.isAttribute("label")) ++count;
        }
        return count;
    }

    std::vector<std::string> ElementTimestamps::getTimestampsLabels() const
    {
        std::vector<std::string> labels;
        for (const auto & element : getChildElements()) {
            if (element.getElementName() == "datetime" && element.isAttribute("label")) labels.push_back(element.getAttribute("label"));
        }
        return labels;
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
