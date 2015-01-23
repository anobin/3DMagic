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
/** Selector file for Matrix4 header file
 *
 * @file Matrix4.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MATRIX4_SELECTOR_H
#define MAGIC3D_MATRIX4_SELECTOR_H


// include the actual Matrix4 based on config

// generic (portable) implementation
#ifdef M3D_MATH_USE_GENERIC
#include "Generic/Matrix4.h"


// intel processors only implementation
#elif defined(M3D_MATH_USE_INTEL)
#include "Intel/Matrix4.h"


// nothing is selected, not valid for math interface
#else
#error "No Math Implementation is selected"

/* the following is provided as documentation of the types that need to
 * be defined for the Math interface 
 */
#include "MathTypes.h"

/** Represents a 4-component (x,y,z,w) matrix. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Matrix4
{
public:
    void createPerspectiveMatrix(Scalar fov, Scalar aspect, Scalar zMin, Scalar zMax);

    void createOrthographicMatrix(Scalar xMin, Scalar xMax, Scalar yMin, Scalar yMax, Scalar zMin, Scalar zMax);

    void createRotationMatrix(Scalar angle, Scalar x, Scalar y, Scalar z);

    void createTranslationMatrix(Scalar x, Scalar y, Scalar z);

    void extractRotation(Matrix3& out);

    Matrix4();

    Matrix4(const Matrix4 &copy);
    
    void set(const Matrix4 &copy);

    void set(unsigned int col, unsigned int row, Scalar value);

    Scalar get(unsigned int col, unsigned int row) const ;

    void setColumn(unsigned int col, const Vector4 &v);

    void getColumn(unsigned int col, Vector4& out) const ;

    void createScaleMatrix(Scalar x, Scalar y, Scalar z);

    void multiply(const Matrix4 &m);

    void multiply(const Matrix4 &m1, const Matrix4 &m2);
    
    template<class T>
    void getArray(T* array) const;
};




#endif // end of selector branch

#endif



























 
 
 
