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
/** Header file for Point3 Generic implementation
 *
 * @file Point3.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_POINT3_GENERIC_H
#define MAGIC3D_POINT3_GENERIC_H

// for Scalar
#include "MathTypes.h"

// for sqrt
#include <math.h>

// for classes
class Matrix3;
class Matrix4;

/** Represents a 3-component (x,y,z) point.
 */
class Point3
{
private:
    /// x component
    Scalar x;
    /// y component
    Scalar y;
    /// z component
    Scalar z;
    
public:
    /// default constructor, set to (0,0,0)
    inline Point3(): x(0), y(0), z(0) {}

    /// standard constructor
    inline Point3(Scalar x, Scalar y, Scalar z): x(x), y(y), z(z) {}

    /// copy constructor
    inline Point3(const Point3 &copy): x(copy.x), y(copy.y), z(copy.z) {}

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

    /// set the contents
    inline void set(Scalar x, Scalar y, Scalar z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    /// set the contents to another vector
    inline void set(const Point3 &copy)
    {
        x = copy.x;
        y = copy.y;
        z = copy.z;
    }
    
    /// translate by a given ammount
    inline void translate(Scalar x, Scalar y, Scalar z)
    {
        this->x += x;
        this->y += y;
        this->z += z;
    }

    /// get the distance to another point
    inline Scalar getDistance(const Point3 &p) const
    {
        Scalar tmp[3];
        tmp[0] = x - p.x;
        tmp[1] = y - p.y;
        tmp[2] = z - p.z;
        tmp[0] *= tmp[0];
        tmp[1] *= tmp[1];
        tmp[2] *= tmp[2];
        return sqrt(tmp[0] + tmp[1] + tmp[2]);
    }
    
    /// transform this point using a matrix
    void transform(const Matrix4 &m);

    /// transform a point using a matrix and store the results in this point
    void transform(const Point3 &p, const Matrix4 &m);

    /// rotate this point using a matrix
    void rotate(const Matrix3 &m);

    /// rotate a point using a matrix and store the result in this point
    void rotate(const Point3 &p, const Matrix3 &m);
    
};



#endif



























 
 
 
 
 
