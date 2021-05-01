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
#include <string>
#include "Attributes.hpp"

namespace mif
{
    class Element
    {
    public: // Types section.
        typedef Attributes::attribute_t attribute_t;
        typedef Attributes::index_t index_t;
        typedef std::vector<Element> elements_t;
        typedef std::pair<std::string, std::string> element_id_t;
        typedef std::vector<element_id_t> path_t;

    public: // Constructors section.

        // Empty constructor.
        Element() = default;

        // Element with possible attributes constructor.
        explicit Element(const std::string & name, const Attributes & attributes = {});

        // Text element with possible attributes constructor.
        Element(const std::string & name, const std::string & text, const Attributes & attributes = {});

        // Parent element with possible attributes constructor.
        Element(const std::string & name, const elements_t & elements, const Attributes & attributes = {});

        // Element with id constructor.
        explicit Element(const element_id_t & name_id);

        // Element with index constructor.
        Element(const std::string & name, const index_t & index);

    public: // Element state section.

        // Name (N), Text (T), Elements (E) present table (a = any)
        // N T E
        // 0 a a - empty
        // 1     - element empty
        // 1   1 - element parent
        // 1 1   - element text
        // 1 1 1 - element mixed

        bool isEmpty() const { return _name.empty(); }
        bool isElementEmpty() const { return !_name.empty() && _text.empty() && _elements.empty(); }
        bool isElementText() const { return !_name.empty() && !_text.empty() && _elements.empty(); }
        bool isElementParent() const { return !_name.empty() && _text.empty() && !_elements.empty(); }
        bool isElementMixed() const { return !_name.empty() && !_text.empty() && !_elements.empty(); }
        bool hasAttributes() const { return !_attributes.empty(); }

    public: // Modify, set and get simple properties section.

        // Clears everything.
        void clear();

        // Sets name of the element.
        void setElementName(const std::string & name) { _name = name; }

        // Returns name of the element.
        std::string & getElementName() { return _name; }

        // Returns name of the element.
        const std::string & getElementName() const { return _name; }

        // Sets text of the element.
        void setElementText(const std::string & text) { _text = text; }

        // Returns text of the element.
        std::string & getElementText() { return _text; }

        // Returns text of the element.
        const std::string & getElementText() const { return _text; }

    public: // Attributes section.

        // Returns true if attribute with given name is present.
        bool isAttribute(const std::string & name) const { return _attributes.isAttribute(name); }

        // Returns true if id attribute is present.
        bool isId() const { return _attributes.isId(); }

        // Returns true if index attribute is present.
        bool isIndex() const { return _attributes.isIndex(); }

        // Adds given general attribute at the end of attribute list or updates value of current attribute with given name.
        void setAttribute(const std::string & name, const std::string & value) { _attributes.setAttribute(name, value); }

        // Adds id attribute with given value at the end of attribute list or updates current id attribute.
        void setId(const std::string & id) { _attributes.setId(id); }

        // Adds index attribute with given value at the end of attribute list or updates current index attribute.
        void setIndex(const index_t & index) { _attributes.setIndex(index); }

        // Adds given attributes at the end of attribute list. Updates values of attributes that already exist.
        void setAttributes(const std::vector<attribute_t> & attributes) { _attributes.setAttributes(attributes); }

        // Adds given attributes at the end of attribute list. Updates values of attributes that already exist.
        void setAttributes(const Attributes & attributes) { _attributes.setAttributes(attributes); }

        // Returns value of given attribute. Returns empty string if attribute not found.
        const std::string & getAttribute(const std::string & name) const { return _attributes.getAttribute(name); }

        // Returns value of id attribute. Returns empty string if id attribute not found.
        const std::string & getId() const { return _attributes.getId(); }

        // Returns value of index attribute. Returns 0 if index attribute not found.
        index_t getIndex() const { return _attributes.getIndex(); }

        // Returns all attributes.
        Attributes & getAttributes() { return _attributes; }

        // Returns all attributes.
        const Attributes & getAttributes() const { return _attributes; }

        // Removes attribute with given name.
        void removeAttribute(const std::string & name) { _attributes.removeAttribute(name); }

        // Removes id attribute.
        void removeId() { _attributes.removeId(); }

        // Removes index attribute.
        void removeIndex() { _attributes.removeIndex(); }

    public: // Child elements section - single element - is child element present.

        // Returns true if at least one child element with given name is found at given path.
        bool isChildElement(const std::string & name, const path_t & path = {}) const;

        // Returns true if at least one element is found at given path.
        bool isChildElement(const path_t & path = {}) const;

        // Returns true if at least one child element with given name and index is found at given path.
        bool isChildElement(const std::string & name, const index_t & index, const path_t & path = {}) const;

    public: // Child elements section - single element - add new child element.

        // Adds given child element at the end of child elements list of element specified by path.
        Element & pushChildElement(const Element & element, const path_t & path = {});

    public: // Child elements section - single element - get child element.

        // Returns first child element with given name at given path. Creates new element at given path if nothing found.
        Element & getChildElement(const std::string & name, const path_t & path = {});

        // Returns first child element with given name at given path. Returns empty element if nothing found.
        const Element & getChildElement(const std::string & name, const path_t & path = {}) const;

        // Returns first element at given path. Creates new element at given path if nothing found.
        Element & getChildElement(const path_t & path = {});

        // Returns first element at given path. Returns empty element if nothing found.
        const Element & getChildElement(const path_t & path = {}) const;

        // Returns first child element with given name and index at given path. Creates new element at given path if nothing found.
        Element & getChildElement(const std::string & name, const index_t & index, const path_t & path = {});

        // Returns first child element with given name and index at given path. Returns empty element if nothing found.
        const Element & getChildElement(const std::string & name, const index_t & index, const path_t & path = {}) const;

    public: // Child elements section - single element - remove child element.

        // Removes all child elements with given name at given path.
        void removeChildElement(const std::string & name, const path_t & path = {});

        // Removes all child elements at given path.
        void removeChildElement(const path_t & path);

        // Removes all child elements with given name and index at given path.
        void removeChildElement(const std::string & name, const index_t & index, const path_t & path = {});

    public: // Child elements section - multiple elements.

        // Adds given elements at the end of child elements list of element at given path.
        void pushChildElements(const elements_t & elements, const path_t & path = {});

        // Returns all child elements of element at given path.
        elements_t & getChildElements(const path_t & path = {});

        // Returns all child elements of element at given path.
        const elements_t & getChildElements(const path_t & path = {}) const;

        // Returns all child elements of element at given path with given name.
        std::vector<Element *> getChildElements(const std::string & name, const path_t & path = {});

        // Returns all child elements of element at given path with given name.
        std::vector<const Element *> getChildElements(const std::string & name, const path_t & path = {}) const;

        // Returns all child elements of element at given path with given name and id.
        std::vector<Element *> getChildElements(const element_id_t & name_id, const path_t & path = {});

        // Returns all child elements of element at given path with given name and id.
        std::vector<const Element *> getChildElements(const element_id_t & name_id, const path_t & path = {}) const;

        // Returns all child elements of element at given path with given name and index.
        std::vector<Element *> getChildElements(const std::string & name, const index_t & index, const path_t & path = {});

        // Returns all child elements of element at given path with given name and index.
        std::vector<const Element *> getChildElements(const std::string & name, const index_t & index, const path_t & path = {}) const;

        // Returns ids of all child elements of element at given path with given name.
        std::vector<std::string> getChildElementsIDs(const std::string & name, const path_t & path = {}) const;

        // Returns indices of all child elements of element at given path with given name.
        std::vector<index_t> getChildElementsIndices(const std::string & name, const path_t & path = {}) const;

        // Removes all child elements.
        void removeChildElements() { _elements.clear(); }

    protected:

        // Returns true if finds element with given name and corresponding id. If given id is empty, only name is checked.
        bool checkNameId(const Element & element, const element_id_t & name_id) const;

        // Returns true if finds element with given name and index.
        bool checkNameIndex(const Element & element, const std::string & name, const index_t & index) const;

    protected:

        // Converts string to double
        static double stringToDouble(const std::string & str);

        // Converts double to string
        static std::string doubleToString(double value) { return std::to_string(value); }

        // Converts string to size_t
        static size_t stringToSize(const std::string & str);

        // Converts size_t to string
        static std::string sizeToString(size_t value) { return std::to_string(value); }

        // Converts string to index_t
        static index_t stringToIndex(const std::string & str) { return Attributes::stringToIndex(str); }

        // Converts index_t to string
        static std::string indexToString(const index_t & index) { return Attributes::indexToString(index); }

    protected:
        std::string _name;          // Element name.
        std::string _text;          // Text of the element.
        Attributes _attributes;     // Attributes of the element.
        elements_t _elements;       // Child elements.
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
