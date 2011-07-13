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
/** Header file for Model class
 *
 * @file Model.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MODEL_H
#define MAGIC3D_MODEL_H

#include "../Graphics/VertexHandler.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

namespace Magic3D
{


/** Base class for all models (collection of vertex attributes
 * on graphics memory)
 */
class Model
{
protected:
	/// vertex handler to handler vertex attribute data
	VertexHandler data;
	
	/// shape of model used for collisions on physics engine
	btCollisionShape* collisionShape;
	
public:
    /// default constructor
    inline Model(const VertexAttribSpec* spec): data(spec), collisionShape(NULL) {}
    
	/// destructor
	virtual ~Model();


	/** Draw the object
	 */
	inline void draw(VertexArray::Primitives primitive = VertexArray::TRIANGLES)
	{
		data.draw(primitive);
	}

	/** Get the model's type name
	 */
	virtual const char* getTypeName() = 0;
	
	/** Get the collision shape for this model
	 */
	inline btCollisionShape& getCollisionShape()
	{
		return *this->collisionShape;
	}

};





};



#endif





















