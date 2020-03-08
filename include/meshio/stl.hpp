/*
 * Copyright (c) 2015, Lakshman Anumolu, Pradeep Garigipati
 * All rights reserved.
 *
 * This file is part of MeshIO whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt
 * file.
 */

#ifndef __STL_HPP__
#define __STL_HPP__

#include <meshio/vectors.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>

namespace meshio {
namespace stl {

enum class Format {
    Ascii,
    Binary,
};

/* class to store data from STL file */
template<class T>
class Data {
  public:
    std::vector< Vec4<T> >      mPositions;
    std::vector< Vec3<float> >  mNormals;

    Data() {}

    ~Data() {
        this->clear();
    }

    void resize(unsigned pNumTriangles) {
        mPositions.resize(3*pNumTriangles);
        mNormals.resize(pNumTriangles);
    }

    void clear() {
        mPositions.clear();
        mNormals.clear();
    }

    bool operator==(const Data<T>& pSTLObj) {
        if(this->mPositions.size() != pSTLObj.mPositions.size())
            return false;

        if(this->mNormals.size() != pSTLObj.mNormals.size())
            return false;

        for(unsigned i = 0; i < mPositions.size(); ++i)
            if(!(this->mPositions[i] == pSTLObj.mPositions[i]))
                return false;

        for(unsigned i = 0; i < mNormals.size(); ++i)
            if(!(this->mNormals[i] == pSTLObj.mNormals[i]))
                return false;

        return true;
    }
};

template<typename T=float>
bool read(std::vector< meshio::stl::Data<T> > &pObjects, const char* pFileName);

template<typename T=float>
bool write(const char* pFileName,
           const meshio::stl::Format pFormat,
           const std::vector< meshio::stl::Data<T> > &pObjects);

#include <meshio/details/stl.inl>

}
}

#endif // __STL_HPP__
