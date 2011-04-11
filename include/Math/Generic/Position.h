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
/** Header file for Position Generic implementation
 *
 * @file Position.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_POSITION_GENERIC_H
#define MAGIC3D_POSITION_GENERIC_H

// for Scalar
#include "MathTypes.h"

// for lots of stuff
#include <math.h>

// for math classes
#include "Matrix3.h"
#include "Matrix4.h"
#include "Point3.h"
#include "Vector3.h"

/** Represents a 3D position using a location and directional vectors. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Position
{
private:
    /// location
    Point3 location;
    /// forward vector
    Vector3 forward;
    /// up vector
    Vector3 up;
    
public:
    /// default constructor, start at (0,0,0) looking down negative z 
    inline Position(): location(0,0,0), forward(0,0,-1), up(0,1,0) {}

    /// standard constructor
    inline Position(Scalar x, Scalar y, Scalar z): location(x,y,z), 
        forward(0,0,-1), up(0,1,0) {}

    /// copy constructor
    inline Position(const Position &copy): location(copy.location),
        forward(copy.forward), up(copy.up) {}

    /// copy setter
    inline void set(const Position &copy)
    {
        location.set(copy.location);
        forward.set(copy.forward);
        up.set(copy.up);
    }

    /// get location
    inline const Point3 &getLocation() const {return location;}

    /// get location
    inline Point3 &getLocation() {return location;}

    /// get forward vector
    inline const Vector3 &getForwardVector() const {return forward;}

    /// get forward vector
    inline Vector3 &getForwardVector() {return forward;}

    /// get up vector
    inline const Vector3 &getUpVector() const {return up;}

    /// get up vector
    inline Vector3 &getUpVector() {return up;}

    /// get the local z axis
    inline const Vector3 &getLocalZAxis() const {return forward;}

    /// get the local y axis
    inline const Vector3 &getLocalYAxis() const {return up;}

    /// get the local x axis
    inline void getLocalXAxis(Vector3 &x) const {return x.crossProduct(up, forward);}
    
    /// translate the location along the orthonormal axis in real world coordinates
    inline void translate(Scalar x, Scalar y, Scalar z)
    {
        location.translate(x, y, z);
    }

    /** Translate the location in local coordinates (forward vector is negative
     * z, up vector is positive y, and cross is positive x)
     */
    inline void translateLocal(Scalar x, Scalar y, Scalar z)
    {
        Vector3 cross;
        this->getLocalXAxis(cross);
        // translate along each of the local axis for the ammount specified
        location.translate( 
            (cross.getX()*x) + (forward.getX()*z) + (up.getX()*y),
            (cross.getY()*x) + (forward.getY()*z) + (up.getY()*y),
            (cross.getZ()*x) + (forward.getZ()*z) + (up.getZ()*y)
        );
    }

    /// get a transformation matrix that represents this position
    void getTransformMatrix(Matrix4& out);

    /// get a rotation matrix that represents the orientation of this position
    void getRotationMatrix(Matrix3& out);

    /// get a matrix that can be used to move objects around the camera
    void getCameraMatrix(Matrix4& out);

    /// rotate this position in world coordinates
    void rotate(Scalar angle, const Vector3 &axis);

    /// rotate this position in local coordinates
    void rotateLocal(Scalar angle, const Vector3 &axis);

    /// normalize this position
    void normalize();
};



#endif



























 
 
 
 
