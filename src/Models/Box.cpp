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
/** Implementation file for Box class
 *
 * @file Box.cpp
 * @author Andrew Keating
 */

#include <Models/Box.h>

namespace Magic3D
{
	
/** Standard constructor
 * @param width the width of the box
 * @param height the height of the box
 * @param depth the depth of the box
 */
 Box::Box(const VertexAttribSpec* spec, float width, float height, float depth):
    Model(spec)
{
	// we always work with half lengths
	width = width/2;
	height = height/2;
	depth = depth/2;
	
	// create physical body
	this->boxShape = new btBoxShape(btVector3(width, height, depth));
	this->Model::collisionShape = this->boxShape;
	
	// remeber to always construct models around the origin, a transform
	// matrix will be used on the graphics hardware to push them into
	// the right positions. Also don't try to reduce the number of vertices
	// by using something other than triangles, you'll screw up the lightening
	// becuase some points require normals in two different directions
	
	// 6 points per side, 6 sides
	data.begin(6*6);
	
	// top  and bottom 6 points
	for (int i=0; i < 2; i++)
	{
		float h = height;
		float n = 1.0f;
		if (i == 1)
		{
			h = -height;
			n = -1.0f;
		}
		data.normal3f	(0.0f, n, 0.0f);
		data.texCoord2f	(0.0f, 0.0f);
		data.position3f	(-width, h, -depth); // top left
		data.normal3f	(0.0f, n, 0.0f);
		data.texCoord2f	(0.0f, 1.0f);
		data.position3f	(-width, h, depth); // bottom left
		data.normal3f	(0.0f, n, 0.0f);
		data.texCoord2f	(1.0f, 0.0f);
		data.position3f	(width, h, -depth); // top right
		data.normal3f	(0.0f, n, 0.0f);
		data.texCoord2f	(0.0f, 1.0f);
		data.position3f	(-width, h, depth); // bottom left
		data.normal3f	(0.0f, n, 0.0f);
		data.texCoord2f	(1.0f, 0.0f);
		data.position3f	(width, h, -depth); // top right
		data.normal3f	(0.0f, n, 0.0f);
		data.texCoord2f	(1.0f, 1.0f);
		data.position3f	(width, h, depth); // bottom right
	}
	
	// 2 sides, 6 points each
	for (int i=0; i < 2; i++)
	{
		float w = -width;
		float n = -1.0f;
		if (i == 1)
		{
			w = width;
			n = 1.0f;
		}
		data.normal3f	(n, 0.0f, 0.0f);
		data.texCoord2f	(0.0f, 0.0f);
		data.position3f	(w, height, -depth); // top left
		data.normal3f	(n, 0.0f, 0.0f);
		data.texCoord2f	(0.0f, 1.0f);
		data.position3f	(w, -height, -depth); // bottom left
		data.normal3f	(n, 0.0f, 0.0f);
		data.texCoord2f	(1.0f, 0.0f);
		data.position3f	(w, height, depth); // top right
		data.normal3f	(n, 0.0f, 0.0f);
		data.texCoord2f	(0.0f, 1.0f);
		data.position3f	(w, -height, -depth); // bottom left
		data.normal3f	(n, 0.0f, 0.0f);
		data.texCoord2f	(1.0f, 0.0f);
		data.position3f	(w, height, depth); // top right
		data.normal3f	(n, 0.0f, 0.0f);
		data.texCoord2f	(1.0f, 1.0f);
		data.position3f	(w, -height, depth); // bottom right
	}
	
	// front and back, 6 points each
	for (int i=0; i < 2; i++)
	{
		float d = -depth;
		float n = -1.0f;
		if (i == 1)
		{
			d = depth;
			n = 1.0f;
		}
		data.normal3f	(0.0f, 0.0f, n);
		data.texCoord2f	(0.0f, 0.0f);
		data.position3f	(-width, height, d); // top left
		data.normal3f	(0.0f, 0.0f, n);
		data.texCoord2f	(0.0f, 1.0f);
		data.position3f	(-width, -height, d); // bottom left
		data.normal3f	(0.0f, 0.0f, n);
		data.texCoord2f	(1.0f, 0.0f);
		data.position3f	(width, height, d); // top right
		data.normal3f	(0.0f, 0.0f, n);
		data.texCoord2f	(0.0f, 1.0f);
		data.position3f	(-width, -height, d); // bottom left
		data.normal3f	(0.0f, 0.0f, n);
		data.texCoord2f	(1.0f, 0.0f);
		data.position3f	(width, height, d); // top right
		data.normal3f	(0.0f, 0.0f, n);
		data.texCoord2f	(1.0f, 1.0f);
		data.position3f	(width, -height, d); // bottom right
	}
	
	// close vertex data
	data.end();
}
	
/// destructor
Box::~Box()
{
	delete boxShape;
}

/** Get the object's type name
 */
const char* Box::getTypeName()
{
	return "Box";
}
	
	
	
	
	
	
	
	
	
};

















































