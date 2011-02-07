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
/** header file for MotionState class
 *
 * @file MotionState.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MOTION_STATE_H
#define MAGIC3D_MOTION_STATE_H

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

namespace Magic3D
{
	
// forward declaration of position
class Position;
	
/** Used in listener callback pattern with
 * physics library to automatically keep the
 * physical location of an object up to date with
 * graphical location.
 */
class MotionState : public ::btMotionState
{
private:
	/// reference to position to sync with
	Position* position;
	
	/// default constructor
	inline MotionState(): position(NULL) {}
	
public:
	/** Standard constructor
	 * @param position the position to keep in sync
	 */
	inline MotionState(Position& position): position(&position) {}
	
	/// destructor
	virtual ~MotionState();
	
	/** get the world tansform of the linked position
	 * @param worldTrans out parameter to palce transform in
	 * @warning this method is called by physics library and should
	 * not be called directly
	 */
	virtual void getWorldTransform (btTransform &worldTrans) const;
	
	/** set the world transform of the linked position
	 * @param worldTrans the transform to set the position to
	 * @warning this method is called by physics library and should
	 * not be called directly
	 */
	virtual void setWorldTransform (const btTransform &worldTrans);
	
	
	
};	
	
	
};









#endif






