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
/** Header file for Matrix4 Generic Implementation
 *
 * @file Matrix4.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MATRIX4_GENERIC_H
#define MAGIC3D_MATRIX4_GENERIC_H

// for Scalar
#include "MathTypes.h"

// for Vector4 and Matrix3
#include "Vector.h"
#include "Matrix3.h"

// for a lot of stuff
#define _USE_MATH_DEFINES
#include <math.h>

// for memcpy
#include <string.h>


/** Represents a 4x4-component (x,y,z,w) matrix
 */
class Matrix4
{
private:
    /// matrix data, column major
    Scalar data[4*4];
    
    /// the identity matrix
    static const Scalar identity[];
    
public:
    /// default constructor, load identity
    inline Matrix4()
    {
        memcpy(this->data, Matrix4::identity, sizeof(Scalar)*4*4);
    }

    /// copy constructor
    inline Matrix4(const Matrix4 &copy)
    {
        memcpy(this->data, copy.data, sizeof(Scalar)*4*4);
    }

    inline Matrix4(const Matrix3& matrix)
    {
        memcpy(this->data, Matrix4::identity, sizeof(Scalar) * 4 * 4);

        memcpy(data, matrix.data, sizeof(Scalar) * 3);
        memcpy(data + 4, matrix.data + 3, sizeof(Scalar) * 3);
        memcpy(data + 8, matrix.data + 6, sizeof(Scalar) * 3);
    }
    
    /// copy setter
    inline void set(const Matrix4 &copy)
    {
        memcpy(this->data, copy.data, sizeof(Scalar)*4*4);
    }

    /// set a element
    inline void set(unsigned int col, unsigned int row, Scalar value)
    {
        data[(col*4)+row] = value;
    }

    /// get a element
    inline Scalar get(unsigned int col, unsigned int row) const
    {
        return data[(col*4)+row];
    }

    /// set a column
    inline void setColumn(unsigned int col, const Vector4 &v)
    {
        data[col*4  ] = v.x();
        data[col*4+1] = v.y();
        data[col*4+2] = v.z();
        data[col*4+3] = v.w();
    }

    /// get a column
    inline Vector4 getColumn(unsigned int col) const
    {
        return Vector4(
			data[col*4  ],
			data[col*4+1],
			data[col*4+2],
			data[col*4+3]
		);
    }
    
    inline const Scalar* getArray() const
    {
        return data;
    }
    
    /// create a scale matrix
    void createScaleMatrix(Scalar x, Scalar y, Scalar z);

    /// multiply this matrix and another matrix
    void multiply(const Matrix4 &m);

    /// multiply two other matrixes and store the result in this matrix
    void multiply(const Matrix4 &m1, const Matrix4 &m2);
    
    /// create a perepective matrix
    void createPerspectiveMatrix(Scalar fov, Scalar aspect, Scalar zMin, Scalar zMax);

    /// create a orthographic matrix
    void createOrthographicMatrix(Scalar xMin, Scalar xMax, Scalar yMin, Scalar yMax, Scalar zMin, Scalar zMax);

    /// create rotation matrix
    void createRotationMatrix(Scalar angle, Scalar x, Scalar y, Scalar z);

    /// create a translation matrix
    void createTranslationMatrix(Scalar x, Scalar y, Scalar z);

    /// extract the rotational component out of this matrix
    void extractRotation(Matrix3& out);

	Matrix4 inverse() const;

	Scalar determinant() const;

	Matrix4 transpose() const;

	Matrix4 cofactor() const;
};




#endif



























 
 
 
