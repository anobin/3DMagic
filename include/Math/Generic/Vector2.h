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
/** Header file for Vector2 Generic implementation
 *
 * @file Vector2.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VECTOR2_GENERIC_H
#define MAGIC3D_VECTOR2_GENERIC_H

// for Scalar type
#include "MathTypes.h"

/** Represents a 2-component (x,y) vector
 */
class Vector2
{
private:
    /// x component of vector
    Scalar x;
    /// y component of vector
    Scalar y;
    
public:
    /// default constructor, set to (0,0)
    inline Vector2(): x(0), y(0) {}
    
    /// standard constructor
    inline Vector2(Scalar x, Scalar y): x(x), y(y) {}

    /// copy constructor
    inline Vector2(const Vector2 &copy): x(copy.x), y(copy.y) {}

    /// add a vector to this vector
    inline void add(const Vector2 &v)
    {
        x += v.x;
        y += v.y;
    }

    /// add two vectors and store the result in this vector
    inline void add(const Vector2 &v1, const Vector2 &v2)
    {
        x = v1.x + v2.x;
        y = v1.y + v2.y;
    }

    /// subtract a vector from this vector
    inline void subtract(const Vector2 &v)
    {
        x -= v.x;
        y -= v.y;
    }

    /// subtract v2 from v1 and store the result in this vector
    inline void subtract(const Vector2 &v1, const Vector2 &v2)
    {
        x = v1.x - v2.x;
        y = v1.y - v2.y;
    }

    /// scale this vector by a given factor
    inline void scale(Scalar factor)
    {
        x *= factor;
        y *= factor;
    }

    /// set this vector to copy the contents of another vector
    inline void set(const Vector2 &copy)
    {
        x = copy.x;
        y = copy.y;
    }

    /// set the contents of this vector
    inline void set(Scalar x, Scalar y)
    {
        this->x = x;
        this->y = y;
    }

    /// set the x component
    inline void setX(Scalar x)
    {
        this->x = x;
    }

    /// set the y component
    inline void setY(Scalar y)
    {
        this->y = y;
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
    
};







#endif



























 
 
 
