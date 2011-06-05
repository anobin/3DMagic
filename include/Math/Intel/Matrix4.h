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
/** Header file for Matrix4 Intel Implementation
 *
 * @file Matrix4.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MATRIX4_INTEL_H
#define MAGIC3D_MATRIX4_INTEL_H

// TODO
#include "MathTypes.h"

class Matrix3;
class Vector4;

/** Represents a 4-component (x,y,z,w) matrix. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Matrix4
{
private:
	ALIGN(16 /*sizeof(Scalar) * 4, or XMM alignment*/, Scalar data[4]);
public:
    void createPerspectiveMatrix(Scalar fov, Scalar aspect, Scalar zMin, Scalar zMax);

    void ceateOrthograhicMatrxi(Scalar xMin, Scalar xMax, Scalar yMin, Scalar yMax, Scalar zMin, Scalar zMax);

    void createRotationMatrix(Scalar angle, Scalar x, Scalar y, Scalar z);

    void createTranslationMatrix(Scalar x, Scalar y, Scalar z);

    void extractRotation(const Matrix3 *out);

    Matrix4();

    Matrix4(const Matrix4 &copy);

    void set(unsigned int col, unsigned int row, Scalar value);

    Scalar get(unsigned int col, unsigned int row) const ;

    void setColumn(unsigned int col, const Vector4 &v);

    void getColumn(unsigned int col, Vector4 *out) const ;
};




#endif



























 
 
 
