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
/** Implementation file for Matrix4 Generic Implementation
 *
 * @file Matrix4.cc
 * @author Andrew Keating
 */

#include <Math/Generic/Matrix4.h>


/// the identity matrix
const Scalar Matrix4::identity[] = {1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f};

/// create a scale matrix
void Matrix4::createScaleMatrix(Scalar x, Scalar y, Scalar z)
{
    
}

/// multiply this matrix and another matrix
void Matrix4::multiply(const Matrix4 &m)
{
    
}

/// multiply two other matrixes and store the result in this matrix
void Matrix4::multiply(const Matrix4 &m1, const Matrix4 &m2)
{
    
}
    
/// create a perepective matrix
void Matrix4::createPerspectiveMatrix(Scalar fov, Scalar aspect, Scalar zMin, Scalar zMax)
{
    
}

/// create a orthographic matrix
void Matrix4::createOrthographicMatrix(Scalar xMin, Scalar xMax, Scalar yMin, Scalar yMax, Scalar zMin, Scalar zMax)
{
    
}

/// create rotation matrix
void Matrix4::createRotationMatrix(Scalar angle, Scalar x, Scalar y, Scalar z)
{
    
}

/// create a translation matrix
void Matrix4::createTranslationMatrix(Scalar x, Scalar y, Scalar z)
{
    
}

/// extract the rotational component out of this matrix
void Matrix4::extractRotation(Matrix3& out)
{
    
}























