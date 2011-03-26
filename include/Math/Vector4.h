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
/** Selector file for Vector4 header file
 *
 * @file Vector4.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VECTOR4_SELECTOR_H
#define MAGIC3D_VECTOR4_SELECTOR_H


// include the actual Vector4 based on config

// generic (portable) implementation
#ifdef M3D_MATH_USE_GENERIC
#include "Generic/Vector4.h"


// intel processors only implementation
#elseifdef M3D_MATH_USE_INTEL
#include "Intel/Vector4.h"


// nothing is selected, not valid for math interface
#else
#error "No Math Implementation is selected"

/* the following is provided as documentation of the types that need to
 * be defined for the Math interface 
 */
#include "MathTypes.h"

/** Represents a 4-component (x,y,z,w) vector. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Vector4
{
public:
    Vector4();

    Vector4(Scalar x, Scalar y, Scalar z, Scalar w);

    Vector4(const Vector4 &copy);

    void set(Scalar x, Scalar y, Scalar z, Scalar w);

    void set(const Vector4 &copy);

    void setX(Scalar value);

    void setY(Scalar value);

    void setZ(Scalar value);

    void setW(Scalar value);

    Scalar getX() const ;

    Scalar getY() const ;

    Scalar getZ() const ;

    Scalar getW() const ;
};




#endif // end of selector branch

#endif



























 
