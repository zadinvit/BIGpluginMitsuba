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
    class ElementArea : public Element
    {
    public: // Types section.
        typedef Element::elements_t elements_t;

    public: // Constructors section.

        // Empty constructor.
        ElementArea() : Element("area") {}

        // Fill constructor.
        ElementArea(const std::string & id, double top, double left, double height, double width, double whole_height = 0, double whole_width = 0, const std::string & units = "px", const Attributes & attributes = {}, const elements_t & elements = {});

        // Converting constructor. Converts Element into ElementArea.
        explicit ElementArea(const Element & element);

    public: // Conversions sections.

        // Converts ElementArea into Element.
        Element toElement() const;

    public: // Set property section.

        // Sets area.
        void setArea(double top, double left, double height, double width, double whole_height = 0, double whole_width = 0, const std::string & units = "px");

        // Sets DPI.
        void setDPI(double DPI) { this->DPI = DPI; }

        // Sets pixels per millimeter.
        void setPixelsPerMillimeter(double ppmm) { this->ppmm = ppmm; }

        // Sets pixel size.
        void setPixelSize(double pixel_size, const std::string & unit = "mm") { this->pixel_size = pixel_size; pixel_size_unit = unit; }

    public: // Get property section.

        // Returns area top.
        double & getAreaTop() { return top; }

        // Returns area top.
        const double & getAreaTop() const { return top; }

        // Returns area left.
        double & getAreaLeft() { return left; }

        // Returns area left.
        const double & getAreaLeft() const { return left; }

        // Returns area height.
        double & getAreaHeight() { return height; }

        // Returns area height.
        const double & getAreaHeight() const { return height; }

        // Returns area width.
        double & getAreaWidth() { return width; }

        // Returns area width.
        const double & getAreaWidth() const { return width; }

        // Returns whole area height.
        double & getAreaWholeHeight() { return whole_height; }

        // Returns whole area height.
        const double & getAreaWholeHeight() const { return whole_height; }

        // Returns whole area width.
        double & getAreaWholeWidth() { return whole_width; }

        // Returns whole area width.
        const double & getAreaWholeWidth() const { return whole_width; }

        // Returns area units.
        std::string & getAreaUnits() { return units; }

        // Returns area units.
        const std::string & getAreaUnits() const { return units; }

        // Returns DPI.
        double & getDPI() { return DPI; }

        // Returns DPI.
        const double & getDPI() const { return DPI; }

        // Returns pixels per millimeter.
        double & getPixelsPerMillimeter() { return ppmm; }

        // Returns pixels per millimeter.
        const double & getPixelsPerMillimeter() const { return ppmm; }

        // Returns pixel size.
        double & getPixelSize() { return pixel_size; }

        // Returns pixel size.
        const double & getPixelSize() const { return pixel_size; }

        // Returns pixel size unit.
        std::string & getPixelSizeUnit() { return pixel_size_unit; }

        // Returns pixel size unit.
        const std::string & getPixelSizeUnit() const { return pixel_size_unit; }

    private:
        double top = 0;
        double left = 0;
        double height = 0;
        double width = 0;
        double whole_height = 0;
        double whole_width = 0;
        std::string units = "px";

        double DPI = 0;
        double ppmm = 0;
        double pixel_size = 0;
        std::string pixel_size_unit = "mm";
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
