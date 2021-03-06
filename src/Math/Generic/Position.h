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
#include "Vector.h"

/** Represents a 3D position using a location and directional vectors. 
 * Note to Implementations: The inline keywords are used here as a
 * recommendation, not a requirement.
 */
class Position
{
private:
    Vector3 location;

    Vector3 forward;
    Vector3 up;
    
public:
    /// default constructor, start at (0,0,0) looking down negative z 
    inline Position(): location(0,0,0), forward(0,0,-1), up(0,1,0) {}

    /// standard constructor
    inline Position(Scalar x, Scalar y, Scalar z): location(x,y,z), 
        forward(0,0,-1), up(0,1,0) {}

	inline Position(const Vector3& location, const Vector3& forward, 
		const Vector3& up): location(location), forward(forward), up(up)
	{
		this->normalize(); // normalize to set the correct up vector
	}

    /// copy constructor
    inline Position(const Position &copy): location(copy.location),
        forward(copy.forward), up(copy.up) {}

    /// copy setter
    inline void set(const Position &copy)
    {
        location = copy.location;
        forward = copy.forward;
        up = copy.up;
    }

    inline const Vector3& getLocation() const {return location;}
    inline void setLocation(const Vector3& v) { this->location = v; }

    inline const Vector3& getForwardVector() const {return forward;}
    inline const Vector3& getUpVector() const {return up;}
    inline Vector3 getRightVector() const { return up * forward; }

    inline const Vector3 &getLocalZAxis() const {return forward;}
    inline const Vector3 &getLocalYAxis() const {return up;}
    inline Vector3 getLocalXAxis() const {return up * forward;}
    
    /// translate the location along the orthonormal axis in real world coordinates
    inline void translate(Scalar x, Scalar y, Scalar z)
    {
        location = location.translate(x, y, z);
    }

    /** Translate the location in local coordinates (forward vector is negative
     * z, up vector is positive y, and cross is positive x)
     */
    inline void translateLocal(Scalar x, Scalar y, Scalar z)
    {
        Vector3 cross = this->getLocalXAxis();
        // translate along each of the local axis for the ammount specified
        location = location.translate( 
            (cross.x()*x) + (forward.x()*z) + (up.x()*y),
            (cross.y()*x) + (forward.y()*z) + (up.y()*y),
            (cross.z()*x) + (forward.z()*z) + (up.z()*y)
        );
    }

    /// get a transformation matrix that represents this position
    void getTransformMatrix(Matrix4& out) const;

    /// get a rotation matrix that represents the orientation of this position
    void getRotationMatrix(Matrix3& out) const;

    /// get a matrix that can be used to move objects around the camera
    void getCameraMatrix(Matrix4& out) const;

    /// rotate this position in world coordinates
    void rotate(Scalar angle, const Vector3 &axis);

    /// rotate this position in local coordinates
    void rotateLocal(Scalar angle, const Vector3 &axis);

    /// normalize this position
    void normalize();
};



#endif



























 
 
 
 
