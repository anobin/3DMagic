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
/** Header file for Vector2 Intel implementation
 *
 * @file Vector2.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VECTOR2_INTEL_H
#define MAGIC3D_VECTOR2_INTEL_H

// TODO
#error "Intel Math Implementation is not yet implemented"

#include "MathTypes.h"

/** Represents a 2-component (x,y) vector. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Vector2
{
public:
    Vector2(Scalar x, Scalar y);

    Vector2(const Vector2 &copy);

    void add(const Vector2 &v1);

    void add(const Vector2 &v1, const Vector2 &v2);

    void subtract(const Vector2 &v1);

    void subtract(const Vector2 &v1, const Vector2 &v2);

    void scale(Scalar factor);

    Vector2();

    void set(const Vector2 &copy);

    void set(Scalar x, Scalar y);

    void setX(Scalar x);

    void setY(Scalar y);

    Scalar getX() const ;

    Scalar getY() const ;
};





#endif



























 
 
 
