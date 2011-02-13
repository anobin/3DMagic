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
/** Header file for Circle2D class
 * 
 * @file Circle2D.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_CIRCLE_2D_H
#define MAGIC3D_CIRCLE_2D_H

#include "Model.h"

namespace Magic3D
{
	
/** Represents a single two-dimensional circle to be used for 
 * HUD's and GUI's
 */
class Circle2D : public Model
{
protected:
	/// default constructor
	inline Circle2D() {/* intentionally left blank */}
	
public:
	/** Standard constructor 
	 * @param x the x coordinate of the center
	 * @param y the y coordinate of the center
	 * @param radius the radius of the circle
	 * @param precisionAngle the angle between any two points on the edge of the
	 *                       circle, the lower angle, the better looking
	 */
	Circle2D(int x, int y, int radius, float precisionAngle);
	
	/// destructor
	virtual ~Circle2D();
	
	/** get the object's type name
	 */
	virtual const char* getTypeName();
};


};

#endif















