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
/** Implementation file for MotionState class
 *
 * @file MotionState.cpp
 * @author Andrew Keating
 */

#include <Physics/MotionState.h>

namespace Magic3D
{
	
/// destructor
MotionState::~MotionState()
{
	/* intentionally left blank */
}
	
/** get the world tansform of the linked position
 * @param worldTrans out parameter to palce transform in
 * @warning this method is called by physics library and should
 * not be called directly
 */
void MotionState::getWorldTransform(btTransform &worldTrans) const
{
	// set the location/origin
	Point3& l = this->position->getLocation();
	worldTrans.setOrigin (btVector3(l.getX(), l.getY(), l.getZ()));
	
	// set the basis/rotational matrix
	// since openGL matrix is column major and Bullet is row
	// major, we have to do some converting
    // TODO
	/*btMatrix3x3 matrix;
	this->position->getRotationMatrix(matrix);
	worldTrans.setBasis(matrix);*/
}
	
/** set the world transform of the linked position
 * @param worldTrans the transform to set the position to
 * @warning this method is called by physics library and should
 * not be called directly
 */
void MotionState::setWorldTransform (const btTransform &worldTrans)
{
	// set the location/origin
	const btVector3& location = worldTrans.getOrigin();
	this->position->getLocation().set(location.getX(), location.getY(), location.getZ());
	
    // TODO
	// set the basis/rotational matrix
	//this->position->setRowMajorRotationMatrix(worldTrans.getBasis());
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};




