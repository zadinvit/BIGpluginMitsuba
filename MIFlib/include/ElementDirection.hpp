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
    enum class DirectionType { source, sensor, half_angle, diff_angle };

    DirectionType stringToDirectionType(const std::string & str);
    std::string directionTypeToString(DirectionType type);

    class ElementDirection : public Element
    {
    public: // Types section.
        typedef Element::elements_t elements_t;

    public: // Constructors section.

        // Empty constructor.
        ElementDirection() : Element("direction") {}

        // Fill constructor.
        ElementDirection(const index_t & index, DirectionType type, double theta, double phi, const std::string & units = "deg", const std::string & arm_reference_id = {});

        // Fill constructor.
        ElementDirection(const index_t & index, const std::string & type, double theta, double phi, const std::string & units = "deg", const std::string & arm_reference_id = {});

        // Converting constructor. Converts Element into ElementDirection.
        explicit ElementDirection(const Element & element, const std::string & default_units = "deg");

    public: // Conversions sections.

        // Converts ElementDirection into Element.
        Element toElement(bool addUnits = true) const;

    public: // Set property section.

        // Sets direction type, theta, phi, units and reference to arm of a device.
        void setDirection(const index_t & index, DirectionType type, double theta, double phi, const std::string & units = "deg", const std::string & arm_reference_id = {});

        // Sets direction type, theta, phi, units and reference to arm of a device.
        void setDirection(const index_t & index, const std::string & type, double theta, double phi, const std::string & units = "deg", const std::string & arm_reference_id = {});

        // Sets direction type.
        void setType(DirectionType type) { this->type = type; }

        // Sets direction type.
        void setType(const std::string & type) { this->type = stringToDirectionType(type); }

        // Sets theta.
        void setTheta(double theta) { this->theta = theta; }

        // Sets phi.
        void setPhi(double phi) { this->phi = phi; }

        // Sets units.
        void setUnits(const std::string & units) { this->units = units; }

        // Sets reference to arm of a device.
        void setReference(const std::string & arm_reference_id) { reference = arm_reference_id; }

    public: // Get property section.

        // Returns direction type.
        DirectionType & getType() { return type; }

        // Returns direction type.
        const DirectionType & getType() const { return type; }

        // Returns theta.
        double & getTheta() { return theta; }

        // Returns theta.
        const double & getTheta() const { return theta; }

        // Returns phi.
        double & getPhi() { return phi; }

        // Returns phi.
        const double & getPhi() const { return phi; }

        // Returns units.
        std::string & getUnits() { return units; }

        // Returns units.
        const std::string & getUnits() const { return units; }

        // Returns reference to arm.
        std::string & getReference() { return reference; }

        // Returns reference to arm of a device.
        const std::string & getReference() const { return reference; }

    private:
        DirectionType type;
        double theta = 0;
        double phi = 0;
        std::string units = "deg";
        std::string reference;
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
