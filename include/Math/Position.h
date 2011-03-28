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
/** Selector file for Position header file
 *
 * @file Position.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_POSITION_SELECTOR_H
#define MAGIC3D_POSITION_SELECTOR_H


// include the actual Position based on config

// generic (portable) implementation
#ifdef M3D_MATH_USE_GENERIC
#include "Generic/Position.h"


// intel processors only implementation
#elif defined(M3D_MATH_USE_INTEL)
#include "Intel/Position.h"


// nothing is selected, not valid for math interface
#else
#error "No Math Implementation is selected"

/* the following is provided as documentation of the types that need to
 * be defined for the Math interface 
 */
#include "MathTypes.h"

/** Represents a 3D position using a location and directional vectors. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Position
{
public:
    Position();

    Position(Scalar x, Scalar y, Scalar z);

    Position(const Position &copy);

    void set(const Position &copy);

    void translate(Scalar x, Scalar y, Scalar z);

    void translateLocal(Scalar x, Scalar y, Scalar z);

    void getTransformMatrix(Matrix4& out);

    void getRotationMatrix(Matrix3& out);

    void getCameraMatrix(Matrix4& out);

    void rotate(Scalar angle, const Vector3 &axis);

    void rotateLocal(Scalar angle, const Vector3 &axis);

    void normalize();

    Point3 &getLocation() const ;

    Point3 &getLocation();

    Vector3 &getForwardVector() const ;

    Vector3 &getFowardVector();

    Vector3 &getUpVector() const ;

    Vector3 &getUpVector();

    Vector3 &getLocalZAxis() const ;

    Vector3 &getLocalYAxis() const ;

    void getLocalXAxis(Vector3 &x) const ;
};




#endif // end of selector branch

#endif



























 
 
 
 
