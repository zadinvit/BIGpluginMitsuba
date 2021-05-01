/**
* Image Library - version 0.2
* --------------------------------------------------------
* Copyright (C) 2019-2021, Radomir Vavra (vavra.radomir@gmail.com)
*
* This library is distributed under the 3-clause BSD license. See notice at the end
* of this file.
*
* Version History of class Image:
*  0.1              First stable version, awaits further testing
*
* TODO List of class Image:
*   - implement independent class Array2D and Array3D.
*   - add iterators (whole array / selected area).
*   - add reshape method: Reshapes size of array while overall size remains constant.
*   - inherit Image from Array3D.
*   - add image data conversions.
*   - add getArea method (black surrounding / user defined surrounding)
*   - implement unit tests
*   - optimize speed
*/

#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

namespace image
{
    template <class T>
    class Image
    {
    protected:
        std::vector<T> _img;    // data
        size_t _height;         // number of rows
        size_t _width;          // number of columns
        size_t _np;             // number of (color) planes

    public:

        // Empty constructor. Constructs an empty container, with no elements.
        Image() : _height(0), _width(0), _np(0) { }

        // Fill constructor. Constructs a container with width x height x np elements. Data are default initialized.
        Image(size_t height, size_t width, size_t np = 1) : _img(height * width * np), _height(height), _width(width), _np(np) { }

        // Fill constructor. Constructs a container with width x height x np elements. Each element is a copy of val. np is required to not interfere with previous fill constructor.
        Image(const T &val, size_t height, size_t width, size_t np) : _img(height * width * np, val), _height(height), _width(width), _np(np) { }

        // Fill constructor. Constructs a container with height x width x np elements. Copies img to the container. The img must be of exact size.
        Image(const std::vector<T> &img, size_t height, size_t width, size_t np = 1) : _img(img), _height(height), _width(width), _np(np)
        {
            if (img.size() != height * width * np) {
                _img.clear();
                _height = 0;
                _width = 0;
                _np = 0;
                throw std::length_error("size does not correspond");
            }
        }

        // Fill constructor. Constructs a container with height x width x np elements. Copies data to the container. The data must be of size of at least height x width x np elements.
        Image(const T * data, size_t height, size_t width, size_t np = 1) : _img(data, data + height * width * np), _height(height), _width(width), _np(np) { }

        // Copy constructor. Constructs a container as a copy of the other.
        Image(const Image & other) = default;

        // Move constructor. Constructs a container that acquires the elements of the other.
        Image(Image && other) = default;

        // Initializer list constructor. Constructs a container with the contents of the initializer list.
        Image(size_t height, size_t width, std::initializer_list<T> init_list) : Image<T>(height, width, 1, init_list) { }

        // Initializer list constructor. Constructs a container with the contents of the initializer list.
        Image(size_t height, size_t width, size_t np, std::initializer_list<T> init_list) : _img(init_list.begin(), init_list.end()), _height(height), _width(width), _np(np)
        {
            if (init_list.size() != height * width * np) {
                _img.clear();
                _height = 0;
                _width = 0;
                _np = 0;
                throw std::length_error("size does not correspond");
            }
        }

        // Initializer list constructor, structured, 1-dimensional. Constructs a container with the contents of the initializer list.
        Image(std::initializer_list<T> init_list)
        {
            if (init_list.size()) {
                _img = init_list;
                _height = init_list.size();
                _width = 1;
                _np = 1;
            }
            else {
                _height = 0;
                _width = 0;
                _np = 0;
            }
        }

        // Initializer list constructor, structured, 2-dimensional. Constructs a container with the contents of the initializer list.
        Image(std::initializer_list<std::initializer_list<T>> init_list)
        {
            if (init_list.size() && init_list.begin()->size()) {
                _height = init_list.size();
                _width = init_list.begin()->size();
                _np = 1;
                for (const auto & row : init_list) {
                    if (row.size() != _width) {
                        _img.clear();
                        _height = 0;
                        _width = 0;
                        _np = 0;
                        throw std::length_error("number of columns does not correspond");
                        return;
                    }
                    for (const auto & col : row) {
                        _img.push_back(col);
                    }
                }
            }
            else {
                _height = 0;
                _width = 0;
                _np = 0;
            }
        }

        // Initializer list constructor, structured, 3-dimensional. Constructs a container with the contents of the initializer list.
        Image(std::initializer_list<std::initializer_list<std::initializer_list<T>>> init_list)
        {
            if (init_list.size() && init_list.begin()->size() && init_list.begin()->begin()->size()) {
                _height = init_list.size();
                _width = init_list.begin()->size();
                _np = init_list.begin()->begin()->size();
                for (const auto & row : init_list) {
                    if (row.size() != _width) {
                        _img.clear();
                        _height = 0;
                        _width = 0;
                        _np = 0;
                        throw std::length_error("number of columns does not correspond");
                        return;
                    }
                    for (const auto & col : row) {
                        if (col.size() != _np) {
                            _img.clear();
                            _height = 0;
                            _width = 0;
                            _np = 0;
                            std::length_error("number of planes does not correspond");
                            return;
                        }
                        for (const auto & plane : col) {
                            _img.push_back(plane);
                        }
                    }
                }
            }
            else {
                _height = 0;
                _width = 0;
                _np = 0;
            }
        }

        // Destructor
        ~Image() = default;

        // The copy assignment. Copies all the elements from the other into the container (with the other preserving its contents).
        Image & operator=(const Image & other) = default;

        // Initializer list assignment, structured, 1-dimensional. Fills the container with the contents of the initializer list.
        Image & operator=(std::initializer_list<T> init_list)
        {
            if (init_list.size()) {
                _img = init_list;
                _height = init_list.size();
                _width = 1;
                _np = 1;
            }
            else {
                _img.clear();
                _height = 0;
                _width = 0;
                _np = 0;
            }
            return *this;
        }

        // Initializer list assignment, structured, 2-dimensional. Fills the container with the contents of the initializer list.
        Image & operator=(std::initializer_list<std::initializer_list<T>> init_list)
        {
            if (init_list.size() && init_list.begin()->size()) {
                std::vector<T> img;
                auto width = init_list.begin()->size();
                for (const auto & row : init_list) {
                    if (row.size() != width) {
                        throw std::length_error("number of columns does not correspond");
                        return *this;
                    }
                    for (const auto & col : row) {
                        img.push_back(col);
                    }
                }
                _img.swap(img);
                _height = init_list.size();
                _width = width;
                _np = 1;
            }
            else {
                _img.clear();
                _height = 0;
                _width = 0;
                _np = 0;
            }
            return *this;
        }

        // Initializer list assignment, structured, 3-dimensional. Fills the container with the contents of the initializer list.
        Image & operator=(std::initializer_list<std::initializer_list<std::initializer_list<T>>> init_list)
        {
            if (init_list.size() && init_list.begin()->size() && init_list.begin()->begin()->size()) {
                std::vector<T> img;
                auto width = init_list.begin()->size();
                auto np = init_list.begin()->begin()->size();
                for (const auto & row : init_list) {
                    if (row.size() != width) {
                        throw std::length_error("number of columns does not correspond");
                        return *this;
                    }
                    for (const auto & col : row) {
                        if (col.size() != np) {
                            std::length_error("number of planes does not correspond");
                            return *this;
                        }
                        for (const auto & plane : col) {
                            img.push_back(plane);
                        }
                    }
                }
                _img.swap(img);
                _height = init_list.size();
                _width = width;
                _np = np;
            }
            else {
                _img.clear();
                _height = 0;
                _width = 0;
                _np = 0;
            }
            return *this;
        }

        // The move assignment. Moves the elements of the other into the container (the other is left empty).
        Image & operator=(Image && other) = default;

        // Returns the number of rows of the image.
        size_t height() const { return _height; }

        // Returns the number of columns of the image.
        size_t width() const { return _width; }

        // Returns the number of (color) planes.
        size_t np() const { return _np; }

        // Returns the number of elements.
        size_t size() const { return _img.size(); }

        // Resizes the container so that it contains height x width x np elements. Invalidates data.
        void resize(size_t height, size_t width, size_t np = 1)
        {
            _height = height;
            _width = width;
            _np = np;
            _img.resize(height * width * np);
        }

        // Test whether image is empty.
        bool empty() const { return _img.empty(); }

        // Access element by index. Does not check bounds.
        T & operator[] (size_t index)
        {
            return _img[index];
        }

        // Access element by index. Does not check bounds.
        const T & operator[] (size_t index) const
        {
            return _img[index];
        }

        // Access element at given row and column assuming the number of (color) planes is 1. Does not check bounds.
        T & operator() (size_t row, size_t col)
        {
            return _img[row * _width + col];
        }

        // Access element at given row and column assuming the number of (color) planes is 1. Does not check bounds.
        const T & operator() (size_t row, size_t col) const
        {
            return _img[row * _width + col];
        }

        // Access element at given row, column and (color) plane. Does not check bounds.
        T & operator() (size_t row, size_t col, size_t plane)
        {
            return _img[(row * _width + col) * _np + plane];
        }

        // Access element at given row, column and (color) plane. Does not check bounds.
        const T & operator() (size_t row, size_t col, size_t plane) const
        {
            return _img[(row * _width + col) * _np + plane];
        }

        // Access element by index. Checks bounds.
        T & at(size_t index)
        {
            if (index >= size()) throw std::out_of_range("index out of range");
            return _img[index];
        }

        // Access element by index. Checks bounds.
        const T & at(size_t index) const
        {
            if (index >= size()) throw std::out_of_range("index out of range");
            return _img[index];
        }

        // Access element at given row and column assuming the number of (color) planes is 1. Checks bounds.
        T & at(size_t row, size_t col)
        {
            if (row >= _height) throw std::out_of_range("row out of range");
            if (col >= _width) throw std::out_of_range("col out of range");
            if (_np != 1) return _img[(row * _width + col) * _np];
            return _img[row * _width + col];
        }

        // Access element at given row and column assuming the number of (color) planes is 1. Checks bounds.
        const T & at(size_t row, size_t col) const
        {
            if (row >= _height) throw std::out_of_range("row out of range");
            if (col >= _width) throw std::out_of_range("col out of range");
            if (_np != 1) return _img[(row * _width + col) * _np];
            return _img[row * _width + col];
        }

        // Access element at given row, column and (color) plane. Checks bounds.
        T & at(size_t row, size_t col, size_t plane)
        {
            if (row >= _height) throw std::out_of_range("row out of range");
            if (col >= _width) throw std::out_of_range("col out of range");
            if (plane >= _np) throw std::out_of_range("plane out of range");
            return _img[(row * _width + col) * _np + plane];
        }

        // Access element at given row, column and (color) plane. Checks bounds.
        const T & at(size_t row, size_t col, size_t plane) const
        {
            if (row >= _height) throw std::out_of_range("row out of range");
            if (col >= _width) throw std::out_of_range("col out of range");
            if (plane >= _np) throw std::out_of_range("plane out of range");
            return _img[(row * _width + col) * _np + plane];
        }

        // Returns a direct pointer to the memory array used internally by the image to store its owned elements.
        T * data() noexcept
        {
            return _img.data();
        }

        // Returns a direct pointer to the memory array used internally by the image to store its owned elements.
        const T * data() const noexcept
        {
            return _img.data();
        }

        // todo: comment
        std::vector<T> & vector() noexcept
        {
            return _img;
        }

        // todo: comment
        const std::vector<T> & vector() const noexcept
        {
            return _img;
        }

        // Assigns val to the image, replacing its current contents, and keeping its size.
        void assign(const T &val)
        {
            _img.assign(size(), val);
        }

        // Assigns val to the image, replacing its current contents, and modifying its size accordingly.
        void assign(const T &val, size_t height, size_t width, size_t np = 1)
        {
            _height = height;
            _width = width;
            _np = np;
            _img.assign(height * width * np, val);
        }

        // Assigns img to the image, replacing its current contents, and modifying its size accordingly.
        void assign(const std::vector<T> &img, size_t height, size_t width, size_t np = 1)
        {
            if (img.size() != height * width * np) {
                throw std::length_error("size does not correspond");
                return;
            }
            _height = height;
            _width = width;
            _np = np;
            _img.assign(img.cbegin(), img.cend());
        }

        // Exchanges the content of the container by the content of the other, which is another object of the same type. Sizes may differ.
        void swap(Image & other)
        {
            std::swap(_img, other._img);
            std::swap(_height, other._height);
            std::swap(_width, other._width);
            std::swap(_np, other._np);
        }

        // Removes all data from the image
        void clear()
        {
            _img.clear();
            _height = 0;
            _width = 0;
            _np = 0;
        }




        // TODO: iterators -------------------------------------------------------------------------------------------
        template<typename U>
        struct iterator
        {
            std::vector<U> & _img;
            size_t pointer;

            iterator(std::vector<U> & _img) : _img{ _img }, pointer{ 0 } {}

            iterator(std::vector<U> & _img, size_t size) : _img{ _img }, pointer{ size } {}

            bool operator!=(const iterator<U>& other) const { return !(*this == other); }

            bool operator==(const iterator<U>& other) const { return pointer == other.pointer; }

            iterator& operator++() { ++pointer; return *this; }

            T& operator*() const { return _img[pointer]; }
        };

        iterator<T> begin() { return iterator<T>(_img); }
        iterator<T> end() { return iterator<T>(_img, size()); }

        template<typename U>
        struct iterator_row
        {
            std::vector<T> & _img;  // data
            //size_t _height;         // number of rows
            //size_t _width;          // number of columns
            //size_t _np;             // number of (color) planes            
            size_t pointer;
            size_t jump;

            iterator_row(std::vector<U> & _img, size_t width, size_t np) : _img{ _img }, jump{ width * np }, pointer{ 0 } {}

            iterator_row(std::vector<U> & _img, size_t height, size_t width, size_t np) : _img{ _img }, pointer{ height * width * np } {}

            bool operator!=(const iterator_row<U>& other) const { return !(*this == other); }

            bool operator==(const iterator_row<U>& other) const { return pointer == other.pointer; }

            iterator_row& operator++() { pointer += jump; return *this; }

            T& operator*() const { return _img[pointer]; }
        };

        iterator_row<T> begin_rows() { return iterator_row<T>(_img, _width, _np); }
        iterator_row<T> end_rows() { return iterator_row<T>(_img, _height, _width, _np); }

        //class iterator {
        //public:
        //    //typedef typename std::vector::difference_type difference_type;
        //    //typedef typename A::value_type value_type;
        //    //typedef typename A::reference reference;
        //    //typedef typename A::pointer pointer;
        //    typedef std::random_access_iterator_tag iterator_category; //or another tag

        //    iterator();
        //    iterator(const iterator&);
        //    ~iterator();

        //    iterator& operator=(const iterator&);
        //    bool operator==(const iterator&) const;
        //    bool operator!=(const iterator&) const;
        //    bool operator<(const iterator&) const; //optional
        //    bool operator>(const iterator&) const; //optional
        //    bool operator<=(const iterator&) const; //optional
        //    bool operator>=(const iterator&) const; //optional

        //    iterator& operator++();
        //    iterator operator++(int); //optional
        //    iterator& operator--(); //optional
        //    iterator operator--(int); //optional
        //    iterator& operator+=(size_type); //optional
        //    iterator operator+(size_type) const; //optional
        //    friend iterator operator+(size_type, const iterator&); //optional
        //    iterator& operator-=(size_type); //optional            
        //    iterator operator-(size_type) const; //optional
        //    difference_type operator-(iterator) const; //optional

        //    reference operator*() const;
        //    pointer operator->() const;
        //    reference operator[](size_type) const; //optional
        //};
    // END: iterators --------------------------------------------------------------------------------------




        friend std::ostream& operator << (std::ostream& out, const Image<T>& image)
        {
            out.write((const char *)&image._height, sizeof(image._height));
            out.write((const char *)&image._width, sizeof(image._width));
            out.write((const char *)&image._np, sizeof(image._np));
            out.write((const char *)image._img.data(), image.size() * sizeof(T));
            return out;
        }

        friend std::istream& operator >> (std::istream& in, Image<T>& image)
        {
            in.read((char *)&image._height, sizeof(image._height));
            in.read((char *)&image._width, sizeof(image._width));
            in.read((char *)&image._np, sizeof(image._np));
            image.resize(image._height, image._width, image._np);
            in.read((char *)image._img.data(), image.size() * sizeof(T));
            return in;
        }

        friend bool operator == (const Image<T> & lhs, const Image<T> & rhs)
        {
            if (lhs.height() != rhs.height() || lhs.width() != rhs.width() || lhs.np() != rhs.np()) return false;
            return lhs._img == rhs._img;
        }

        friend bool operator != (const Image<T> & lhs, const Image<T> & rhs)
        {
            return !(lhs == rhs);
        }

    };


    //template<typename T>
    //inline QDataStream& operator << (QDataStream& out, const Image<T>& image)
    //{
    //    out << static_cast<quint64>(image.height()) << static_cast<quint64>(image.width()) << static_cast<quint64>(image.np());
    //    // todo: deal with big endian and little endian systems
    //    out.writeRawData((const char *)image.data(), static_cast<int>(image.size() * sizeof(T)));
    //    return out;
    //}
    //
    //template<typename T>
    //QDataStream& operator >> (QDataStream& in, Image<T>& image)
    //{
    //    quint64 height, width, np;
    //    in >> height >> width >> np;
    //    if (in.status() != QDataStream::Ok) return in;
    //    image.resize(static_cast<size_t>(height), static_cast<size_t>(width), static_cast<size_t>(np));
    //    in.readRawData((char *)image.data(), static_cast<int>(image.size() * sizeof(T)));
    //    if (in.status() != QDataStream::Ok) image = Image<T>();
    //    return in;
    //}
    //
    //

    //template <class T>
    //class Image2 : public Image
    //{
    //public:
    //
    //    // Converts image to unsigned char using clipping.
    //    operator Image<unsigned char>() const
    //    {
    //        Image<unsigned char> image(_width, _height, _np);
    //        for (size_t i = 0; i < size(); ++i) {
    //            image[i] = static_cast<unsigned char>(std::min(std::max(static_cast<int>(_img[i]), 0), 255));
    //        }
    //        return image;
    //    }
    //
    //    // Converts image to float.
    //    operator Image<float>() const
    //    {
    //        Image<float> image(_width, _height, _np);
    //        for (size_t i = 0; i < size(); ++i) {
    //            image[i] = static_cast<float>(_img[i]);
    //        }
    //        return image;
    //    }
    //};


    //template<class T>
    //bool operator==(const Image<T> & lhs, const Image<T> & rhs)
    //{
    //    if (lhs.height() != rhs.height() || lhs.width() != rhs.width() || lhs.np() != rhs.np()) return false;
    //    for (decltype(lhs.height()) row = 0; row != lhs.height(); ++row) {
    //        for (decltype(lhs.width()) col = 0; col != lhs.width(); ++col) {
    //            for (decltype(lhs.np()) p = 0; p != lhs.np(); ++p) {
    //                if (lhs(row, col, p) != rhs(row, col, p)) return false;
    //            }
    //        }
    //    }
    //    return true;
    //}
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
