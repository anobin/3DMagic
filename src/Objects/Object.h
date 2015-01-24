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
#include "../Physics/PhysicalBody.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"

#include <set>

namespace Magic3D
{

class World;
    
/** Base class for all objects 
 */
class Object
{
protected:
    friend class World;
    
	/// 3D position of object
	Position position;
	
	std::shared_ptr<Mesh> mesh;

	Material* material;
	
	/// physical body for object
	PhysicalBody* physical;
	bool physicalAlloc;
	
	/// purely graphical attachments to this object, used mainly for decals
	std::set<Object*> attachments;
	
public:
	/// standard constructor
	inline Object(): mesh(NULL), material(NULL),
	    physical(NULL), physicalAlloc(false) {}
	
	/// standard constructor for graphical-only objects
	inline Object(std::shared_ptr<Mesh> mesh, Material* material): mesh(mesh), material(material),
	    physical(NULL), physicalAlloc(false) {}
	
	/// standard constructor for physical-only objects
	inline Object(CollisionShape* shape, const PhysicalBody::Properties& prop = 
	    PhysicalBody::Properties() ): mesh(NULL), material(nullptr),
	    physical(NULL), physicalAlloc(false) 
	{
	    this->createPhysical(shape, prop);
	}
	
	/// standard constructor for objects
	inline Object(std::shared_ptr<Mesh> mesh, Material* material, CollisionShape* shape, const PhysicalBody::Properties& prop = 
	    PhysicalBody::Properties() ): mesh(mesh), material(material),
	    physical(NULL), physicalAlloc(false) 
	{
	    this->createPhysical(shape, prop);
	}
	    
	/// destructor
	virtual ~Object();
	
	/// set the physical component directly
	inline void setPhysical(PhysicalBody* pbody)
	{
	    if (physicalAlloc)
	        delete this->physical;
	    this->physical = pbody;
	    this->physicalAlloc = false;
	}
	
	/// create the physical component
	inline void createPhysical(CollisionShape* shape, 
	    const PhysicalBody::Properties& prop = PhysicalBody::Properties() )
	{
	    if (physicalAlloc)
	        delete this->physical;
	    this->physical = new PhysicalBody(this->position, *shape, prop);
	    this->physicalAlloc = true;
	}
	
	/// remove the physical component
	inline void removePhysical()
	{
	    if (physicalAlloc)
	        delete this->physical;
	    this->physical = NULL;
	    this->physicalAlloc = false;
	}
	
	/// shortcut for setting just the location
	inline void setLocation(const Point3& location)
	{
		this->position.setLocation(location);
	    if (physical)
	        physical->syncPositionToPhysics();
	}

	/// set the Position
	inline void setPosition(const Position& position)
	{
	    this->position.set(position);
	    if (physical)
	        physical->syncPositionToPhysics();
	}
	
	/// get the position for modification
	inline const Position& getPosition()
	{
		return this->position;
	}
	
	inline void addAttachment(Object* ob)
	{
	    attachments.insert(ob);
	}
	
	inline void removeAttachment(Object* ob)
	{
	    std::set<Object*>::iterator it;
	    it = attachments.find(ob);
	    MAGIC_THROW(it == attachments.end(), "Attempted to remove an attachment that "
	        "was never attached.");
	    attachments.erase(ob);
	}
	
	/// get physical body for this model
	inline PhysicalBody* getPhysical()
	{
	    return physical;
	}

	inline std::shared_ptr<Mesh> getMesh()
	{
	     return mesh;   
	}

	inline Material* getMaterial()
	{
		return material;
	}

};





};



#endif





















