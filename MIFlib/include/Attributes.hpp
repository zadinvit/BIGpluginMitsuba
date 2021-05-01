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

namespace mif
{
    class Attributes
    {
    public: // Types section.
        typedef std::pair<std::string, std::string> attribute_t;
        typedef size_t index_t;

    public: // Constructors section.

        // Empty constructor.
        Attributes() = default;

        // Single general attribute constructor.
        Attributes(const std::string & name, const std::string & value) : _attributes(1, { name, value }) {}

        // Id attribute constructor.
        Attributes(const std::string & id) : Attributes("id", id) {}

        // Index attribute constructor.
        Attributes(const index_t & index) : Attributes("index", indexToString(index)) {}

        // General attributes constructor.
        Attributes(const std::vector<attribute_t> & attributes) : _attributes(attributes) {}

    public: // Is attribute present section.

        // Returns true if no attributes are present.
        bool empty() const { return _attributes.empty(); }

        // Returns true if attribute with given name is present.
        bool isAttribute(const std::string & name) const;

        // Returns true if id attribute is present.
        bool isId() const { return isAttribute("id"); }

        // Returns true if index attribute is present.
        bool isIndex() const { return isAttribute("index"); }

    public: // Set attribute section.

        // Adds given general attribute at the end of attribute list or updates value of current attribute with given name.
        void setAttribute(const std::string & name, const std::string & value);

        // Adds id attribute with given value at the end of attribute list or updates current id attribute.
        void setId(const std::string & id) { setAttribute("id", id); }

        // Adds index attribute with given value at the end of attribute list or updates current index attribute.
        void setIndex(const index_t & index) { setAttribute("index", indexToString(index)); }

        // Adds given attributes at the end of attribute list. Updates values of attributes that already exist.
        void setAttributes(const std::vector<attribute_t> & attributes) { for (const auto & attribute : attributes) setAttribute(attribute.first, attribute.second); }

        // Adds given attributes at the end of attribute list. Updates values of attributes that already exist.
        void setAttributes(const Attributes & attributes) { for (const auto & attribute : attributes._attributes) setAttribute(attribute.first, attribute.second); }

    public: // Get attribute section.

        // Returns value of given attribute. Returns empty string if attribute not found.
        const std::string & getAttribute(const std::string & name) const;

        // Returns value of id attribute. Returns empty string if id attribute not found.
        const std::string & getId() const { return getAttribute("id"); }

        // Returns value of index attribute. Returns 0 if index attribute not found.
        index_t getIndex() const { return stringToIndex(getAttribute("index")); }

    public: // Remove attribute section.

        // Removes attribute with given name.
        void removeAttribute(const std::string & name);

        // Removes id attribute.
        void removeId() { removeAttribute("id"); }

        // Removes index attribute.
        void removeIndex() { removeAttribute("index"); }

    public: // Iterators section.

        auto begin() const { return _attributes.cbegin(); }
        auto end() const { return _attributes.cend(); }

    public:

        // Converts string to index_t
        static index_t stringToIndex(const std::string & str);

        // Converts index_t to string
        static std::string indexToString(const index_t & index) { return std::to_string(index); }

    protected:
        std::vector<attribute_t> _attributes;
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
