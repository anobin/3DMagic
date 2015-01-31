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
/** Implementation file for built-in FlatSurface batch
 *
 * @file FlatSurface.cpp
 * @author Andrew Keating
 */

#include <Graphics/MeshBuilder.h>

namespace Magic3D
{
	
/** Build a flat surface batch
 * @param width the width of the surface
 * @param height the height of the surface
 * @param slices the number of squares on width
 * @param stacks the number of squares on height
 */
 void MeshBuilder::buildFlatSurface(Mesh* batch, float width, float height, int slices, 
    int stacks, bool texRepeat, float texPerX, float texPerY)
{
    this->begin(slices*stacks*6, 3, batch);
	
	float x = -width/2;
	float z = -height/2;
	float xStep = width/slices;
	float zStep = height/stacks;
	float texX;
	float texY;
	
	if (!texRepeat)
	{
		texX = (1.0f/slices);
		texY = (1.0f/stacks);
	}
	else
	{
		texX = (width/slices) / texPerX;
		texY = (height/stacks) / texPerY;
	}
	
	for (int i=0,j=0;;)
	{
		// top left
		normal3f(0.0f, 1.0f, 0.0f);
		texCoord2f(texX*i, texY*j);
		vertex3f(x, 0.0f, z);
		
		// bottom left
		normal3f(0.0f, 1.0f, 0.0f);
		texCoord2f(texX*i, texY*(j+1));
		vertex3f(x, 0.0f, z+zStep);
	
		// top right
		normal3f(0.0f, 1.0f, 0.0f);
		texCoord2f(texX*(i+1), texY*j);
		vertex3f(x+xStep, 0.0f, z);
		
		// top right
		normal3f(0.0f, 1.0f, 0.0f);
		texCoord2f(texX*(i+1), texY*j);
		vertex3f(x+xStep, 0.0f, z);

		// bottom left
		normal3f(0.0f, 1.0f, 0.0f);
		texCoord2f(texX*i, texY*(j+1));
		vertex3f(x, 0.0f, z+zStep);

		// bottom right
		normal3f(0.0f, 1.0f, 0.0f);
		texCoord2f(texX*(i+1), texY*(j+1));
		vertex3f(x+xStep, 0.0f, z+zStep);
		
		i++;
		if (i < slices)
			x += xStep;
		else
		{
			i = 0;
			x = -width/2;
			j++;
			if ( j >= stacks)
				break; // done
			z += zStep;
		}	
	}
	
	this->end();
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};




