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
/** Header file for Sphere class
 *
 * @file Sphere.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_SPHERE_H
#define MAGIC3D_SPHERE_H

#include "Model.h"
#include "../Graphics/VertexHandler.h"

namespace Magic3D
{

/** Represent a sphere with configurable detail
 */
class Sphere : public Model
{
protected:
	/// collision shape for this model
	btSphereShape* sphereShape;
	
	/// default constructor
	inline Sphere(): Model(NULL), sphereShape(NULL) {/*intentionally left blank*/}
	
public:
	/** Standard constructor
	 * @param radius the radius of the sphere
	 * @param slices the number of squares on width
	 * @param stacks the number of squares on height
	 */
	Sphere(const VertexAttribSpec* spec, float radius, int slices, int stacks);
	
	/// destructor
	virtual ~Sphere();

	/** Get the object's type name
	 */
	virtual const char* getTypeName();
	
};



};


#endif









