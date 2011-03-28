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
/** Header file for Vector3 Generic implementation
 *
 * @file Vector3.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VECTOR3_GENERIC_H
#define MAGIC3D_VECTOR3_GENERIC_H

// for Scalar
#include "MathTypes.h"

// for cos, sin, and tan
#include <math.h>

// forware declarations for matrixes
class Matrix4;
class Matrix3;


/** Represents a 3-component (x,y,z) vector
 */
class Vector3
{
private:
    /// x component
    Scalar x;
    /// y component
    Scalar y;
    /// z/depth component
    Scalar z;
    
public:
    /// default constructor, set to (0,0,0)
    inline Vector3(): x(0), y(0), z(0) {}

    /// standard constructor
    inline Vector3(Scalar x, Scalar y, Scalar z): x(x), y(y), z(z) {}

    /// copy constructor
    inline Vector3(const Vector3 &copy): x(copy.x), y(copy.y), z(copy.z) {}

    /// set the contents of this vector
    inline void set(Scalar x, Scalar y, Scalar z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    /// set this vector to the conentes of another
    inline void set(const Vector3 &copy)
    {
        x = copy.x;
        y = copy.y;
        z = copy.z;
    }

    /// set the x component
    inline void setX(Scalar value)
    {
        x = value;
    }

    /// set the y component
    inline void setY(Scalar value)
    {
        y = value;
    }

    /// set the z component
    inline void setZ(Scalar value)
    {
        z = value;
    }

    /// get the x component
    inline Scalar getX() const
    {
        return x;
    }

    /// get the y component
    inline Scalar getY() const
    {
        return y;
    }

    /// get the z component
    inline Scalar getZ() const
    {
        return z;
    }
    
    /// add a vector to this vector
    inline void add(const Vector3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    /// add two vectors and store the result in this vector
    inline void add(const Vector3 &v1, const Vector3 &v2)
    {
        x = v1.x + v2.x;
        y = v1.y + v2.y;
        z = v1.z + v2.z;
    }

    /// subtract a vector from this vector
    inline void subtract(const Vector3 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    /// subtract v2 from v1 and store the result in this vector
    inline void subtract(const Vector3 &v1, const Vector3 &v2)
    {
        x = v1.x - v2.x;
        y = v1.y - v2.y;
        z = v1.z - v2.z;
    }

    /// scale this vector by a given factor
    inline void scale(Scalar factor)
    {
        x *= factor;
        y *= factor;
        z *= factor;
    }
    
    /// cross product this vector and another
    inline void crossProduct(const Vector3 &v)
    {
        Scalar tmp[3];
        tmp[0] =  y*v.z - v.y*z;
        tmp[1] = -x*v.z + v.x*z;
        tmp[2] =  x*v.y - v.x*y;
        
        x = tmp[0];
        y = tmp[1];
        z = tmp[2];
    }

    /// cross product v1 and v2 and store the result in this vector
    inline void crossProduct(const Vector3 &v1, const Vector3 &v2)
    {
        x =  v1.y*v2.z - v2.y*v1.z;
        y = -v1.x*v2.z + v2.x*v1.z;
        z =  v1.x*v2.y - v2.x*v1.y;
    }

    /// dot product another vector with this vector
    inline Scalar dotProduct(const Vector3 &v) const
    {
        return (x*v.x) + (y*v.y) + (z*v.z);
    }

    /// find the angle between this vector and another
    inline Scalar angleBetween(const Vector3 &v) const
    {
        return acos(this->dotProduct(v));
    }

    /// get the length of this vector
    inline Scalar getLength() const
    {
        return sqrt((x*x)+(y*y)+(z*z));
    }

    /// normalize this vector (turn into unit vector)
    inline void normalize()
    {
        this->scale(Scalar(1.0) / this->getLength());
    }

    /// transform this vector using a matrix
    void transform(const Matrix4 &m);

    /// transform another vector using a matrix and store the result in this vector
    void transform(const Vector3 &v, const Matrix4 &m);

    /// rotate this vector using a matrix
    void rotate(const Matrix3 &m);

    /// rotate another vector using a matrix and store the result in this vector
    void rotate(const Vector3 &v, const Matrix3 &m);
    
};



#endif



























 
 
