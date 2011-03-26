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
/** Header file for Point2 Intel implementation
 *
 * @file Point2.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_POINT2_INTEL_H
#define MAGIC3D_POINT2_INTEL_H

// TODO
#error "Intel Math Implementation is not yet implemented"

#include "MathTypes.h"

/** Represents a 2-component (x,y) point. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Point2
{
public:
    Point2();

    Point2(Scalar x, Scalar y);

    Point2(const Point2 &copy);

    Scalar getX() const ;

    Scalar getY() const ;

    void setX(Scalar value);

    void setY(Scalar value);

    Scalar getDistance(const Point2 &p) const ;

    void set(Scalar x, Scalar y);

    void set(const Point2 &copy);
};




#endif



























 
 
 
 
