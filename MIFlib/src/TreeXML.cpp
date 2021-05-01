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
* Version History of class TreeXML:
*  0.1              Empty version.
*  1.0              First stable version implemented by Vit Zadina.
*  1.1              Polished by Radomir Vavra.
*  1.2              Fixed bug with unnamed child in readElement by Radomir Vavra.
*
* TODO List of class TreeXML:
*   - implement unit tests.
*/

#include "../include/TreeXML.hpp"
#include "../include/ext/pugixml.hpp"

namespace mif
{
    Element readElement(const pugi::xml_node & node)
    {
        // make new element
        Element e;
        // set its name
        e.setElementName(node.name());
        // set its text
        e.setElementText(node.text().as_string());
        // set its attributes
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            e.setAttribute(attr.name(), attr.value());
        }
        // read child elements
        for (const auto & child : node) {
            if (child.name() != "") e.pushChildElement(readElement(child));
        }
        return e;
    }

    Element TreeXML::xmlStringToDataTree(const std::string & xml_string)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(xml_string.c_str());
        return readElement(doc.document_element());
    }

    // write xml to string
    struct xml_string_writer : pugi::xml_writer
    {
        std::string result;

        virtual void write(const void* data, size_t size)
        {
            result.append(static_cast<const char*>(data), size);
        }
    };

    void writeElement(pugi::xml_node & node, const Element & e)
    {
        auto child = node.append_child(e.getElementName().c_str());
        for (const auto & attribute : e.getAttributes()) {
            child.append_attribute(attribute.first.c_str()) = attribute.second.c_str();
        }
        if (!e.getElementText().empty()) child.append_child(pugi::node_pcdata).set_value(e.getElementText().c_str());
        for (const auto & element : e.getChildElements()) {
            writeElement(child, element);
        }
    }

    std::string TreeXML::dataTreeToXMLstring(const Element & root)
    {
        pugi::xml_document doc;
        auto declarationNode = doc.append_child(pugi::node_declaration);
        declarationNode.append_attribute("version") = "1.0";
        pugi::xml_node rnode = doc.append_child(root.getElementName().c_str());
        for (const auto & element : root.getChildElements()) {
            writeElement(rnode, element);
        }
        xml_string_writer writer;
        doc.print(writer, "    ");
        return writer.result;
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
