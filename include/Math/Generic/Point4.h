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
/** Header file for Point4 Generic implementation
 *
 * @file Point4.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_POINT4_GENERIC_H
#define MAGIC3D_POINT4_GENERIC_H

// for Scalar
#include "MathTypes.h"


/** Represents a 4-component (x,y,z,w) point
 */
class Point4
{
private:
    /// x component
    Scalar x;
    /// y component
    Scalar y;
    /// z component
    Scalar z;
    /// w component
    Scalar w;
    
public:
    /// default constructor, set to (0,0,0,1)
    inline Point4(): x(0), y(0), z(0), w(1) {}

    /// stanadard constructor, w optional
    inline Point4(Scalar x, Scalar y, Scalar z, Scalar w = 1.0)
        : x(x), y(y), z(z), w(w) {}

    /// copy constructor
    inline Point4(const Point4 &copy)
        : x(copy.x), y(copy.y), z(copy.z), w(copy.w) {}

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

	inline Scalar get(int i) const
	{
		switch(i)
		{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
		}
		return w;
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

	inline void set(int i, Scalar value)
	{
		switch(i)
		{
			case 0: x = value; break;
			case 1: y = value; break;
			case 2: z = value; break;
			case 3: w = value; break;
		}
	}

    /// set the contents, w optional
    inline void set(Scalar x, Scalar y, Scalar z, Scalar w = 1.0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    /// set the contents to another point
    inline void set(const Point4 &copy)
    {
        this->x = copy.x;
        this->y = copy.y;
        this->z = copy.z;
        this->w = copy.w;
    }

	Point4 transform(const Matrix4 &m) const;
};



#endif



























 
 
 
 
 
 
