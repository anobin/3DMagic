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

// TODO
#error "Generic Math Implementation is not yet implemented"

#include "MathTypes.h"

/** Represents a 4-component (x,y,z,w) point. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Point4
{
public:
    Point4();

    Point4(Scalar x, Scalar y, Scalar z, Scalar w);

    Point4(const Point4 &copy);

    Scalar getX() const ;

    Scalar getY() const ;

    Scalar getZ() const ;

    Scalar getW() const ;

    void setX(Scalar value);

    void setY(Scalar value);

    void setZ(Scalar value);

    void setW(Scalar value);

    void set(Scalar x, Scalar y, Scalar z, Scalar w);

    void set(const Point4 &copy);
};



#endif



























 
 
 
 
 
 
