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
/** Implementation file for built-in Box batch
 *
 * @file Box.cpp
 * @author Andrew Keating
 */


#include <Graphics/BatchBuilder.h>

namespace Magic3D
{
	
/** Build a box batch
 * @param width the width of the box
 * @param height the height of the box
 * @param depth the depth of the box
 */
void BatchBuilder::buildBox(Batch* batch, float width, float height, float depth )
{
	// we always work with half lengths
	width = width/2;
	height = height/2;
	depth = depth/2;
	
	// remeber to always construct models around the origin, a transform
	// matrix will be used on the graphics hardware to push them into
	// the right positions. Also don't try to reduce the number of vertices
	// by using something other than triangles, you'll screw up the lightening
	// becuase some points require normals in two different directions
	
	// 6 points per side, 6 sides
	this->begin(6*6, 3, batch);
		
	// top, 6 points
    normal3f	(0.0f, 1.0f, 0.0f);
    texCoord2f	(0.0f, 0.0f);
    vertex3f	(-width, height, -depth); // top left
    normal3f	(0.0f, 1.0f, 0.0f);
    texCoord2f	(0.0f, 1.0f);
    vertex3f	(-width, height, depth); // bottom left
    normal3f	(0.0f, 1.0f, 0.0f);
    texCoord2f	(1.0f, 0.0f);
    vertex3f	(width, height, -depth); // top right
    normal3f	(0.0f, 1.0f, 0.0f);
    texCoord2f	(0.0f, 1.0f);
    vertex3f	(-width, height, depth); // bottom left
    normal3f	(0.0f, 1.0f, 0.0f);
    texCoord2f	(1.0f, 1.0f);
    vertex3f	(width, height, depth); // bottom right
	normal3f	(0.0f, 1.0f, 0.0f);
    texCoord2f	(1.0f, 0.0f);
    vertex3f	(width, height, -depth); // top right
    
    // bottom, 6 points
    normal3f	(0.0f, -1.0f, 0.0f);
    texCoord2f	(1.0f, 0.0f);
    vertex3f	(-width, -height, -depth); // top left
	normal3f	(0.0f, -1.0f, 0.0f);
    texCoord2f	(0.0f, 0.0f);
    vertex3f	(width, -height, -depth); // top right
    normal3f	(0.0f, -1.0f, 0.0f);
    texCoord2f	(1.0f, 1.0f);
    vertex3f	(-width, -height, depth); // bottom left
    normal3f	(0.0f, -1.0f, 0.0f);
    texCoord2f	(1.0f, 1.0f);
    vertex3f	(-width, -height, depth); // bottom left
	normal3f	(0.0f, -1.0f, 0.0f);
    texCoord2f	(0.0f, 0.0f);
    vertex3f	(width, -height, -depth); // top right
    normal3f	(0.0f, -1.0f, 0.0f);
    texCoord2f	(0.0f, 1.0f);
    vertex3f	(width, -height, depth); // bottom right

    // left side, 6 points
    normal3f	(-1.0f, 0.0f, 0.0f);
    texCoord2f	(0.0f, 0.0f);
    vertex3f	(-width, height, -depth); // top left
    normal3f	(-1.0f, 0.0f, 0.0f);
    texCoord2f	(0.0f, 1.0f);
    vertex3f	(-width, -height, -depth); // bottom left
    normal3f	(-1.0f, 0.0f, 0.0f);
    texCoord2f	(1.0f, 0.0f);
    vertex3f	(-width, height, depth); // top right
    normal3f	(-1.0f, 0.0f, 0.0f);
    texCoord2f	(0.0f, 1.0f);
    vertex3f	(-width, -height, -depth); // bottom left
    normal3f	(-1.0f, 0.0f, 0.0f);
    texCoord2f	(1.0f, 1.0f);
    vertex3f	(-width, -height, depth); // bottom right
	normal3f	(-1.0f, 0.0f, 0.0f);
    texCoord2f	(1.0f, 0.0f);
    vertex3f	(-width, height, depth); // top right
    
    // right side, 6 points
    normal3f	(1.0f, 0.0f, 0.0f);
    texCoord2f	(1.0f, 0.0f);
    vertex3f	(width, height, -depth); // top left
	normal3f	(1.0f, 0.0f, 0.0f);
    texCoord2f	(0.0f, 0.0f);
    vertex3f	(width, height, depth); // top right
    normal3f	(1.0f, 0.0f, 0.0f);
    texCoord2f	(1.0f, 1.0f);
    vertex3f	(width, -height, -depth); // bottom left
    normal3f	(1.0f, 0.0f, 0.0f);
    texCoord2f	(1.0f, 1.0f);
    vertex3f	(width, -height, -depth); // bottom left
	normal3f	(1.0f, 0.0f, 0.0f);
    texCoord2f	(0.0f, 0.0f);
    vertex3f	(width, height, depth); // top right
	normal3f	(1.0f, 0.0f, 0.0f);
    texCoord2f	(0.0f, 1.0f);
    vertex3f	(width, -height, depth); // bottom right
	
	// front, 6 points
    normal3f	(0.0f, 0.0f, -1.0f);
    texCoord2f	(1.0f, 0.0f);
    vertex3f	(-width, height, -depth); // top left
    normal3f	(0.0f, 0.0f, -1.0f);
    texCoord2f	(0.0f, 0.0f);
    vertex3f	(width, height, -depth); // top right
	normal3f	(0.0f, 0.0f, -1.0f);
    texCoord2f	(1.0f, 1.0f);
    vertex3f	(-width, -height, -depth); // bottom left
    normal3f	(0.0f, 0.0f, -1.0f);
    texCoord2f	(1.0f, 1.0f);
    vertex3f	(-width, -height, -depth); // bottom left
	normal3f	(0.0f, 0.0f, -1.0f);
    texCoord2f	(0.0f, 0.0f);
    vertex3f	(width, height, -depth); // top right
	normal3f	(0.0f, 0.0f, -1.0f);
    texCoord2f	(0.0f, 1.0f);
    vertex3f	(width, -height, -depth); // bottom right
	
    // back, 6 points
    normal3f	(0.0f, 0.0f, 1.0f);
    texCoord2f	(0.0f, 0.0f);
    vertex3f	(-width, height, depth); // top left
    normal3f	(0.0f, 0.0f, 1.0f);
    texCoord2f	(0.0f, 1.0f);
    vertex3f	(-width, -height, depth); // bottom left
    normal3f	(0.0f, 0.0f, 1.0f);
    texCoord2f	(1.0f, 0.0f);
    vertex3f	(width, height, depth); // top right
    normal3f	(0.0f, 0.0f, 1.0f);
    texCoord2f	(0.0f, 1.0f);
    vertex3f	(-width, -height, depth); // bottom left
	normal3f	(0.0f, 0.0f, 1.0f);
    texCoord2f	(1.0f, 1.0f);
    vertex3f	(width, -height, depth); // bottom right
    normal3f	(0.0f, 0.0f, 1.0f);
    texCoord2f	(1.0f, 0.0f);
    vertex3f	(width, height, depth); // top right
	
	this->end();
}
	
	
	
	
	
	
	
	
};

















































