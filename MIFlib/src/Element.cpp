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
* Version History of class Element:
*  1.0              First stable version.
*  1.1              Element name and text related methods renamed from setName, getName, setText, getText to setElementName, getElementName, setElementText, getElementText.
*  1.2              Child related methods renamed from *Element* to *ChildElement*.
*
* TODO List of class Element:
*   - implement unit tests
*   - add noexcept specifiers
*/

#include "../include/Element.hpp"

namespace mif
{
    Element::Element(const std::string & name, const Attributes & attributes)
        : _name(name), _attributes(attributes)
    {}

    Element::Element(const std::string & name, const std::string & text, const Attributes & attributes)
        : _name(name), _text(text), _attributes(attributes)
    {}

    Element::Element(const std::string & name, const elements_t & elements, const Attributes & attributes)
        : _name(name), _attributes(attributes), _elements(elements)
    {}

    Element::Element(const element_id_t & name_id)
        : _name(name_id.first), _attributes(name_id.second)
    {}

    Element::Element(const std::string & name, const index_t & index)
        : _name(name), _attributes(index)
    {}

    void Element::clear()
    {
        _name = {};
        _text = {};
        _attributes = {};
        _elements = {};
    }

    bool Element::isChildElement(const std::string & name, const path_t & path) const
    {
        return !getChildElement(name, path).isEmpty();
    }

    bool Element::isChildElement(const path_t & path) const
    {
        return !getChildElement(path).isEmpty();
    }

    bool Element::isChildElement(const std::string & name, const index_t & index, const path_t & path) const
    {
        return !getChildElement(name, index, path).isEmpty();
    }

    Element & Element::pushChildElement(const Element & element, const path_t & path)
    {
        Element & e = getChildElement(path);
        _elements.push_back(element);
        return _elements.back();
    }

    Element & Element::getChildElement(const std::string & name, const path_t & path)
    {
        Element & parent = getChildElement(path);
        for (auto & element : parent._elements) {
            if (element.getElementName() == name) return element;
        }
        return parent.pushChildElement(Element(name));
    }

    const Element & Element::getChildElement(const std::string & name, const path_t & path) const
    {
        const Element & parent = getChildElement(path);
        for (const auto & element : parent._elements) {
            if (element.getElementName() == name) return element;
        }
        static const Element empty;
        return empty;
    }

    Element & Element::getChildElement(const path_t & path)
    {
        Element * parent = this;
        for (auto & name_id : path) {
            bool found = false;
            for (auto & element : parent->_elements) {
                if (checkNameId(element, name_id)) {
                    found = true;
                    parent = &element;
                    break;
                }
            }
            if (!found) parent = &(parent->pushChildElement(Element(name_id)));
        }
        return *parent;
    }

    const Element & Element::getChildElement(const path_t & path) const
    {
        static const Element empty;
        const Element * parent = this;
        for (const auto & name_id : path) {
            bool found = false;
            for (const auto & element : parent->_elements) {
                if (checkNameId(element, name_id)) {
                    found = true;
                    parent = &element;
                    break;
                }
            }
            if (!found) return empty;
        }
        return *parent;
    }

    Element & Element::getChildElement(const std::string & name, const index_t & index, const path_t & path)
    {
        Element & parent = getChildElement(path);
        for (auto & element : parent._elements) {
            if (checkNameIndex(element, name, index)) return element;
        }
        return parent.pushChildElement(Element(name, index));
    }

    const Element & Element::getChildElement(const std::string & name, const index_t & index, const path_t & path) const
    {
        const Element & parent = getChildElement(path);
        for (const auto & element : parent._elements) {
            if (checkNameIndex(element, name, index)) return element;
        }
        static const Element empty;
        return empty;
    }

    void Element::removeChildElement(const std::string & name, const path_t & path)
    {
        Element & parent = getChildElement(path);
        for (auto it = parent._elements.begin(); it != parent._elements.end();) {
            if (it->getElementName() == name) it = _elements.erase(it);
            else ++it;
        }
    }

    void Element::removeChildElement(const path_t & path)
    {
        auto path2 = path;
        path2.pop_back();
        Element & parent = getChildElement(path2);
        for (auto it = parent._elements.begin(); it != parent._elements.end();) {
            if (checkNameId(*it, path.back())) it = _elements.erase(it);
            else ++it;
        }
    }

    void Element::removeChildElement(const std::string & name, const index_t & index, const path_t & path)
    {
        Element & parent = getChildElement(path);
        for (auto it = parent._elements.begin(); it != parent._elements.end();) {
            if (checkNameIndex(*it, name, index)) it = _elements.erase(it);
            else ++it;
        }
    }

    void Element::pushChildElements(const elements_t & elements, const path_t & path)
    {
        Element & parent = getChildElement(path);
        if (!parent.isEmpty()) parent._elements.insert(parent._elements.end(), elements.begin(), elements.end());
    }

    Element::elements_t & Element::getChildElements(const path_t & path)
    {
        return getChildElement(path)._elements;
    }

    const Element::elements_t & Element::getChildElements(const path_t & path) const
    {
        return getChildElement(path)._elements;
    }

    std::vector<Element *> Element::getChildElements(const std::string & name, const path_t & path)
    {
        std::vector<Element *> elements;
        Element & parent = getChildElement(path);
        for (auto & element : parent._elements) {
            if (element.getElementName() == name) elements.push_back(&element);
        }
        return elements;
    }

    std::vector<const Element *> Element::getChildElements(const std::string & name, const path_t & path) const
    {
        std::vector<const Element *> elements;
        const Element & parent = getChildElement(path);
        for (const auto & element : parent._elements) {
            if (element.getElementName() == name) elements.push_back(&element);
        }
        return elements;
    }

    std::vector<Element *> Element::getChildElements(const element_id_t & name_id, const path_t & path)
    {
        std::vector<Element *> elements;
        Element & parent = getChildElement(path);
        for (auto & element : parent._elements) {
            if (checkNameId(element, name_id)) elements.push_back(&element);
        }
        return elements;
    }

    std::vector<const Element *> Element::getChildElements(const element_id_t & name_id, const path_t & path) const
    {
        std::vector<const Element *> elements;
        const Element & parent = getChildElement(path);
        for (const auto & element : parent._elements) {
            if (checkNameId(element, name_id)) elements.push_back(&element);
        }
        return elements;
    }

    std::vector<Element *> Element::getChildElements(const std::string & name, const index_t & index, const path_t & path)
    {
        std::vector<Element *> elements;
        Element & parent = getChildElement(path);
        for (auto & element : parent._elements) {
            if (checkNameIndex(element, name, index)) elements.push_back(&element);
        }
        return elements;
    }

    std::vector<const Element *> Element::getChildElements(const std::string & name, const index_t & index, const path_t & path) const
    {
        std::vector<const Element *> elements;
        const Element & parent = getChildElement(path);
        for (const auto & element : parent._elements) {
            if (checkNameIndex(element, name, index)) elements.push_back(&element);
        }
        return elements;
    }

    std::vector<std::string> Element::getChildElementsIDs(const std::string & name, const path_t & path) const
    {
        std::vector<std::string> ids;
        const Element & parent = getChildElement(path);
        for (const auto & element : parent._elements) {
            if (element.getElementName() == name && element.isId()) ids.push_back(element.getId());
        }
        return ids;
    }

    std::vector<Element::index_t> Element::getChildElementsIndices(const std::string & name, const path_t & path) const
    {
        std::vector<index_t> indices;
        const Element & parent = getChildElement(path);
        for (const auto & element : parent._elements) {
            if (element.getElementName() == name && element.isIndex()) {
                indices.push_back(element.getIndex());
            }
        }
        return indices;
    }

    bool Element::checkNameId(const Element & element, const element_id_t & name_id) const
    {
        if (element.getElementName() == name_id.first && element.getId() == name_id.second) return true;
        return false;
    }

    bool Element::checkNameIndex(const Element & element, const std::string & name, const index_t & index) const
    {
        if (element.getElementName() == name && element.isIndex() && element.getIndex() == index) return true;
        return false;
    }

    double Element::stringToDouble(const std::string & str)
    {
        double value = 0;
        try {
            value = std::stod(str);
        }
        catch (...) {
            value = 0;
        }
        return value;
    }

    size_t Element::stringToSize(const std::string & str)
    {
        size_t value = 0;
        try {
            value = std::stoull(str);
        }
        catch (...) {
            value = 0;
        }
        return value;
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
