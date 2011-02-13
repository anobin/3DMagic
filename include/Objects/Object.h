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

#include "../Models/Model.h"
#include "Position.h"
#include "../Util/Color.h"
#include "../Graphics/Texture.h"
#include "../Exceptions/MagicException.h"
#include "../Physics/MotionState.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

namespace Magic3D
{


/** Base class for all objects 
 */
class Object
{
protected:
	/// model to use to represent this object
	Model* model;
	
	/// 3D position of object
	Position position;
	
	/// base texture of object
	Texture* baseTexture;
	
	/// MotionState for physical body
	MotionState motionState;
	
	/// physical body for object
	btRigidBody* body;
	
	/// default constructor
	inline Object(): model(NULL), baseTexture(NULL), motionState(position), body(NULL) {}
	
	inline void build(float mass, float friction = 0.5f, float restitution = 0.0f,
						float linearDamping = 0.0f)
	{
		// calc inertia
		btVector3 fallInertia(0,0,0);
		model->getCollisionShape().calculateLocalInertia(mass,fallInertia);
		
		// construct rigid body
		btRigidBody::btRigidBodyConstructionInfo 
			fallRigidBodyCI(mass, &motionState, &model->getCollisionShape(), fallInertia);
		fallRigidBodyCI.m_friction = friction;
		fallRigidBodyCI.m_restitution = restitution;
		fallRigidBodyCI.m_linearDamping = linearDamping;
		body = new btRigidBody(fallRigidBodyCI);
	}
	
public:
	/// standard constructor
	inline Object(Model& model, float mass): model(&model), baseTexture(NULL),
			motionState(position), body(NULL) { build(mass); }
			
	/// standard constructor
	inline Object(Model& model, float mass, const Point3f& location, float friction = 0.5,
				  float restitution=0.0f, float linearDamping = 0.0f): model(&model), 
		baseTexture(NULL), motionState(position), body(NULL) 
	{ 
		position.getLocation().set(location);
		build(mass, friction, restitution, linearDamping); 
	}
			
	/// standard constructor
	inline Object(Model& model, float mass, const Position& position, float friction = 0.5f, 
				  float restitution = 0.0f, float linearDamping = 0.0f): model(&model), 
		baseTexture(NULL), motionState(this->position), body(NULL) 
	{ 
		this->position.set(position);
		build(mass, friction, restitution, linearDamping); 
	}
	
	/// destructor
	virtual ~Object();


	/** Draw the object
	 */
	inline void draw()
	{
		model->draw();
	}
	
	/// get the model used
	inline Model* getModel()
	{
		return model;
	}
	
	/// set the model to use
	inline void setModel(Model& model)
	{
		this->model = &model;
	}

	/// set the Position
	inline void setPosition(const Position& position)
	{
		this->position.set(position);
	}
	
	/// get the position for modification
	inline Position& getPosition()
	{
		return this->position;
	}
	
	/// set the texture
	inline void setBaseTexture(Texture& texture)
	{
		this->baseTexture = &texture;
	}
	
	/// get the texture
	inline Texture* getBaseTexture()
	{
		return this->baseTexture;
	}
	
	/// get the rigid body for this object
	inline btRigidBody* getRigidBody()
	{
		return body;
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

};





};



#endif





















