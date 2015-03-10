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
/** Header file for Object class
 *
 * @file Object.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_OBJECT_H
#define MAGIC3D_OBJECT_H

#include "../Math/Position.h"
#include "../Util/Color.h"
#include "../Graphics/Texture.h"
#include "../Exceptions/MagicException.h"
#include "../Graphics/Material.h"
#include <CollisionShapes\CollisionShape.h>
#include <Physics\MotionState.h>
#include <Objects\Model.h>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <set>

namespace Magic3D
{

class World;
class PhysicsSystem;
    
/** Base class for all objects 
 */
class Object
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
    friend class World;
	friend class PhysicsSystem;
    
	Position position;
	
	std::shared_ptr<Model> model;

	MotionState motionState;
	btRigidBody* body;


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
		if (body == nullptr)
			return;
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
	inline Object(
		std::shared_ptr<Model> model, 
		const Properties& prop = Properties() 
		): model(model), motionState(position), body(nullptr)
	{
		if (model->getCollisionShape() != nullptr)
		{
            const CollisionShape& shape = model->getCollisionShape()->getCollisionShape();

			// calc inertia
			btVector3 fallInertia(0,0,0);
			if (prop.mass != 0.0f)
                shape._getShape().calculateLocalInertia(prop.mass, fallInertia);
		
			// construct rigid body
			btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(
				prop.mass, 
				&motionState, 
                &shape._getShape(),
				fallInertia);
			fallRigidBodyCI.m_friction = prop.friction;
			fallRigidBodyCI.m_restitution = prop.bouncyness;
			body = new btRigidBody(fallRigidBodyCI);
		}
	}
	    
	/// destructor
	virtual ~Object();
	
	/// shortcut for setting just the location
	inline void setLocation(const Vector3& location)
	{
		this->position.setLocation(location);
		this->syncPositionToPhysics();
	}

	/// set the Position
	inline void setPosition(const Position& position)
	{
	    this->position.set(position);
		this->syncPositionToPhysics();
	}
	
	/// get the position for modification
	inline const Position& getPosition()
	{
		return this->position;
	}

	inline std::shared_ptr<Model> getModel()
	{
	     return model;
	}

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