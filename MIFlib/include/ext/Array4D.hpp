/**
* Image Library - version 0.2
* --------------------------------------------------------
* Copyright (C) 2019-2021, Radomir Vavra (vavra.radomir@gmail.com)
*
* This library is distributed under the 3-clause BSD license. See notice at the end
* of this file.
*
* Version History of class Array4D:
*  0.1              First stable version, awaits further testing
*
* TODO List of class Array4D:
*   - implement initializer list constructors.
*   - add iterators (whole array / selected area).
*   - add reshape method: Reshapes size of array while overall size remains constant.
*   - consider to inherit it from Array3D.
*   - implement unit tests
*   - optimize speed
*/

#pragma once

#include <vector>
#include <algorithm>

namespace image
{
    template <class T>
    class Array4D
    {
    protected:
        std::vector<T> _data;   // data
        size_t _first;          // first dimension size
        size_t _second;         // second dimension size
        size_t _third;          // third dimension size
        size_t _fourth;         // fourth dimension size

    public:

        // Empty constructor. Constructs an empty container, with no elements.
        Array4D() : _first(0), _second(0), _third(0), _fourth(0) { }

        // Fill constructor. Constructs a container with first x second x third x fourth elements. Data are default initialized.
        Array4D(size_t first, size_t second, size_t third, size_t fourth) : _data(first * second * third * fourth), _first(_first), _second(second), _third(third), _fourth(fourth) { }

        // Fill constructor. Constructs a container with first x second x third x fourth elements. Each element is a copy of val.
        Array4D(const T &val, size_t first, size_t second, size_t third, size_t fourth) : _data(first * second * third * fourth, val), _first(_first), _second(second), _third(third), _fourth(fourth) { }

        // Fill constructor. Constructs a container with first x second x third x fourth elements. Copies data to the container. The data must be of exact size.
        Array4D(const std::vector<T> &data, size_t first, size_t second, size_t third, size_t fourth) : _data(data), _first(_first), _second(second), _third(third), _fourth(fourth)
        {
            if (data.size() != first * second * third * fourth) {
                _data.clear();
                _first = 0;
                _second = 0;
                _third = 0;
                _fourth = 0;
                throw std::length_error("size does not correspond");
            }
        }

        // Copy constructor. Constructs a container as a copy of the other.
        Array4D(const Array4D & other) = default;

        // Move constructor. Constructs a container that acquires the elements of the other.
        Array4D(Array4D && other) = default;

        // todo
        //// Initializer list constructor. Constructs a container with the contents of the initializer list.
        //Image(size_t height, size_t width, std::initializer_list<T> init_list) : Image<T>(height, width, 1, init_list) { }

        // todo
        //// Initializer list constructor. Constructs a container with the contents of the initializer list.
        //Image(size_t height, size_t width, size_t np, std::initializer_list<T> init_list) : _img(init_list.begin(), init_list.end()), _height(height), _width(width), _np(np)
        //{
        //    if (init_list.size() != height * width * np) {
        //        _img.clear();
        //        _height = 0;
        //        _width = 0;
        //        _np = 0;
        //        throw std::length_error("size does not correspond");
        //    }
        //}

        // todo
        //// Initializer list constructor, structured, 1-dimensional. Constructs a container with the contents of the initializer list.
        //Image(std::initializer_list<T> init_list)
        //{
        //    if (init_list.size()) {
        //        _img = init_list;
        //        _height = init_list.size();
        //        _width = 1;
        //        _np = 1;
        //    }
        //    else {
        //        _height = 0;
        //        _width = 0;
        //        _np = 0;
        //    }
        //}

        // todo
        //// Initializer list constructor, structured, 2-dimensional. Constructs a container with the contents of the initializer list.
        //Image(std::initializer_list<std::initializer_list<T>> init_list)
        //{
        //    if (init_list.size() && init_list.begin()->size()) {
        //        _height = init_list.size();
        //        _width = init_list.begin()->size();
        //        _np = 1;
        //        for (const auto & row : init_list) {
        //            if (row.size() != _width) {
        //                _img.clear();
        //                _height = 0;
        //                _width = 0;
        //                _np = 0;
        //                throw std::length_error("number of columns does not correspond");
        //                return;
        //            }
        //            for (const auto & col : row) {
        //                _img.push_back(col);
        //            }
        //        }
        //    }
        //    else {
        //        _height = 0;
        //        _width = 0;
        //        _np = 0;
        //    }
        //}

        // todo
        //// Initializer list constructor, structured, 3-dimensional. Constructs a container with the contents of the initializer list.
        //Image(std::initializer_list<std::initializer_list<std::initializer_list<T>>> init_list)
        //{
        //    if (init_list.size() && init_list.begin()->size() && init_list.begin()->begin()->size()) {
        //        _height = init_list.size();
        //        _width = init_list.begin()->size();
        //        _np = init_list.begin()->begin()->size();
        //        for (const auto & row : init_list) {
        //            if (row.size() != _width) {
        //                _img.clear();
        //                _height = 0;
        //                _width = 0;
        //                _np = 0;
        //                throw std::length_error("number of columns does not correspond");
        //                return;
        //            }
        //            for (const auto & col : row) {
        //                if (col.size() != _np) {
        //                    _img.clear();
        //                    _height = 0;
        //                    _width = 0;
        //                    _np = 0;
        //                    std::length_error("number of planes does not correspond");
        //                    return;
        //                }
        //                for (const auto & plane : col) {
        //                    _img.push_back(plane);
        //                }
        //            }
        //        }
        //    }
        //    else {
        //        _height = 0;
        //        _width = 0;
        //        _np = 0;
        //    }
        //}

        // Destructor
        ~Array4D() = default;

        // The copy assignment. Copies all the elements from the other into the container (with the other preserving its contents).
        Array4D & operator=(const Array4D & other) = default;

        // todo
        //// Initializer list assignment, structured, 1-dimensional. Fills the container with the contents of the initializer list.
        //Image & operator=(std::initializer_list<T> init_list)
        //{
        //    if (init_list.size()) {
        //        _img = init_list;
        //        _height = init_list.size();
        //        _width = 1;
        //        _np = 1;
        //    }
        //    else {
        //        _img.clear();
        //        _height = 0;
        //        _width = 0;
        //        _np = 0;
        //    }
        //    return *this;
        //}

        // todo
        //// Initializer list assignment, structured, 2-dimensional. Fills the container with the contents of the initializer list.
        //Image & operator=(std::initializer_list<std::initializer_list<T>> init_list)
        //{
        //    if (init_list.size() && init_list.begin()->size()) {
        //        std::vector<T> img;
        //        auto width = init_list.begin()->size();
        //        for (const auto & row : init_list) {
        //            if (row.size() != width) {
        //                throw std::length_error("number of columns does not correspond");
        //                return *this;
        //            }
        //            for (const auto & col : row) {
        //                img.push_back(col);
        //            }
        //        }
        //        _img.swap(img);
        //        _height = init_list.size();
        //        _width = width;
        //        _np = 1;
        //    }
        //    else {
        //        _img.clear();
        //        _height = 0;
        //        _width = 0;
        //        _np = 0;
        //    }
        //    return *this;
        //}

        // todo
        //// Initializer list assignment, structured, 3-dimensional. Fills the container with the contents of the initializer list.
        //Image & operator=(std::initializer_list<std::initializer_list<std::initializer_list<T>>> init_list)
        //{
        //    if (init_list.size() && init_list.begin()->size() && init_list.begin()->begin()->size()) {
        //        std::vector<T> img;
        //        auto width = init_list.begin()->size();
        //        auto np = init_list.begin()->begin()->size();
        //        for (const auto & row : init_list) {
        //            if (row.size() != width) {
        //                throw std::length_error("number of columns does not correspond");
        //                return *this;
        //            }
        //            for (const auto & col : row) {
        //                if (col.size() != np) {
        //                    std::length_error("number of planes does not correspond");
        //                    return *this;
        //                }
        //                for (const auto & plane : col) {
        //                    img.push_back(plane);
        //                }
        //            }
        //        }
        //        _img.swap(img);
        //        _height = init_list.size();
        //        _width = width;
        //        _np = np;
        //    }
        //    else {
        //        _img.clear();
        //        _height = 0;
        //        _width = 0;
        //        _np = 0;
        //    }
        //    return *this;
        //}

        // The move assignment. Moves the elements of the other into the container (the other is left empty).
        Array4D & operator=(Array4D && other) = default;

        // Returns size of first dimension.
        size_t first() const { return _first; }

        // Returns size of second dimension.
        size_t second() const { return _second; }

        // Returns size of third dimension.
        size_t third() const { return _third; }

        // Returns size of fourth dimension.
        size_t fourth() const { return _fourth; }

        // Returns the number of elements.
        size_t size() const { return _data.size(); }

        // Resizes the container so that it contains first x second x third x fourth elements. Invalidates data.
        void resize(size_t first, size_t second, size_t third, size_t fourth)
        {
            _first = first;
            _second = second;
            _third = third;
            _fourth = fourth;
            _data.resize(first * second * third * fourth);
        }

        // Test whether image is empty.
        bool empty() const { return _data.empty(); }

        // Access element by index. Does not check bounds.
        T & operator[] (size_t index) { return _data[index]; }

        // Access element by index. Does not check bounds.
        const T & operator[] (size_t index) const { return _data[index]; }

        // Access element at coordinates given by first, second, third and fourth. Does not check bounds.
        T & operator() (size_t first, size_t second, size_t third, size_t fourth)
        {
            return _data[((first * _second + second) * _third + third) * _fourth + fourth];
        }

        // Access element at coordinates given by first, second, third and fourth. Does not check bounds.
        const T & operator() (size_t first, size_t second, size_t third, size_t fourth) const
        {
            return _data[((first * _second + second) * _third + third) * _fourth + fourth];
        }

        // Access element by index. Checks bounds.
        T & at(size_t index)
        {
            if (index >= size()) throw std::out_of_range("index out of range");
            return _data[index];
        }

        // Access element by index. Checks bounds.
        const T & at(size_t index) const
        {
            if (index >= size()) throw std::out_of_range("index out of range");
            return _data[index];
        }

        // Access element at coordinates given by first, second, third and fourth. Checks bounds.
        T & at(size_t first, size_t second, size_t third, size_t fourth)
        {
            if (first >= _first) throw std::out_of_range("first out of range");
            if (second >= _second) throw std::out_of_range("second out of range");
            if (third >= _third) throw std::out_of_range("third out of range");
            if (fourth >= _fourth) throw std::out_of_range("fourth out of range");
            return _data[((first * _second + second) * _third + third) * _fourth + fourth];
        }

        // Access element at coordinates given by first, second, third and fourth. Checks bounds.
        const T & at(size_t first, size_t second, size_t third, size_t fourth) const
        {
            if (first >= _first) throw std::out_of_range("first out of range");
            if (second >= _second) throw std::out_of_range("second out of range");
            if (third >= _third) throw std::out_of_range("third out of range");
            if (fourth >= _fourth) throw std::out_of_range("fourth out of range");
            return _data[((first * _second + second) * _third + third) * _fourth + fourth];
        }

        // Returns a direct pointer to the memory array used internally by the container to store its owned elements.
        T * data() noexcept { return _data.data(); }

        // Returns a direct pointer to the memory array used internally by the container to store its owned elements.
        const T * data() const noexcept { return _data.data(); }

        // Returns reference to internal container. Potentially dangerous, do not change vector properties.
        std::vector<T> & vector() noexcept { return _data; }

        // Returns reference to internal container.
        const std::vector<T> & vector() const noexcept { return _data; }

        // Assigns val to the data, replacing its current contents, and keeping its size.
        void assign(const T &val) { _data.assign(size(), val); }

        // Assigns val to the container, replacing its current contents, and modifying its size accordingly.
        void assign(const T &val, size_t first, size_t second, size_t third, size_t fourth)
        {
            _first = first;
            _second = second;
            _third = third;
            _fourth = fourth;
            _data.assign(first * second * third * fourth, val);
        }

        // Assigns data to the container, replacing its current contents, and modifying its size accordingly.
        void assign(const std::vector<T> & data, size_t first, size_t second, size_t third, size_t fourth)
        {
            if (data.size() != first * second * third * fourth) {
                throw std::length_error("size does not correspond");
                return;
            }
            _first = first;
            _second = second;
            _third = third;
            _fourth = fourth;
            _data.assign(data.cbegin(), data.cend());
        }

        // Exchanges the content of the container by the content of the other, which is another object of the same type. Sizes may differ.
        void swap(Array4D & other)
        {
            std::swap(_data, other._data);
            std::swap(_first, other._first);
            std::swap(_second, other._second);
            std::swap(_third, other._third);
            std::swap(_fourth, other._fourth);
        }

        // Removes all data from the image
        void clear()
        {
            _data.clear();
            _first = 0;
            _second = 0;
            _third = 0;
            _fourth = 0;
        }

        friend bool operator == (const Array4D<T> & lhs, const Array4D<T> & rhs)
        {
            if (lhs.first() != rhs.first() || lhs.second() != rhs.second() || lhs.third() != rhs.third() || lhs.fourth() != rhs.fourth()) return false;
            return lhs._data == rhs._data;
        }

        friend bool operator != (const Array4D<T> & lhs, const Array4D<T> & rhs)
        {
            return !(lhs == rhs);
        }

    };
}

/**
* 3-Clause BSD License
*
* Copyright (c) 2019-2021, Radomir Vavra (vavra.radomir@gmail.com).
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
