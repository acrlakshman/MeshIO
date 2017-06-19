/*
 * Copyright (c) 2017, Lakshman Anumolu, Pradeep Garigipati
 * All rights reserved.
 *
 * This file is part of MeshIO whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt
 * file.
 */

#include "testHelpers.hpp"
#include <meshio/obj.hpp>

#include <gtest/gtest.h>

using namespace std;
using namespace meshio;

TEST(OBJ, READ_OBJ)
{
    // Reference OBJData object
    OBJData<float> refOBJData;
    initializeReferenceOBJObject(refOBJData);

    OBJData<float> objData;
    obj::read<float>(objData, TEST_DIR "/cube_tri.obj", nullptr);

    EXPECT_TRUE(objData == refOBJData);

    refOBJData.clear();
    objData.clear();
}

TEST(OBJ, WRITE_OBJ)
{
    // Reference OBJData object
    OBJData<float> refOBJData;
    initializeReferenceOBJObject(refOBJData);
    obj::write<float>(TEST_DIR "/cube_tri_write_ref.obj", refOBJData);

    OBJData<float> objData;
    obj::read<float>(objData, TEST_DIR "/cube_tri.obj", nullptr);
    obj::write<float>(TEST_DIR "/cube_tri_write.obj", objData);

    // TODO: File comparison
    EXPECT_TRUE(true); // place holder

    refOBJData.clear();
    objData.clear();
}

TEST(OBJ, WRITE_OBJ_WITH_MTL)
{
    OBJData<float> objData;
    obj::read<float>(objData, TEST_DIR "/test2.obj", nullptr);
    obj::write<float>(TEST_DIR "/test2_write.obj", objData);
    objData.clear();
}

TEST(OBJ, TEST_MISCELLANEOUS)
{
    OBJData<float> refOBJData;
    OBJData<float> tmpOBJData;

    initializeReferenceOBJObject(refOBJData);

    EXPECT_FALSE(tmpOBJData == refOBJData);

    initializeReferenceOBJObject(tmpOBJData);
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJAttributes[0].mOBJName = std::string("string");
    EXPECT_FALSE(tmpOBJData == refOBJData);
    refOBJData.mOBJAttributes[0].mOBJName = tmpOBJData.mOBJAttributes[0].mOBJName;

    tmpOBJData.mOBJAttributes[0].mTexturePositions.push_back(Vec3<float>(1,0,1));
    refOBJData.mOBJAttributes[0].mTexturePositions.push_back(Vec3<float>(1,0,0));
    EXPECT_FALSE(tmpOBJData == refOBJData);
    refOBJData.mOBJAttributes[0].mTexturePositions = tmpOBJData.mOBJAttributes[0].mTexturePositions;

    tmpOBJData.mOBJAttributes[0].mColors.push_back(Vec3<float>(1,0,1));
    refOBJData.mOBJAttributes[0].mColors.push_back(Vec3<float>(1,0,0));
    EXPECT_FALSE(tmpOBJData == refOBJData);
    refOBJData.mOBJAttributes[0].mColors = tmpOBJData.mOBJAttributes[0].mColors;

    tmpOBJData.mOBJAttributes[0].mParameterSpaceVertices.push_back(Vec3<float>(1,0,1));
    refOBJData.mOBJAttributes[0].mParameterSpaceVertices.push_back(Vec3<float>(1,0,0));
    EXPECT_FALSE(tmpOBJData == refOBJData);
    refOBJData.mOBJAttributes[0].mParameterSpaceVertices = tmpOBJData.mOBJAttributes[0].mParameterSpaceVertices;

    // Materials
    tmpOBJData.mOBJMaterials.resize(1);
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials.resize(1);
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].ambient[0] = 1.0;
    tmpOBJData.mOBJMaterials[0].ambient[1] = 1.0;
    tmpOBJData.mOBJMaterials[0].ambient[2] = 1.0;
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].ambient[0] = 1.0;
    refOBJData.mOBJMaterials[0].ambient[1] = 1.0;
    refOBJData.mOBJMaterials[0].ambient[2] = 1.0;
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].diffuse[0] = 1.0;
    tmpOBJData.mOBJMaterials[0].diffuse[1] = 1.0;
    tmpOBJData.mOBJMaterials[0].diffuse[2] = 1.0;
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].diffuse[0] = 1.0;
    refOBJData.mOBJMaterials[0].diffuse[1] = 1.0;
    refOBJData.mOBJMaterials[0].diffuse[2] = 1.0;
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].specular[0] = 1.0;
    tmpOBJData.mOBJMaterials[0].specular[1] = 1.0;
    tmpOBJData.mOBJMaterials[0].specular[2] = 1.0;
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].specular[0] = 1.0;
    refOBJData.mOBJMaterials[0].specular[1] = 1.0;
    refOBJData.mOBJMaterials[0].specular[2] = 1.0;
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].transmittance[0] = 1.0;
    tmpOBJData.mOBJMaterials[0].transmittance[1] = 1.0;
    tmpOBJData.mOBJMaterials[0].transmittance[2] = 1.0;
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].transmittance[0] = 1.0;
    refOBJData.mOBJMaterials[0].transmittance[1] = 1.0;
    refOBJData.mOBJMaterials[0].transmittance[2] = 1.0;
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].emission[0] = 1.0;
    tmpOBJData.mOBJMaterials[0].emission[1] = 1.0;
    tmpOBJData.mOBJMaterials[0].emission[2] = 1.0;
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].emission[0] = 1.0;
    refOBJData.mOBJMaterials[0].emission[1] = 1.0;
    refOBJData.mOBJMaterials[0].emission[2] = 1.0;
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].shininess = 1.0;
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].shininess = 1.0;
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].ior = 1.0;
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].ior = 1.0;
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].dissolve = 1.0;
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].dissolve = 1.0;
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].illum = 1;
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].illum = 1;
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].ambient_texname = std::string("string");
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].ambient_texname = std::string("string");
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].diffuse_texname = std::string("string");
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].diffuse_texname = std::string("string");
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].specular_texname = std::string("string");
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].specular_texname = std::string("string");
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].specular_highlight_texname = std::string("string");
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].specular_highlight_texname = std::string("string");
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].bump_texname = std::string("string");
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].bump_texname = std::string("string");
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].displacement_texname = std::string("string");
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].displacement_texname = std::string("string");
    EXPECT_TRUE(tmpOBJData == refOBJData);

    tmpOBJData.mOBJMaterials[0].alpha_texname = std::string("string");
    EXPECT_FALSE(tmpOBJData == refOBJData);

    refOBJData.mOBJMaterials[0].alpha_texname = std::string("string");
    EXPECT_TRUE(tmpOBJData == refOBJData);

    refOBJData.clear();
    tmpOBJData.clear();
}

TEST(OBJ, READ_WRITE_INVALID)
{
    OBJData<float> objData;
    EXPECT_FALSE(obj::read<float>(objData, TEST_DIR "/nonexistant/cube_tri_nonexistant.obj", nullptr));
    EXPECT_FALSE(obj::write<float>(TEST_DIR "/nonexistant/cube_tri_write_invalid.obj", objData));
}
