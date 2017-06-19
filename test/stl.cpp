/*
 * Copyright (c) 2017, Lakshman Anumolu, Pradeep Garigipati
 * All rights reserved.
 *
 * This file is part of MeshIO whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt
 * file.
 */

#include <gtest/gtest.h>
#include <meshio/stl.hpp>
#include "testHelpers.hpp"

using namespace std;
using namespace meshio;

TEST(STL, INVALID_FILE)
{
    vector< STLData<float> > objs;
    EXPECT_FALSE(stl::read<float>(objs, "/home/nonexistant/cube.stl"));
}

TEST(STL, READ_BINARY)
{
    /* Reference STLData object */
    vector< STLData<float> > referenceObjs;
    initializeReferenceSTLObj(referenceObjs);

    /* Read stl file using library function */
    vector< STLData<float> > objs;
    stl::read<float>(objs, TEST_DIR "/cube_binary.stl");

    EXPECT_TRUE(objs[0] == referenceObjs[0]);

    referenceObjs.clear();
    objs.clear();
}

TEST(STL, READ_ASCII)
{
    /* Reference STLData object */
    vector< STLData<float> > referenceObjs;
    initializeReferenceSTLObj(referenceObjs);

    vector< STLData<float> > objs;
    stl::read<float>(objs, TEST_DIR "/cube_ascii.stl");

    EXPECT_TRUE(objs[0] == referenceObjs[0]);

    referenceObjs.clear();
    objs.clear();
}

TEST(STL, WRITE_INVALID_FORMAT)
{
    vector< STLData<float> > objs;
    EXPECT_FALSE(stl::write("/home/nonexistant/wrongformat.stl", static_cast<meshio::STLFormat>(2), objs));
}


TEST(STL, WRITE_TO_INVALID_BINARY_FILE)
{
    vector< STLData<float> > objs;
    EXPECT_FALSE(stl::write("/home/nonexistatnt/binary.stl", meshio::STL_BINARY, objs));
}


TEST(STL, WRITE_TO_INVALID_ASCII_FILENAME)
{
    vector< STLData<float> > objs;
    EXPECT_FALSE(stl::write("/home/nonexistatnt/ascii.stl", meshio::STL_ASCII, objs));
}

TEST(STL, WRITE_BINARY)
{
    vector< STLData<float> > objs;
    stl::read<float>(objs, TEST_DIR "/cube_ascii.stl");
    stl::write(TEST_DIR "/cube_ascii2binary.stl", meshio::STL_BINARY, objs);

    objs.clear();
}

TEST(STL, WRITE_ASCII)
{
    vector< STLData<float> > objs;
    stl::read<float>(objs, TEST_DIR "/cube_binary.stl");
    stl::write(TEST_DIR "/cube_binary2ascii.stl", meshio::STL_ASCII, objs);

    objs.clear();
}

TEST(STL, CROSS_CHECK)
{
    vector< STLData<float> > binReadObjs;
    stl::read<float>(binReadObjs, TEST_DIR "/cube_ascii2binary.stl");

    vector< STLData<float> > asciiReadObjs;
    stl::read<float>(asciiReadObjs, TEST_DIR "/cube_ascii.stl");

    EXPECT_TRUE(binReadObjs[0] == asciiReadObjs[0]);
}

TEST(STL, WRONG_ATTRIBUTES_SIZE)
{
    vector< STLData<float> > referenceObjs;
    vector< STLData<float> > tmpObjs;
    initializeReferenceSTLObj(referenceObjs);

    tmpObjs.resize(referenceObjs.size());
    EXPECT_FALSE(tmpObjs[0] == referenceObjs[0]);

    tmpObjs[0].mPositions.resize(referenceObjs[0].mPositions.size());
    EXPECT_FALSE(tmpObjs[0] == referenceObjs[0]);

    tmpObjs[0].mNormals.resize(referenceObjs[0].mNormals.size());
    EXPECT_FALSE(tmpObjs[0] == referenceObjs[0]);

    tmpObjs[0].mTexturePositions.resize(referenceObjs[0].mTexturePositions.size());
    EXPECT_FALSE(tmpObjs[0] == referenceObjs[0]);

    tmpObjs[0].mColors.resize(referenceObjs[0].mColors.size());
    EXPECT_FALSE(tmpObjs[0] == referenceObjs[0]);

    tmpObjs[0].mParameterSpaceVertices.resize(referenceObjs[0].mParameterSpaceVertices.size());
    EXPECT_FALSE(tmpObjs[0] == referenceObjs[0]);

    tmpObjs[0].mPositions = referenceObjs[0].mPositions;
    EXPECT_FALSE(tmpObjs[0] == referenceObjs[0]);

    tmpObjs[0].mNormals.push_back(meshio::Vec3<float>(1, 0, 0));
    EXPECT_FALSE(tmpObjs[0] == referenceObjs[0]);
    tmpObjs[0].mNormals = referenceObjs[0].mNormals;

    tmpObjs[0].mTexturePositions.push_back(meshio::Vec3<float>(1, 0, 0));
    EXPECT_FALSE(tmpObjs[0] == referenceObjs[0]);
    tmpObjs[0].mTexturePositions = referenceObjs[0].mTexturePositions;
    
    tmpObjs[0].mColors.push_back(meshio::Vec3<float>(1, 0, 0));
    EXPECT_FALSE(tmpObjs[0] == referenceObjs[0]);
    tmpObjs[0].mColors = referenceObjs[0].mColors;

    tmpObjs[0].mParameterSpaceVertices.push_back(meshio::Vec3<float>(1, 0, 0));
    EXPECT_FALSE(tmpObjs[0] == referenceObjs[0]);

    referenceObjs.clear();
    tmpObjs.clear();
}
