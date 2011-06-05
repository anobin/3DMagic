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
/** Header file for Matrix3 Intel implementation
 *
 * @file Matrix3.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MATRIX3_INTEL_H
#define MAGIC3D_MATRIX3_INTEL_H

// TODO
#include "MathTypes.h"
#include "Vector3.h"

/** Represents a 3-component (x,y,z) matrix. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Matrix3
{
public:
    Matrix3();

    Matrix3(const Matrix3 &copy);

    void set(const Matrix3 &copy);

    Scalar get(unsigned int col, unsigned int row) const ;

    void set(unsigned int col, unsigned int row, Scalar value);

    void setColumn(unsigned int col, const Vector3 &v);

    void getColumn(unsigned int col, Vector3 *out) const ;

    void multiply(const Matrix3 &m);

    void multiply(const Matrix3 &m1, const Matrix3 &m2);

    void createScaleMatrix(Scalar x, Scalar y, Scalar z);

    void createRotationMatrix(Scalar angle, Scalar x, Scalar y, Scalar z);
};




#endif



























 
 
