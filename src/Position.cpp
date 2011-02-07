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
/** Implementation file for Position class
 *
 * @file Position.cpp
 * @author Andrew Keating
 */

#include <Position.h>

namespace Magic3D
{
	
/// destructor
Position::~Position()
{
	/* intentionally left blank */
}
	
	
/** Get the transformation matrix for this position
 * @param matrix place to place matrix in
 */
void Position::getTransformationMatrix(Matrix44f& matrix)
{
	// get local x axis
	Vector3f xAxis;
	xAxis.crossProduct(up, forward);
	
	float* data = (float*)&matrix.data;

	// fill in x axis, first column
	data[0] = xAxis.getX();
	data[1] = xAxis.getY();
	data[2] = xAxis.getZ();
	data[3] = 0.0f;
	
	// fill in y axis, second column
	data[4] = up.getX();
	data[5] = up.getY();
	data[6] = up.getZ();
	data[7] = 0.0f;       
							
	// fill in z axis, thrid column
	data[8] = forward.getX();
	data[9] = forward.getY();
	data[10] = forward.getZ();
	data[11] = 0.0f;

	// fill in translation, fourth column
	data[12] = location.getX();
	data[13] = location.getY();
	data[14] = location.getZ();
	data[15] = 1.0f; // W component is always 1, duh..
}

void Position::getRotationMatrix(Matrix44f& matrix)
{
	// get local x axis
	Vector3f xAxis;
	xAxis.crossProduct(up, forward);
	
	float* data = (float*)&matrix.data;

	// fill in x axis, first column
	data[0] = xAxis.getX();
	data[1] = xAxis.getY();
	data[2] = xAxis.getZ();
	data[3] = 0.0f;
	
	// fill in y axis, second column
	data[4] = up.getX();
	data[5] = up.getY();
	data[6] = up.getZ();
	data[7] = 0.0f;       
							
	// fill in z axis, thrid column
	data[8] = forward.getX();
	data[9] = forward.getY();
	data[10] = forward.getZ();
	data[11] = 0.0f;

	// fill in null translation, fourth column
	data[12] = 0.0f;
	data[13] = 0.0f;
	data[14] = 0.0f;
	data[15] = 1.0f; // W component is always 1, duh..
}

/** Get the rotation matrix into a 3x3 matrix
 * @param matrix out parameter to place matrix data into
 * @note this method is provided to ease performance interactions with 
 * the physics library
 */
void Position::getRowMajorRotationMatrix(btMatrix3x3& matrix)
{
	// get local x axis
	Vector3f xAxis;
	xAxis.crossProduct(up, forward);

	matrix.setValue (
	
		// fill in x axis, first column
		xAxis.getX(),
		xAxis.getY(),
		xAxis.getZ(),
		
		// fill in y axis, second column
		up.getX(),
		up.getY(),
		up.getZ(),
								
		// fill in z axis, thrid column
		forward.getX(),
		forward.getY(),
		forward.getZ()
	
	);
}

/** Set the orientation (forward and up vectors) to the
 * provided rotation matrix
 * @param matrix matrix to use to set new orientation
 * @note this method is provided to ease performance interactions with 
 * the physics library
 */
void Position::setRowMajorRotationMatrix(const btMatrix3x3& matrix)
{
	btVector3 upV = matrix.getRow(1);
	btVector3 forwardV = matrix.getRow(2);
		
	// fill in y axis, second row
	up.set(upV.getX(), upV.getY(), upV.getZ());
							
	// fill in z axis, thrid row
	forward.set(forwardV.getX(), forwardV.getY(), forwardV.getZ());
	
}

/** Get the row major rotation matrix into a 3x3 matrix
 * @param matrix out parameter to place matrix data into
 */
void Position::getRotationMatrix(Matrix33f& matrix)
{
	// get local x axis
	Vector3f xAxis;
	xAxis.crossProduct(up, forward);
	
	float* data = (float*)&matrix.data;

	// fill in x axis, first column
	data[0] = xAxis.getX();
	data[1] = xAxis.getY();
	data[2] = xAxis.getZ();
	
	// fill in y axis, second column
	data[3] = up.getX();
	data[4] = up.getY();
	data[5] = up.getZ();
							
	// fill in z axis, thrid column
	data[6] = forward.getX();
	data[7] = forward.getY();
	data[8] = forward.getZ();
}



/** Get a reversed transformation matrix to be used for cameras
 * @param matrix place to place matrix in
 */
void Position::getCameraTransformationMatrix(Matrix44f& matrix)
{			
	// z is reversed
	Vector3f z(-forward.getX(), -forward.getY(), -forward.getZ());

	// get x vector
	Vector3f x;
	x.crossProduct(up, z);

	// matrix is transposed (rows instead of columns)
	#define MAGIC3D_A(row,col)  matrix.data[col*4+row]
		MAGIC3D_A(0, 0) = x.data[0];
		MAGIC3D_A(0, 1) = x.data[1];
		MAGIC3D_A(0, 2) = x.data[2];
		MAGIC3D_A(0, 3) = 0.0;
		MAGIC3D_A(1, 0) = up.data[0];
		MAGIC3D_A(1, 1) = up.data[1];
		MAGIC3D_A(1, 2) = up.data[2];
		MAGIC3D_A(1, 3) = 0.0;
		MAGIC3D_A(2, 0) = z.data[0];
		MAGIC3D_A(2, 1) = z.data[1];
		MAGIC3D_A(2, 2) = z.data[2];
		MAGIC3D_A(2, 3) = 0.0;
		MAGIC3D_A(3, 0) = 0.0;
		MAGIC3D_A(3, 1) = 0.0;
		MAGIC3D_A(3, 2) = 0.0;
		MAGIC3D_A(3, 3) = 1.0;
	#undef MAGIC3D_A
		
	// apply reversed translation
	Matrix44f trans;
	trans.createTranslationMatrix(-location.getX(), -location.getY(), -location.getZ());
	matrix.multiply(trans);
}
	
void Position::getCameraRotationMatrix(Matrix44f& matrix)
{
	// z is reversed
	Vector3f z(-forward.getX(), -forward.getY(), -forward.getZ());

	// get x vector
	Vector3f x;
	x.crossProduct(up, z);

	// matrix is transposed (rows instead of columns)
	#define MAGIC3D_A(row,col)  matrix.data[col*4+row]
		MAGIC3D_A(0, 0) = x.data[0];
		MAGIC3D_A(0, 1) = x.data[1];
		MAGIC3D_A(0, 2) = x.data[2];
		MAGIC3D_A(0, 3) = 0.0;
		MAGIC3D_A(1, 0) = up.data[0];
		MAGIC3D_A(1, 1) = up.data[1];
		MAGIC3D_A(1, 2) = up.data[2];
		MAGIC3D_A(1, 3) = 0.0;
		MAGIC3D_A(2, 0) = z.data[0];
		MAGIC3D_A(2, 1) = z.data[1];
		MAGIC3D_A(2, 2) = z.data[2];
		MAGIC3D_A(2, 3) = 0.0;
		MAGIC3D_A(3, 0) = 0.0;
		MAGIC3D_A(3, 1) = 0.0;
		MAGIC3D_A(3, 2) = 0.0;
		MAGIC3D_A(3, 3) = 1.0;
	#undef MAGIC3D_A
}


/** Rotate around in world coordinates
 * @param angle the angle to rotate in radians
 * @param axis the vector to rotate around
 */
void Position::rotate(float angle, const Vector3f& axis)
{
	// create rotation matrix
	Matrix44f rotMat;
	rotMat.createRotationMatrix(angle, axis.getX(), axis.getY(), axis.getZ());

	// transform the up vector
	up.transform(rotMat);

	// transform the forward vector
	forward.transform(rotMat);
}
	
/** Rotate around in local coordinates
 * @param angle the angle to rotate in radians
 * @param axis the vector rotate around
 */
void Position::rotateLocal(float angle, const Vector3f& axis)
{
	// transform the rotation axis to world coordinates
	Matrix44f rotMat;
	this->getRotationMatrix(rotMat);
	Vector3f worldAxis(axis);
	worldAxis.transform(rotMat);
	
	// perform rotation
	this->rotate(angle, worldAxis);
}
	
/// Normalize so that axes are orthonormal
void Position::normalize()
{
	// get x axis
	Vector3f cross;
	cross.crossProduct(up, forward);
	
	// recalculate up vector
	up.crossProduct(cross, forward);
	
	// normalize both vectors too
	up.normalize();
	up.normalize();
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};

















