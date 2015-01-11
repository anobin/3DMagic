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

class Object;
class PhysicsSystem;

/** Physical body for an object, contains the physics properties 
 * of the object (collision shape, physics position, mass, friction, etc.).
 */
class PhysicalBody
{
public:
    /// properties of physical body
    struct Properties
    {
        /// mass of object, 0 means static object, defaults to 0
        Scalar mass;
        /// friction of object's surface, defaults to 0.5
        Scalar friction;
        /// bouncyness of object, defaults to 0
        Scalar bouncyness;
        
        inline Properties(): mass(0.0f), friction(0.5f), bouncyness(0) {}
    };
    
protected:
    friend class Object;
    friend class PhysicsSystem;
    
	/// MotionState for physical body
	MotionState motionState;
	
	/// physical body for object
	btRigidBody* body;
	
	inline void build(btCollisionShape* shape, const Properties& prop)
	{   
		// calc inertia
		btVector3 fallInertia(0,0,0);
        if (prop.mass != 0.0f)
		    shape->calculateLocalInertia(prop.mass,fallInertia);
		
		// construct rigid body
		btRigidBody::btRigidBodyConstructionInfo 
			fallRigidBodyCI(prop.mass, &motionState, shape, fallInertia);
		fallRigidBodyCI.m_friction = prop.friction;
		fallRigidBodyCI.m_restitution = prop.bouncyness;
		body = new btRigidBody(fallRigidBodyCI);
	}
	
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
	
public:
	/// standard constructor
	inline PhysicalBody(Position& position, CollisionShape& shape, 
	    const Properties& prop = Properties()): 
	    motionState(position), body(NULL) 
	{ 
	    build(shape.getShape(), prop); 
	}
	
	/// destructor
	virtual ~PhysicalBody();
	
	inline void applyForce(Vector3 force, Vector3 origin = Vector3(0.0f,0.0f,0.0f))
    {
        body->applyForce(
            btVector3(force.x(), force.y(), force.z()), 
            btVector3(origin.x(), origin.y(), origin.z())
        );
    }
	

};





};



#endif





















