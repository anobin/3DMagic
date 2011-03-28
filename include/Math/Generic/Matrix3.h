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
/** Header file for Matrix3 Generic implementation
 *
 * @file Matrix3.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MATRIX3_GENERIC_H
#define MAGIC3D_MATRIX3_GENERIC_H

// for Scalar
#include "MathTypes.h"

// for Vector3
#include "Vector3.h"

// for a lot of stuff
#include <math.h>

// for memcpy
#include <string.h>


/** Represents a 3x3-component (x,y,z) matrix. 
 */
class Matrix3
{
private:
    /// the matrix data, column major
    Scalar data[3*3];
    
    /// the identity matrix
    static const Scalar identity[];
    
public:
    /// default constructor, load identity
    inline Matrix3()
    {
        memcpy(this->data, Matrix3::identity, sizeof(Scalar)*3*3);
    }

    /// copy constructor
    inline Matrix3(const Matrix3 &copy)
    {
        memcpy(this->data, copy.data, sizeof(Scalar)*3*3);
    }

    /// copy setter
    inline void set(const Matrix3 &copy)
    {
        memcpy(this->data, copy.data, sizeof(Scalar)*3*3);
    }

    /// get a specific element
    inline Scalar get(unsigned int col, unsigned int row) const
    {
        return data[col*row];
    }

    /// set a specific element
    inline void set(unsigned int col, unsigned int row, Scalar value)
    {
        data[col*row] = value;
    }

    /// set a column
    inline void setColumn(unsigned int col, const Vector3 &v)
    {
        data[col*3]   = v.getX();
        data[col*3+1] = v.getY();
        data[col*3+2] = v.getZ();
    }

    /// get a column
    inline void getColumn(unsigned int col, Vector3& out) const
    {
        out.setX(data[col*3]  );
        out.setY(data[col*3+1]);
        out.setZ(data[col*3+2]);
    }
    
    template<class T>
    void getArray(T* array) const
    {
        for (int i=0; i < 3*3; i++)
            array[i] = (T)data[i];
    }

    /// multiply this matrix and another
    void multiply(const Matrix3 &m);

    /// multiply two matrixes and store the result in this matrix
    void multiply(const Matrix3 &m1, const Matrix3 &m2);

    /// turn this matrix into a scale matrix
    void createScaleMatrix(Scalar x, Scalar y, Scalar z);

    /// turn this matrix into a rotation matrix
    void createRotationMatrix(Scalar angle, Scalar x, Scalar y, Scalar z);
};




#endif



























 
 
