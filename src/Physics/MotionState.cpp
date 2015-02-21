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
	const Vector3& l = this->position->getLocation();
	worldTrans.setOrigin (btVector3(l.x(), l.y(), l.z()));
	
	// set the basis/rotational matrix
	// since openGL matrix is column major and Bullet is row
	// major, we have to do some converting
    
	btMatrix3x3 matrix;
    Vector3 xAxis = this->position->getLocalXAxis();
    const Vector3& up = this->position->getUpVector();
    const Vector3& forward = this->position->getForwardVector();

    matrix.setValue (
    
        // fill in x axis, first column
        xAxis.x(),
        xAxis.y(),
        xAxis.z(),
        
        // fill in y axis, second column
        up.x(),
        up.y(),
        up.z(),
                                
        // fill in z axis, thrid column
        forward.x(),
        forward.y(),
        forward.z()
    
    );
	worldTrans.setBasis(matrix);
}
	
/** set the world transform of the linked position
 * @param worldTrans the transform to set the position to
 * @warning this method is called by physics library and should
 * not be called directly
 */
void MotionState::setWorldTransform (const btTransform &worldTrans)
{
	const btVector3& location = worldTrans.getOrigin();
	const btMatrix3x3& matrix = worldTrans.getBasis();
	btVector3 upV = matrix.getColumn(1);
    btVector3 forwardV = matrix.getColumn(2);                           

	// TODO: clean this up, no need to use 
	(*this->position) = Position(
		Vector3(location.getX(), location.getY(), location.getZ()),
		Vector3(forwardV.getX(), forwardV.getY(), forwardV.getZ()),
		Vector3(upV.getX(), upV.getY(), upV.getZ())
	);
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};




