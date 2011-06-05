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
/** Header file for Point3 Intel implementation
 *
 * @file Point3.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_POINT3_INTEL_H
#define MAGIC3D_POINT3_INTEL_H

// TODO
#include "MathTypes.h"

/** Represents a 3-component (x,y,z) point. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Point3
{
public:
    void translate(Scalar x, Scalar y, Scalar z);

    Scalar getDistance(const Point3 &p) const ;

    Point3();

    Point3(Scalar x, Scalar y, Scalar z);

    Point3(const Point3 &copy);

    Scalar getX() const ;

    Scalar getY() const ;

    Scalar getZ() const ;

    void setX(Scalar value);

    void setY(Scalar value);

    void setZ(Scalar value);

    void set(Scalar x, Scalar y, Scalar z);

    void set(const Point3 &copy);
};



#endif



























 
 
 
 
 
