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
/** Header file for Box class
 *
 * @file Box.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_BOX_H
#define MAGIC3D_BOX_H

#include "Model.h"
#include "../Graphics/VertexHandler.h"
#include "../Util/Units.h"

namespace Magic3D
{

/** Represents a 3D rectangular box of a specified width, height, and depth
 */
class Box : public Model
{
protected:
	/// box collison shape for physics
	btBoxShape* boxShape;
	
	/// default constructor
	inline Box(): Model(NULL), boxShape(NULL) {/*intentionally left blank*/}
	
public:
	/** Standard constructor
	 * @param width the width of the box
	 * @param height the height of the box
	 * @param depth the depth of the box
	 */
	Box(const VertexAttribSpec* spec, float width, float height, float depth);
	
	/// destructor
	virtual ~Box();

	/** Get the object's type name
	 */
	virtual const char* getTypeName();




};



};


#endif


















