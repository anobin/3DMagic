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
/** Header file for Vector3 Intel implementation
 *
 * @file Vector3.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_VECTOR3_INTEL_H
#define MAGIC3D_VECTOR3_INTEL_H

#include "MathTypes.h"
#include "Matrix4.h"

/** Represents a 3-component (x,y,z) vector. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Vector3
{
public:
    void crossProduct(const Vector3 &v1);

    void crossProduct(const Vector3 &v1, const Vector3 &v2);

    void dotProduct(const Vector3 &v1);

    void dotProduct(const Vector3 &v1, const Vector3 &v2);

    Scalar angleBetween(const Vector3 &v) const ;

    Scalar getLength() const ;

    void normalize();

    void transform(const Matrix4 &m);

    void transform(const Vector3 &v, const Matrix4 &m);

    void rotate(const Matrix3 &m);

    void rotate(const Vector3 &v, const Matrix3 &m);

    Vector3();

    Vector3(Scalar x, Scalar y, Scalar z);

    Vector3(const Vector3 &copy);

    void set(Scalar x, Scalar y, Scalar z);

    void set(const Vector3 &copy);

    void setX(Scalar value);

    void setY(Scalar value);

    void setZ(Vector3 value);

    Scalar getX() const ;

    Scalar getY() const ;

    Scalar getZ() const ;
};



#endif



























 
 
