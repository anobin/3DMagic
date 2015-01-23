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
/** Header file for Point2 Generic implementation
 *
 * @file Point2.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_POINT2_GENERIC_H
#define MAGIC3D_POINT2_GENERIC_H

// for Scalar
#include "MathTypes.h"

// for sqrt
#include <math.h>


/** Represents a 2-component (x,y) point
 */
class Point2
{
private:
    /// x component
    Scalar x;
    /// y component
    Scalar y;
    
public:
    /// default constructor, set to (0,0)
    inline Point2(): x(0), y(0) {}

    /// standard constructor
    inline Point2(Scalar x, Scalar y): x(x), y(y) {}

    /// copy constructor
    inline Point2(const Point2 &copy): x(copy.x), y(copy.y) {}

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

    /// get the distance to another point
    inline Scalar getDistance(const Point2 &p) const
    {
        Scalar tmp[2];
        tmp[0] = x - p.x;
        tmp[1] = y - p.y;
        tmp[0] *= tmp[0];
        tmp[1] *= tmp[1];
        return sqrt(tmp[0] + tmp[1]);
    }

    /// set the contents
    inline void set(Scalar x, Scalar y)
    {
        this->x = x;
        this->y = y;
    }

    /// set contents to another vector
    inline void set(const Point2 &copy)
    {
        this->x = copy.x;
        this->y = copy.y;
    }
    
};




#endif



























 
 
 
 
