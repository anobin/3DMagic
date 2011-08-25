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
/** Header file for PhysicalBody class
 *
 * @file PhysicalBody.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_PHYSICAL_BODY_H
#define MAGIC3D_PHYSICAL_BODY_H

#include "../Math/Position.h"
#include "../Exceptions/MagicException.h"
#include "MotionState.h"
#include "../Util/magic_assert.h"
#include "../CollisionShapes/CollisionShape.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

namespace Magic3D
{


/** Physical body for an object, contains the physics properties 
 * of the object (collision shape, physics position, mass, friction, etc.).
 */
class PhysicalBody
{
protected:
	/// MotionState for physical body
	MotionState motionState;
	
	/// physical body for object
	btRigidBody* body;
	
	inline void build(float mass, btCollisionShape* shape, float friction = 0.5f, 
	    float restitution = 0.0f, float linearDamping = 0.0f)
	{   
		// calc inertia
		btVector3 fallInertia(0,0,0);
        if (mass != 0.0f)
		    shape->calculateLocalInertia(mass,fallInertia);
		
		// construct rigid body
		btRigidBody::btRigidBodyConstructionInfo 
			fallRigidBodyCI(mass, &motionState, shape, fallInertia);
		fallRigidBodyCI.m_friction = friction;
		fallRigidBodyCI.m_restitution = restitution;
		fallRigidBodyCI.m_linearDamping = linearDamping;
		body = new btRigidBody(fallRigidBodyCI);
	}
	
public:
	/// standard constructor
	inline PhysicalBody(Position& position, CollisionShape& shape, float mass): 
	    motionState(position), body(NULL) 
	{ 
	    build(mass, shape.getShape()); 
	}
	
	/// destructor
	virtual ~PhysicalBody();

	/** sync the graphical position with the physical
	 * position.
	 * @note call this after manually setting the position,
	 * this method does not need to be called for the physics
	 * to update the position, only the other way around
	 * @note try to avoid manually setting an object's position
	 * in the first place, use forces and constraints where possible
	 */
	inline void syncPositionToPhysics()
	{
		// position is already set, so we can get what the transform
		// should be from our motions state.	
		// we have to do this because bullet only gets the world
		// transform from our motion state when the body is created
		
		// sync out position and the rigid body center of mass
		btTransform transform;
		motionState.getWorldTransform(transform);
		body->setCenterOfMassTransform(transform);
		
		// tell bullet that the rigid body needs some attention
		body->activate();
	}
	
	/// get the rigid body for this object
	inline btRigidBody* getRigidBody()
	{
		return body;
	}
	

};





};



#endif





















