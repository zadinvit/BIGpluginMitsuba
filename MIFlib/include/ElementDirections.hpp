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
#include "ElementDirectionsItem.hpp"

namespace mif
{
    class ElementDirections : public Element
    {
    public: // Types section.
        typedef Element::elements_t elements_t;

    public: // Constructors section.

        // Empty constructor.
        ElementDirections() : Element("directions") { setAttribute("type", "list"); }

        // Fill constructor.
        ElementDirections(const std::string & id, const std::vector<ElementDirectionsItem> & items, const Attributes & attributes = {}, const elements_t & elements = {});

        // Converting constructor. Converts Element into ElementDirections.
        explicit ElementDirections(const Element & element, const std::string & default_units = "deg");

    public: // Conversions sections.

        // Converts ElementDirections into Element.
        Element toElement(bool reference_only = false) const;

    public: // Set property section.

            // Sets name to Uniform and sets its parameters stepTheta and stepPhi.
        void setNameUniform(double stepTheta, double stepPhi) { clearAngles(); setName("Uniform"); setStepTheta(stepTheta); setStepPhi(stepPhi); }

        // Sets name to UBO81x81.
        void setNameUBO81x81() { clearAngles(); setName("UBO81x81"); }

        // Sets name to CoatingRegular and sets its parameters stepThetaH and stepThetaD.
        void setNameCoatingRegular(double stepThetaH, double stepThetaD) { clearAngles(); setName("CoatingRegular"); setStepThetaH(stepThetaH); setStepThetaD(stepThetaD); }

        // Sets name to CoatingSpecial and sets its parameters stepThetaH and stepPhiH.
        void setNameCoatingSpecial(double stepThetaH, double stepPhiH) { clearAngles(); setName("CoatingSpecial"); setStepThetaH(stepThetaH); setStepPhiH(stepPhiH); }

        // Sets name.
        void setName(const std::string & name) { setAttribute("name", name); }

        // Sets step theta.
        void setStepTheta(double value) { setAttribute("stepTheta", doubleToString(value)); }

        // Sets step theta half-vector.
        void setStepThetaH(double value) { setAttribute("stepThetaH", doubleToString(value)); }

        // Sets step theta difference-vector.
        void setStepThetaD(double value) { setAttribute("stepThetaD", doubleToString(value)); }

        // Sets step phi.
        void setStepPhi(double value) { setAttribute("stepPhi", doubleToString(value)); }

        // Sets step phi half-vector.
        void setStepPhiH(double value) { setAttribute("stepPhiH", doubleToString(value)); }

        // Sets step phi difference-vector.
        void setStepPhiD(double value) { setAttribute("stepPhiD", doubleToString(value)); }

        // Removes all angles (step theta / phi).
        void clearAngles() { removeAttribute("stepTheta"); removeAttribute("stepThetaH"); removeAttribute("stepThetaD"); removeAttribute("stepPhi"); removeAttribute("stepPhiH"); removeAttribute("stepPhiD"); }

        // Sets one directions item.
        void setDirectionsItem(const ElementDirectionsItem & item);

    public: // Get property section.

        // Returns true if name is Uniform.
        bool isNameUniform() const { return getAttribute("name") == "Uniform"; }

        // Returns true if name is UBO81x81.
        bool isNameUBO81x81() const { return getAttribute("name") == "UBO81x81"; }

        // Returns true if name is CoatingRegular.
        bool isNameCoatingRegular() const { return getAttribute("name") == "CoatingRegular"; }

        // Returns true if name is CoatingSpecial.
        bool isNameCoatingSpecial() const { return getAttribute("name") == "CoatingSpecial"; }

        // Returns name.
        const std::string & getName() const { return getAttribute("name"); }

        // Returns step theta.
        double getStepTheta() const { return stringToDouble(getAttribute("stepTheta")); }

        // Returns step theta half-vector.
        double getStepThetaH() const { return stringToDouble(getAttribute("stepThetaH")); }

        // Returns step theta difference-vector.
        double getStepThetaD() const { return stringToDouble(getAttribute("stepThetaD")); }

        // Returns step phi.
        double getStepPhi() const { return stringToDouble(getAttribute("stepPhi")); }

        // Returns step phi half-vector.
        double getStepPhiH() const { return stringToDouble(getAttribute("stepPhiH")); }

        // Returns step phi difference-vector.
        double getStepPhiD() const { return stringToDouble(getAttribute("stepPhiD")); }

        // Returns true if directions item with given index is present.
        bool isDirectionsItem(const index_t & index) const;

        // Returns directions item with given index. Creates new directions item if nothing found.
        ElementDirectionsItem & getDirectionsItem(const index_t & index);

        // Returns directions item with given index. Returns empty directions item if nothing found.
        const ElementDirectionsItem & getDirectionsItem(const index_t & index) const;

        // Returns all directions;
        std::vector<ElementDirectionsItem> & getDirections() { return items; }

        // Returns all directions;
        const std::vector<ElementDirectionsItem> & getDirections() const { return items; }

        // Returns number of present directions items.
        size_t getDirectionsCount() const { return items.size(); }

        // Returns true if units of all underlying directions items are the same.
        bool sameUnits() const;

        // Returns units of underlying directions items.
        std::string getUnits() const;

    private:
        std::vector<ElementDirectionsItem> items;
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
