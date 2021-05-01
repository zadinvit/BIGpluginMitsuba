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
* Version History of class ElementDirection:
*  1.0              First stable version.
*
* TODO List of class ElementDirection:
*   - implement unit tests
*   - add noexcept specifiers
*/

#include "../include/ElementDirection.hpp"

namespace mif
{
    DirectionType stringToDirectionType(const std::string & str)
    {
        if (str == "source") return DirectionType::source;
        else if (str == "sensor") return DirectionType::sensor;
        else if (str == "half-angle") return DirectionType::half_angle;
        else if (str == "difference-angle") return DirectionType::diff_angle;
        else if (str == "diff-angle") return DirectionType::diff_angle;
        return DirectionType::source;
    }

    std::string directionTypeToString(DirectionType type)
    {
        switch (type) {
        case DirectionType::source: return "source";
        case DirectionType::sensor: return "sensor";
        case DirectionType::half_angle: return "half-angle";
        case DirectionType::diff_angle: return "difference-angle";
        }
        return "";
    }

    ElementDirection::ElementDirection(const index_t & index, DirectionType type, double theta, double phi, const std::string & units, const std::string & arm_reference_id)
        : Element("direction"), type(type), theta(theta), phi(phi), units(units), reference(arm_reference_id)
    {
        setIndex(index);
    }

    ElementDirection::ElementDirection(const index_t & index, const std::string & type, double theta, double phi, const std::string & units, const std::string & arm_reference_id)
        : Element("direction"), type(stringToDirectionType(type)), theta(theta), phi(phi), units(units), reference(arm_reference_id)
    {
        setIndex(index);
    }

    ElementDirection::ElementDirection(const Element & element, const std::string & default_units)
        : Element(element)
    {
        if (element.getElementName() != "direction") {
            clear();
            return;
        }
        type = stringToDirectionType(element.getAttribute("type"));
        theta = stringToDouble(element.getAttribute("theta"));
        phi = stringToDouble(element.getAttribute("phi"));
        if (element.isAttribute("units")) units = element.getAttribute("units");
        else units = default_units;
        reference = element.getAttribute("reference");
    }

    Element ElementDirection::toElement(bool addUnits) const
    {
        Element e = *this;
        e.setAttribute("type", directionTypeToString(type));
        e.setAttribute("theta", doubleToString(theta));
        e.setAttribute("phi", doubleToString(phi));
        if (addUnits) e.setAttribute("units", units);
        if (!reference.empty()) e.setAttribute("reference", reference);
        return e;
    }

    void ElementDirection::setDirection(const index_t & index, DirectionType type, double theta, double phi, const std::string & units, const std::string & arm_reference_id)
    {
        setIndex(index);
        this->type = type;
        this->theta = theta;
        this->phi = phi;
        this->units = units;
        reference = arm_reference_id;
    }

    void ElementDirection::setDirection(const index_t & index, const std::string & type, double theta, double phi, const std::string & units, const std::string & arm_reference_id)
    {
        setIndex(index);
        this->type = stringToDirectionType(type);
        this->theta = theta;
        this->phi = phi;
        this->units = units;
        reference = arm_reference_id;
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
