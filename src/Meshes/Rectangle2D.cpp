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
/** Implementation file for build-in 2D Rectangle batch
 * 
 * @file Rectangle2D.cpp
 * @author Andrew Keating
 */

#include <Graphics/BatchBuilder.h>

namespace Magic3D
{

/** Build 2D rectangle batch
 * @param x the upper-left x coordinate of the rectangle
 * @param y the upper-left y coordinate of the rectangle
 * @param width the width of the rectangle
 * @param height the height of the rectangle
 */
void BatchBuilder::build2DRectangle(Batch* batch, int x, int y, int width, int height)
{	
	// since this is a 2D model, we leave all z coords at 0
	
	// 4 points using a TRIANGLE FAN
	this->begin(4, 3, batch);
	
	// bottom right
	texCoord2f(1.0f, 0.0f);
	vertex3f((float)x+width, (float)y, 0.0f);
	
	// bottom left
	texCoord2f(0.0f, 0.0f);
	vertex3f((float)x, (float)y, 0.0f);
	
	// top left
	texCoord2f(0.0f, 1.0f);
	vertex3f((float)x, (float)y+height, 0.0f);
	
	// top right
	texCoord2f(1.0f, 1.0f);
	vertex3f((float)x+width, (float)y+height, 0.0f);
	
	// end vertex data
	this->end();
}


};






