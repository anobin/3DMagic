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
/** Header file for Position class
 *
 * @file Position.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_POSITION_H
#define MAGIC3D_POSITION_H

#include "../Math/Point.h"
#include "../Math/Vector.h"
#include "../Math/Matrix.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

namespace Magic3D
{


/** Represents a 3D position in space
 * with a 3D location, forward vector, and 
 * up vector
 */
class Position
{
protected:
	/// location
	Point3f location;
	/// forward unit vector
	Vector3f forward;
	/// up unit vector
	Vector3f up;
	
public:
	/// default constructor, start at (0,0,0) looking down negative z
	inline Position(): location(0.0f, 0.0f, 0.0f), forward(0.0f, 0.0f, -1.0f),
					up(0.0f, 1.0f, 0.0f) {/* intentionally left blank */}
	
	/** standard constructor for specifying all parts
	 * @param location the starting location
	 * @param forward the starting forward vector
	 * @param up the starting up vector
	 */
	inline Position(const Point3f& location, const Vector3f& forward, const Vector3f& up):
		location(location), forward(forward), up(up) {/* intetionally left blank */}
	
	/** standard constructor for specifying only the location
	 * @param x starting x location
	 * @param y starting y location
	 * @param z starting z location
	 */
	inline Position(float x, float y, float z): location(x, y, z), 
		forward(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f) {/* intentionally left blank */}
	
	/// copy constructor
	inline Position(const Position& copy): location(copy.location), 
		forward(copy.forward), up(copy.up) {/* intentionally left blank */}
		
	/// set to another position
	inline void set(const Position& copy)
	{
		location.set(copy.location);
		forward.set(copy.forward);
		up.set(copy.up);
	}

	/// destructor
	virtual ~Position();


	/// set location by copy
	inline void setLocation(const Point3f& loc) { location.set(loc); }
	/// set location by components
	inline void setLocation(float x, float y, float z) { location.set(x,y,z); }
	/// get const location
	inline const Point3f& getLocation() const { return location; }
	/// get location for modifications
	inline Point3f& getLocation() { return location; }
	
	/// set forward vector by copy
	inline void setForwardVector(const Vector3f& forward) { this->forward.set(forward); }
	/// set forward vector by components
	inline void setForwardVector(float x, float y, float z) { this->forward.set(x,y,z); }
	/// get const forward vector
	inline const Vector3f& getForwardVector() const { return forward; }
	/// get forward vector for modifications
	inline Vector3f& getForwardVector() { return forward; }
	
	/// set up vector by copy
	inline void setUpVector(const Vector3f& up) { this->up.set(up); }
	/// set up vector by components
	inline void setUpVector(float x, float y, float z) { up.set(x,y,z); }
	/// get const up vector
	inline Vector3f& getUpVector() { return up; }
	
	// get local axises
	inline const Vector3f& getLocalZAxis() { return forward; }
	inline const Vector3f& getLocalYAxis() { return up; }
	inline void getLocalXAxis(Vector3f& x) { return x.crossProduct(up, forward); }


	/** Translate the location along orthonormal axis in
	 * real world coordinates
	 */
	inline void translate(float x, float y, float z)
		{ location.translate(x,y,z); }
		
	/** Translate the location in local coordinates (forward vector is 
	 * negative z, up vector is positive y, and cross is  positive x)
	 */
	inline void translateLocal(float x, float y, float z)
	{ 
		Vector3f cross;
		this->getLocalXAxis(cross);
		// translate along each of the local axis for the ammount specified
		location.translate( 
			(cross.getX()*x) + (forward.getX()*z) + (up.getX()*y),
			(cross.getY()*x) + (forward.getY()*z) + (up.getY()*y),
			(cross.getZ()*x) + (forward.getZ()*z) + (up.getZ()*y)
		);
	}
	
	
	/** Get the transformation matrix for this position
	 * @param matrix place to place matrix in
	 */
	void getTransformationMatrix(Matrix44f& matrix);
	void getRotationMatrix(Matrix44f& matrix);
	void getRotationMatrix(Matrix33f& matrix);
	
	/** Get the rotation matrix into a 3x3 matrix
	 * @param matrix out parameter to place matrix data into
	 * @note this method is provided to ease performance interactions with 
	 * the physics library
	 */
	void getRowMajorRotationMatrix(btMatrix3x3& matrix);

	/** Set the orientation (forward and up vectors) to the
	 * provided rotation matrix
	 * @param matrix matrix to use to set new orientation
	 * @note this method is provided to ease performance interactions with 
	 * the physics library
	 */
	void setRowMajorRotationMatrix(const btMatrix3x3& matrix);
	
	
	/** Get a reversed transformation matrix to be used for cameras
	 * @param matrix place to place matrix in
	 */
	void getCameraTransformationMatrix(Matrix44f& matrix);
	void getCameraRotationMatrix(Matrix44f& matrix);
	
	/** Rotate around in world coordinates
	 * @param angle the angle to rotate in radians
	 * @param axis the vector to rotate around
	 */
	void rotate(float angle, const Vector3f& axis);
	
	/** Rotate around in local coordinates
	 * @param angle the angle to rotate in radians
	 * @param axis the vector rotate around
	 */
	void rotateLocal(float angle, const Vector3f& axis);
	

	/// Normalize so that axes are orthonormal
	void normalize();

};



};






#endif





