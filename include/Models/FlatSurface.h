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
/** Header file for FlatSurface class
 *
 * @file FlatSurface.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_FLAT_SURFACE_H
#define MAGIC3D_FLAT_SURFACE_H

#include "../Model.h"
#include "../VertexHandler.h"
#include "../Units.h"

namespace Magic3D
{

/** Represent a flat 2D surface composed of a configurable
 * number of triangle to allow for proper shading
 */
class FlatSurface : public Model
{
protected:
	/// plane shape
	btStaticPlaneShape* planeShape;
	
	/// default constructor
	inline FlatSurface(): planeShape(NULL) {/*intentionally left blank*/}
	
public:
	/** Standard constructor
	 * @param width the width of the surface
	 * @param height the height of the surface
	 * @param slices the number of squares on width
	 * @param stacks the number of squares on height
	 * @param texRepeat whether or not to repeat textures over the surface
	 * @param texPerX the ammount of X to make one segment of a repeated texture
	 * @param texPerY the ammount of Y to make one segment of a repeated texture
	 */
	FlatSurface(float width, float height, int slices, int stacks,
				bool texRepeat = false, float texPerX = FOOT, float texPerY = FOOT);
	
	/// destructor
	virtual ~FlatSurface();

	/** Get the object's type name
	 */
	virtual const char* getTypeName();




};



};


#endif









