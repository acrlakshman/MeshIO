/*
 * Copyright (c) 2017, Lakshman Anumolu, Pradeep Garigipati
 * All rights reserved.
 *
 * This file is part of MeshIO whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt
 * file.
 */

#ifndef MESHIO_OBJ_OBJ_HPP_
#define MESHIO_OBJ_OBJ_HPP_

#define TINYOBJLOADER_IMPLEMENTATION

#include "./utilities.hpp"
#include "./meshio_defines.hpp"
#include "../third_party/tiny_obj_loader.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace meshio
{

/* Datastructure to store Wavefront OBJ */
template<class T>
class OBJAttribute : public GeometryData<T> {
  public:
    std::string mOBJName;
    std::vector<int> mMaterialIDs;

    OBJAttribute() {}
    ~OBJAttribute() {}

    void resizeVertexAttributes(unsigned pNumTriangles) {
        this->mPositions.resize(pNumTriangles);
        this->mNormals.resize(pNumTriangles);
        this->mTexturePositions.resize(pNumTriangles);
        this->mColors.resize(pNumTriangles);
        this->mParameterSpaceVertices.resize(pNumTriangles);
    }

    void resizeIndices(unsigned pNumIndices) {
        this->mTriVertexIndices.resize(pNumIndices);
        this->mTriTextureIndices.resize(pNumIndices);
        this->mTriNormalIndices.resize(pNumIndices);
    }

    void clear() {
        GeometryData<T>::clear();
        mMaterialIDs.clear();
    }

    bool operator==(const OBJAttribute<T>& pOBJAttribute) const {
        if (dynamic_cast<GeometryData<T>&>(
                        const_cast<OBJAttribute<T>&>(*this)) ==
                dynamic_cast<GeometryData<T>&>(
                        const_cast<OBJAttribute<T>&>(pOBJAttribute))) {

            if (not mOBJName.compare(pOBJAttribute.mOBJName)) {
                // Checking equality for mMaterialIDs.
                if (mMaterialIDs.size() && pOBJAttribute.mMaterialIDs.size()) {
                    for (unsigned i = 0; i < mMaterialIDs.size(); ++i) {
                        if (not (mMaterialIDs[i] == pOBJAttribute.mMaterialIDs[i])) {
                            return false;
                        }
                    }
                    return true;
                } else if (not mMaterialIDs.size() &&
                           not pOBJAttribute.mMaterialIDs.size()) {
                    return true;
                } else {
                    return false;
                }

            } else {
                return false;
            }

        } else {
            return false;
        }
    }

};

template<class T>
class OBJData {
  public:
    std::vector< OBJAttribute<T> > mOBJAttributes;
    std::vector<tinyobj::material_t> mOBJMaterials;

    OBJData() {}
    ~OBJData() {
        clear();
    }

    // TODO: Check whether this can be made virtual by making OBJData derived
    void print() {
        for (unsigned i = 0; i < mOBJAttributes.size(); ++i) {
            OBJAttribute<T> &objAttribute = mOBJAttributes[i];
            std::cout << "Number of points = "
                << objAttribute.mPositions.size() << std::endl;
            for (unsigned j = 0; j < objAttribute.mPositions.size();
                 ++j) {
                std::cout << "v " << objAttribute.mPositions[j].getX() << " "
                    << objAttribute.mPositions[j].getY() << " "
                    << objAttribute.mPositions[j].getZ() << std::endl;
            }

            std::cout << "Number of indices = "
                << objAttribute.mTriVertexIndices.size() << std::endl;
            for (unsigned j = 0; j < objAttribute.mTriVertexIndices.size();
                 ++j) {
                std::cout << "f " << objAttribute.mTriVertexIndices[j].getX()
                    << " " << objAttribute.mTriVertexIndices[j].getY()
                    << " " << objAttribute.mTriVertexIndices[j].getZ()
                    << std::endl;
            }
        }
    }

    void clear() {
        for (unsigned i = 0; i < mOBJAttributes.size(); ++i) {
            mOBJAttributes[i].clear();
        }
        mOBJAttributes.clear();
        mOBJMaterials.clear();
    }

    bool areAttributesSame(const OBJData<T>& pOBJData) {
        if (mOBJAttributes.size() == pOBJData.mOBJAttributes.size()) {
            for (unsigned i = 0; i < mOBJAttributes.size(); ++i) {
                if (not (mOBJAttributes[i] == pOBJData.mOBJAttributes[i])) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }

    bool areMaterialsSame(const OBJData<T>& pOBJData) {
        if (mOBJMaterials.size() && pOBJData.mOBJMaterials.size()) {
            for (unsigned i = 0; i < mOBJMaterials.size(); ++i) {

                if (not isEqual(mOBJMaterials[i].ambient[0],
                             pOBJData.mOBJMaterials[i].ambient[0]) ||
                    not isEqual(mOBJMaterials[i].ambient[1],
                             pOBJData.mOBJMaterials[i].ambient[1]) ||
                    not isEqual(mOBJMaterials[i].ambient[2],
                             pOBJData.mOBJMaterials[i].ambient[2])) {
                    return false;
                }

                if (not isEqual(mOBJMaterials[i].diffuse[0],
                             pOBJData.mOBJMaterials[i].diffuse[0]) ||
                    not isEqual(mOBJMaterials[i].diffuse[1],
                             pOBJData.mOBJMaterials[i].diffuse[1]) ||
                    not isEqual(mOBJMaterials[i].diffuse[2],
                             pOBJData.mOBJMaterials[i].diffuse[2])) {
                    return false;
                }

                if (not isEqual(mOBJMaterials[i].specular[0],
                             pOBJData.mOBJMaterials[i].specular[0]) ||
                    not isEqual(mOBJMaterials[i].specular[1],
                             pOBJData.mOBJMaterials[i].specular[1]) ||
                    not isEqual(mOBJMaterials[i].specular[2],
                             pOBJData.mOBJMaterials[i].specular[2])) {
                    return false;
                }

                if (not isEqual(mOBJMaterials[i].transmittance[0],
                             pOBJData.mOBJMaterials[i].transmittance[0]) ||
                    not isEqual(mOBJMaterials[i].transmittance[1],
                             pOBJData.mOBJMaterials[i].transmittance[1]) ||
                    not isEqual(mOBJMaterials[i].transmittance[2],
                             pOBJData.mOBJMaterials[i].transmittance[2])) {
                    return false;
                }

                if (not isEqual(mOBJMaterials[i].emission[0],
                             pOBJData.mOBJMaterials[i].emission[0]) ||
                    not isEqual(mOBJMaterials[i].emission[1],
                             pOBJData.mOBJMaterials[i].emission[1]) ||
                    not isEqual(mOBJMaterials[i].emission[2],
                             pOBJData.mOBJMaterials[i].emission[2])) {
                    return false;
                }

                if (not isEqual(mOBJMaterials[i].shininess,
                             pOBJData.mOBJMaterials[i].shininess)) {
                    return false;
                }

                if (not isEqual(mOBJMaterials[i].ior,
                             pOBJData.mOBJMaterials[i].ior)) {
                    return false;
                }

                if (not isEqual(mOBJMaterials[i].dissolve,
                             pOBJData.mOBJMaterials[i].dissolve)) {
                    return false;
                }

                if (not (mOBJMaterials[i].illum ==
                      pOBJData.mOBJMaterials[i].illum)) {
                    return false;
                }

                if (mOBJMaterials[i].ambient_texname.compare(
                                pOBJData.mOBJMaterials[i].ambient_texname)) {
                    return false;
                }

                if (mOBJMaterials[i].diffuse_texname.compare(
                                pOBJData.mOBJMaterials[i].diffuse_texname)) {
                    return false;
                }

                if (mOBJMaterials[i].specular_texname.compare(
                                pOBJData.mOBJMaterials[i].specular_texname)) {
                    return false;
                }

                if (mOBJMaterials[i].specular_highlight_texname.compare(
                        pOBJData.mOBJMaterials[i].specular_highlight_texname)) {
                    return false;
                }

                if (mOBJMaterials[i].bump_texname.compare(
                                pOBJData.mOBJMaterials[i].bump_texname)) {
                    return false;
                }

                if (mOBJMaterials[i].displacement_texname.compare(
                                pOBJData.mOBJMaterials[i].displacement_texname)) {
                    return false;
                }

                if (mOBJMaterials[i].alpha_texname.compare(
                                pOBJData.mOBJMaterials[i].alpha_texname)) {
                    return false;
                }

                // TODO: Comparision of unknown_parameter
            }
            return true;
        } else if (not mOBJMaterials.size() && not pOBJData.mOBJMaterials.size()) {
            return true;
        } else {
            return false;
        }
    }

    bool operator==(const OBJData<T>& pOBJData) {
        return (areAttributesSame(pOBJData) &&
                areMaterialsSame(pOBJData));
    }
};

namespace obj
{
/*
 * Usage:
 * OBJData<float> objData;
 * obj::read<float>(objData, "/path/to/file/filename.obj", mtl_file_base_path or nullptr);
 */
template<typename T=float>
bool read(OBJData<T> &pObjects, const char* pFileName,
          const char* pMatFileBasePath);

/*
 * Usage:
 * OBJData<float> objData;
 * obj::write<float>("/path/to/file/filename.obj", objData);
 */
template<typename T=float>
bool write(const char* pFileName, const OBJData<T> &pObjects);

#include "obj.inl"

}
}

#endif // MESHIO_OBJ_OBJ_HPP_
