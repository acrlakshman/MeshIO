/*
 * Copyright (c) 2017, Lakshman Anumolu, Pradeep Garigipati
 * All rights reserved.
 *
 * This file is part of MeshIO whose distribution is governed by
 * the BSD 2-Clause License contained in the accompanying LICENSE.txt
 * file.
 */

#include <meshio/vectors.hpp>
#include <meshio/utilities.hpp>

#include <gtest/gtest.h>

using namespace std;
using namespace meshio;

TEST(VECTORS, 2D)
{
    Vec2<float> vecTwo(1.2,1.1);
    EXPECT_TRUE(vecTwo.size() == 2);

    // Vec2::viterator
    Vec2<float> tmpVecTwo;
    int i = 0;
    for (Vec2<float>::viterator iter = vecTwo.begin();
         iter != vecTwo.end(); ++iter, ++i) {
        if (i == 0) {
            tmpVecTwo.setX(*iter);
        }
        else {
            tmpVecTwo.setY(*iter);
        }
    }
    EXPECT_TRUE(vecTwo == tmpVecTwo);

    // Vec2::const_viterator
    i = 0;
    for (Vec2<float>::const_viterator iter = tmpVecTwo.begin();
         iter != tmpVecTwo.end(); ++iter, ++i) {
        // TODO: This should access `const_viterator end() ...` in Vec2 class
        // But looks like that function is not covered
        if (i == 0) {
            vecTwo.setX(*iter);
        }
        else {
            vecTwo.setY(*iter);
        }
    }
    EXPECT_TRUE(tmpVecTwo == vecTwo);
}

TEST(VECTORS, 3D)
{
    Vec3<float> vecThree(1.5,1.1,3.1);
    EXPECT_TRUE(vecThree.size() == 3);

    // Vec3::viterator
    Vec3<float> tmpVecThree;
    int i = 0;
    for (Vec3<float>::viterator iter = vecThree.begin();
         iter != vecThree.end(); ++iter, ++i) {
        if (i == 0) {
            tmpVecThree.setX(*iter);
        }
        else if (i == 1) {
            tmpVecThree.setY(*iter);
        }
        else {
            tmpVecThree.setZ(*iter);
        }
    }
    EXPECT_TRUE(vecThree == tmpVecThree);
}

TEST(VECTORS, 4D)
{
    Vec4<float> vecFour;
    EXPECT_TRUE(vecFour.size() == 4);
}

TEST(UTILITIES, MISCELLANEOUS)
{
    string fileBaseName;
    string refFileBaseName = "";
    fileBaseName = getFileBaseName("fileName");
    EXPECT_TRUE(not (fileBaseName.compare(refFileBaseName)));
}
