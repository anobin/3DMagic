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
#include "../Math/Position.h"
#include "../Util/Color.h"
#include "../Graphics/Texture.h"
#include "../Exceptions/MagicException.h"
#include "../Physics/PhysicalBody.h"
#include "../Graphics/GraphicalBody.h"

namespace Magic3D
{


/** Base class for all objects 
 */
class Object
{
protected:
	/// 3D position of object
	Position position;
	
	/// graphical body for object
	GraphicalBody graphical;
	
	/// physical body for object
	PhysicalBody physical;
	
public:
	/// standard constructor
	inline Object(Model& model, float mass): graphical(model), 
			physical( position, model, mass ) {}
			
	/// standard constructor
	inline Object(Model& model, float mass, Point3 location):
	    position(location.getX(), location.getY(), location.getZ()),
	    graphical(model), physical( this->position, model, mass ) 
	{}
	
	/// standard constructor
	inline Object(Model& model, float mass, Position position): 
	    position(position), graphical(model), physical( this->position, model, mass ) 
	{}
	
	/// destructor
	virtual ~Object();


	/** Draw the object
	 */
	inline void draw()
	{
		graphical.draw();
	}
	
	/// get the model used
	inline Model* getModel()
	{
		return graphical.getModel();
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
		graphical.setBaseTexture( texture );
	}
	
	/// get the texture
	inline Texture* getBaseTexture()
	{
		return graphical.getBaseTexture();
	}
	
	/// get the rigid body for this object
	inline btRigidBody* getRigidBody()
	{
		return physical.getRigidBody();
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
		physical.syncPositionToPhysics();
	}

};





};



#endif





















