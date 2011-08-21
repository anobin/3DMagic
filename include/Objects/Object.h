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
#include "../Graphics/GraphicalEntity.h"

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
	GraphicalEntity graphical;
	
	/// physical body for object
	PhysicalBody* physical;
	
public:
	/// standard constructor
	inline Object(Model* model): 
	    graphical(model), physical(NULL) {}
	    
	inline Object(Model* model, Position& p): position(p), 
	    graphical(model), physical(NULL) {}
	    
	inline Object(Model* model, Point3 location): 
	    graphical(model), physical(NULL) 
	{
	    position.getLocation().set(location);
	}
	
	/// destructor
	virtual ~Object();

	/// set the Position
	inline void setPosition(const Position& position)
	{
	    if (physical)
	        physical->syncPositionToPhysics();
		this->position.set(position);
	}
	
	/// get the position for modification
	inline const Position& getPosition()
	{
		return this->position;
	}
	
	/// get physical body for this model
	inline PhysicalBody* getPhysical()
	{
	    return physical;
	}
	
	/// get graphical body for this model
	inline GraphicalEntity* getGraphical()
	{
	     return &graphical;   
	}

};





};



#endif





















