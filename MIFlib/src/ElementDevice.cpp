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
* Version History of class ElementDevice:
*  1.0              First stable version.
*
* TODO List of class ElementDevice:
*   - implement sources and sensors methods
*   - implement unit tests
*   - add noexcept specifiers
*/

#include "../include/ElementDevice.hpp"

namespace mif
{
    ElementDevice::ElementDevice(const std::string & id, const std::string & type, const std::string & name, const std::string & serial, const std::string & description, const Attributes & attributes, const elements_t & elements)
        : Element("device", elements, attributes)
    {
        setId(id);
        setType(type);
        setName(name);
        setSerial(serial);
        setDescription(description);
    }

    ElementDevice::ElementDevice(const Element & element) : Element(element)
    {
        if (element.getElementName() != "device") clear();
    }

    //void BTFtree::addDeviceSource(const std::string & device_id, const std::string & id, const std::string & type, const attributes_t & attributes, const elements_t & elements)
    //{
    //    // check that device exists
    //    Element & device = getChildElement({ { "device", device_id } });
    //    if (device.isEmpty()) return;

    //    // create element
    //    Element e("source", elements);
    //    e.pushAttribute({ "id", id });
    //    e.pushAttribute({ "type", type });
    //    e.pushAttributes(attributes);

    //    // check that illumination-sources element exists
    //    Element & sources = device.getChildElement("illumination-sources");
    //    if (sources.isEmpty()) device.pushElement(Element("illumination-sources", { e }));
    //    else sources.pushElement(e);
    //}

    //void BTFtree::addDeviceSensor(const std::string & device_id, const std::string & id, const std::string & type, const attributes_t & attributes, const elements_t & elements)
    //{
    //    // check that device exists
    //    Element & device = getChildElement({ { "device", device_id } });
    //    if (device.isEmpty()) return;

    //    // create element
    //    Element e("sensor", elements);
    //    e.pushAttribute({ "id", id });
    //    e.pushAttribute({ "type", type });
    //    e.pushAttributes(attributes);

    //    // check that illumination-sources element exists
    //    Element & sensors = device.getChildElement("sensors");
    //    if (sensors.isEmpty()) device.pushElement(Element("sensors", { e }));
    //    else sensors.pushElement(e);
    //}

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
