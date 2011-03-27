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
/** Header file for Vector4 Generic implementation
 *
 * @file Vector4.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VECTOR4_GENERIC_H
#define MAGIC3D_VECTOR4_GENERIC_H

// for Scalar
#include "MathTypes.h"

/** Represents a 4-component (x,y,z,w) vector
 */
class Vector4
{
private:
    /// x component
    Scalar x;
    /// y component 
    Scalar y;
    /// z/depth component
    Scalar z;
    /// w/scale component
    Scalar w;
    
public:
    /// default constructor, set to (0,0,0,1)
    inline Vector4(): x(0), y(0), z(0), w(1) {}

    /// standard constructor, w optional
    inline Vector4(Scalar x, Scalar y, Scalar z, Scalar w = 1.0)
        : x(x), y(y), z(z), w(w) {}

    /// copy constructor
    inline Vector4(const Vector4 &copy)
        : x(copy.x), y(copy.y), z(copy.z), w(copy.w) {}

    /// set contents of vector
    inline void set(Scalar x, Scalar y, Scalar z, Scalar w = 1.0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    /// set this vector to the contents of another
    inline void set(const Vector4 &copy)
    {
        x = copy.x;
        y = copy.y;
        z = copy.z;
        w = copy.w;
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

    /// set the w component
    inline void setW(Scalar value)
    {
        w = value;
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

    /// get the w component
    inline Scalar getW() const
    {
        return w;
    }
    
    /// add a vector to this vector
    inline void add(const Vector4 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
    }

    /// add two vectors and store the result in this vector
    inline void add(const Vector4 &v1, const Vector4 &v2)
    {
        x = v1.x + v2.x;
        y = v1.y + v2.y;
        z = v1.z + v2.z;
        w = v1.w + v2.w;
    }

    /// subtract a vector from this vector
    inline void subtract(const Vector4 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
    }

    /// subtract v2 from v1 and store the result in this vector
    inline void subtract(const Vector4 &v1, const Vector4 &v2)
    {
        x = v1.x - v2.x;
        y = v1.y - v2.y;
        z = v1.z - v2.z;
        w = v1.w - v2.w;
    }

    /// scale this vector by a given factor
    inline void scale(Scalar factor)
    {
        x *= factor;
        y *= factor;
        z *= factor;
        w *= factor;
    }
    
};



#endif



























 
