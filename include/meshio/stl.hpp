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

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <map>
#include "meshio_defines.hpp"

namespace meshio
{
namespace stl
{

template<typename T>
bool readAsciiSTL(std::vector< STLData<T> > &pObjects, const char* pFileName);

/*
 * Reads binary STL file assuming the format as described in
 * https://en.wikipedia.org/wiki/STL_(file_format). After the end of first
 * object, we assume that the next object's information starts with number of
 * triangles and the format continues.
 */
template<typename T>
bool readBinarySTL(std::vector< STLData<T> > &pObjects, const char* pFileName);

template<typename T>
bool writeAsciiSTL(const std::vector< STLData<T> > &pObjects,
                   const char* pFileName);

/*
 * Write binary STL in the same format as we read above
 */
template<typename T>
bool writeBinarySTL(const std::vector< STLData<T> > &pObjects,
                    const char* pFileName);

template<typename T>
bool computeVertexNormals(const std::vector< STLData<T> > &pObjects,
                          std::vector< std::vector< Vec3<float> > > &pNormals);

template<typename T=float>
bool readSTL(std::vector< STLData<T> > &pObjects, const char* pFileName)
{
    for(unsigned int i = 0; i < pObjects.size(); ++i)
        pObjects[i].clear();

    std::stringstream strErr;
    std::ifstream ifs(pFileName);
    if(!ifs) {
        strErr << "Cannot open file (" << pFileName << ")" << std::endl;
        std::cerr << strErr.str() << std::endl;
        return false;
    }

    const int bufferMaxSize = 80; // Only to read header
    char buffer[bufferMaxSize];

    ifs.getline(&buffer[0], bufferMaxSize);
    std::string line(&buffer[0]);
    ifs.close();

    if(line.substr(0,5) == "solid")
        return readAsciiSTL<T>(pObjects, pFileName);
    else
        return readBinarySTL<T>(pObjects, pFileName);
}

/*
 * Writes STL files in a specified format
 * "ascii" or "binary"
 */
template<typename T=float>
bool writeSTL(const std::vector< STLData<T> > &pObjects, const char* pFileName,
              const char* pOutputFileFormat="ascii")
{
    if(!strcmp(pOutputFileFormat,"ascii")) {
        return writeAsciiSTL(pObjects, pFileName);
    } else if(!strcmp(pOutputFileFormat,"binary")) {
        return writeBinarySTL(pObjects, pFileName);
    } else {
        return false;
    }
}

template<typename T=float>
bool readAsciiSTL(std::vector< STLData<T> > &pObjects, const char* pFileName)
{
    return true;
}

template<typename T=float>
bool readBinarySTL(std::vector< STLData<T> > &pObjects, const char* pFileName)
{
    uint32_t numTriangles;
    uint16_t attribByteCount;
    float value;
    std::vector<float> values;
    std::size_t sizeFloat = sizeof(float);
    std::size_t sizeUInt16 = sizeof(uint16_t);
    unsigned objectCount = 0;

    std::stringstream strErr;
    std::ifstream ifs(pFileName, std::ios::binary | std::ios::in);
    if(!ifs) {
        strErr << "Cannot open file (" << pFileName << ")" << std::endl;
        std::cerr << strErr.str() << std::endl;
        return false;
    }

    char header[80];
    ifs.read(&header[0],80);

    ifs.read((char *)&numTriangles, sizeof(uint32_t));

    while(numTriangles) {
        //std::cout<<"\tReading " << numTriangles << " facets from object "
        //    << ++objectCount << std::endl;
        STLData<T> stlObject;
        stlObject.resize(numTriangles);
        Vec4<T> position;
        Vec3<float> normal;

        // Default assignment
        position.w = (T)1.;

        for(uint32_t facet = 0; facet < numTriangles; ++facet) {
            values.clear();
            for(short i = 0; i < 3; ++i) {
                ifs.read((char *)&value, sizeFloat);
                values.push_back(value);
            }
            normal.x = values[0];
            normal.y = values[1];
            normal.z = values[2];
            stlObject.mNormals[facet] = normal;

            for(short i = 0; i < 3; ++i) {
                values.clear();
                for(short j = 0; j < 3; ++j) {
                    ifs.read((char *)&value, sizeFloat);
                    values.push_back(value);
                }
                position.x = values[0];
                position.y = values[1];
                position.z = values[2];
                stlObject.mPositions[(3*facet)+i] = position;
            }

            ifs.read((char *)&attribByteCount, sizeUInt16);
        }
        pObjects.push_back(stlObject);
        stlObject.clear();

        numTriangles = 0;
        ifs.read((char *)&numTriangles, sizeof(uint32_t));
    }

    ifs.close();

    return true;
}

template<typename T=float>
bool writeAsciiSTL(const std::vector< STLData<T> > &pObjects,
                   const char* pFileName)
{
    uint32_t numTriangles;
    uint16_t attribByteCount;
    float value;
    std::vector<float> values;
    std::size_t sizeFloat = sizeof(float);
    std::size_t sizeUInt16 = sizeof(uint16_t);
    unsigned objectCount = pObjects.size();

    std::stringstream strErr;
    std::ofstream ofs(pFileName, std::ios::binary | std::ios::out);
    if(!ofs) {
        strErr << "Cannot open file (" << pFileName << ")" << std::endl;
        std::cerr << strErr.str() << std::endl;
        return false;
    }
    std::cout<<pFileName<<std::endl;

    std::string headerStr = "<<<<<<<<<<<<<<<<<<<<<<";
    headerStr = headerStr + "Binary STL file written using MeshIO";
    headerStr = headerStr + ">>>>>>>>>>>>>>>>>>>>>>";
    const char *header = headerStr.c_str();
    std::cout<<"headerStr = "<<headerStr<<std::endl;
    ofs.write(header,80);

    for(unsigned object = 0; object < objectCount; ++object) {
        numTriangles = pObjects[object].size();
        ofs.write((char *)&numTriangles, sizeof(uint32_t));
    }

    /*
    ifs.read((char *)&numTriangles, sizeof(uint32_t));

    while(numTriangles) {
        //std::cout<<"\tReading " << numTriangles << " facets from object "
        //    << ++objectCount << std::endl;
        STLData<T> stlObject;
        stlObject.resize(numTriangles);
        Vec4<T> position;
        Vec3<float> normal;

        // Default assignment
        position.w = (T)1.;

        for(uint32_t facet = 0; facet < numTriangles; ++facet) {
            values.clear();
            for(short i = 0; i < 3; ++i) {
                ifs.read((char *)&value, sizeFloat);
                values.push_back(value);
            }
            normal.x = values[0];
            normal.y = values[1];
            normal.z = values[2];
            stlObject.mNormals[facet] = normal;

            for(short i = 0; i < 3; ++i) {
                values.clear();
                for(short j = 0; j < 3; ++j) {
                    ifs.read((char *)&value, sizeFloat);
                    values.push_back(value);
                }
                position.x = values[0];
                position.y = values[1];
                position.z = values[2];
                stlObject.mPositions[(3*facet)+i] = position;
            }

            ifs.read((char *)&attribByteCount, sizeUInt16);
        }
        pObjects.push_back(stlObject);
        stlObject.clear();

        numTriangles = 0;
        ifs.read((char *)&numTriangles, sizeof(uint32_t));
    }
    */

    ofs.close();

    return true;
}

template<typename T=float>
bool writeBinarySTL(const std::vector< STLData<T> > &pObjects,
                   const char* pFileName)
{
    return true;
}

template<typename T>
bool computeVertexNormals(const std::vector< STLData<T> > &pObjects,
                          std::vector< std::vector< Vec3<float> > > &pNormals)
{
    return true;
}

}
}

#endif // __STL_HPP__
