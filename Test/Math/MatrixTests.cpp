/* 
Copyright (c) 2011 Andrew Keating

This file is part of 3DMagic.

3DMagic is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

3DMagic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with 3DMagic.  If not, see <http://www.gnu.org/licenses/>.

*/
/** Contains Math Matrix tests
 */

// include google test framework
#include <gtest/gtest.h>

// include Matrix class from 3DMagic library
#include <Math/Matrix4.h>
#include <Math/Matrix3.h>
//using namespace Magic3D;


/** Fixture for Math Matrix4 tests
 */
class Math_Matrix4Tests: public ::testing::Test
{
protected:
    // up to four matrixes are used in tests
    Matrix4 m1;
    Matrix4 m2;
    Matrix4 actual;
    Matrix4 expected;
    
    /// setup method
    virtual void SetUp()
    {
        // no setup
    }
    
    /// teardown method
    virtual void TearDown()
    {
        // no teardown
    }
    
    /// test equaility of two matrices
    void ASSERT_MATRIX_EQ(const Matrix4& m1, const Matrix4& m2)
    {
        ASSERT_FLOAT_EQ(m1.get(0,0), m2.get(0,0));
        ASSERT_FLOAT_EQ(m1.get(0,1), m2.get(0,1));
        ASSERT_FLOAT_EQ(m1.get(0,2), m2.get(0,2));
        ASSERT_FLOAT_EQ(m1.get(0,3), m2.get(0,3));
        ASSERT_FLOAT_EQ(m1.get(1,0), m2.get(1,0));
        ASSERT_FLOAT_EQ(m1.get(1,1), m2.get(1,1));
        ASSERT_FLOAT_EQ(m1.get(1,2), m2.get(1,2));
        ASSERT_FLOAT_EQ(m1.get(1,3), m2.get(1,3));
        ASSERT_FLOAT_EQ(m1.get(2,0), m2.get(2,0));
        ASSERT_FLOAT_EQ(m1.get(2,1), m2.get(2,1));
        ASSERT_FLOAT_EQ(m1.get(2,2), m2.get(2,2));
        ASSERT_FLOAT_EQ(m1.get(2,3), m2.get(2,3));
        ASSERT_FLOAT_EQ(m1.get(3,0), m2.get(3,0));
        ASSERT_FLOAT_EQ(m1.get(3,1), m2.get(3,1));
        ASSERT_FLOAT_EQ(m1.get(3,2), m2.get(3,2));
        ASSERT_FLOAT_EQ(m1.get(3,3), m2.get(3,3));
    }
    
    /// set the 16 elements of a matrix
    void matrixSet(Matrix4& m, Scalar e0, Scalar e1, Scalar e2, Scalar e3, Scalar e4
        , Scalar e5, Scalar e6, Scalar e7, Scalar e8, Scalar e9, Scalar e10
        , Scalar e11, Scalar e12, Scalar e13, Scalar e14, Scalar e15)
    {
        m.set(0,0,e0);
        m.set(0,1,e1);
        m.set(0,2,e2);
        m.set(0,3,e3);
        m.set(1,0,e4);
        m.set(1,1,e5);
        m.set(1,2,e6);
        m.set(1,3,e7);
        m.set(2,0,e8);
        m.set(2,1,e9);
        m.set(2,2,e10);
        m.set(2,3,e11);
        m.set(3,0,e12);
        m.set(3,1,e13);
        m.set(3,2,e14);
        m.set(3,3,e15);
    }
    
};


/// tests Matrix4 multiply and assign method
TEST_F(Math_Matrix4Tests, MultiplyAndAssign)
{
    // setup matrices
    matrixSet(actual, 5,-71,-13,86,-86,-8,-62,8,-69,-106,81,125,-1,-52,84,47);
    matrixSet(m1, -92,-59,-55,-91,47,-86,-93,-32,19,126,44,92,32,112,119,-94);
    matrixSet(expected, 8500,17566,-7245,-19536,14080,8873,-5500,-9775,-13869,
              -11805,3233,12466,-17589,-10894,-5617,14105);
              
    // perform test
    actual.multiply(m1);
    ASSERT_MATRIX_EQ(actual,expected);
}

/// tests Matrix4 multiply method
TEST_F(Math_Matrix4Tests, Multiply)
{
    // setup matrices
    matrixSet(m1, 5,-71,-13,86,-86,-8,-62,8,-69,-106,81,125,-1,-52,84,47);
    matrixSet(m2, -92,-59,-55,-91,47,-86,-93,-32,19,126,44,92,32,112,119,-94);
    matrixSet(expected, 8500,17566,-7245,-19536,14080,8873,-5500,-9775,-13869,
              -11805,3233,12466,-17589,-10894,-5617,14105);
              
    // perform test
    actual.multiply(m1,m2);
    ASSERT_MATRIX_EQ(actual,expected);
}

/// tests Matrix4 getCameraMatrix method

/// tests Matrix4 extractRotation method





















