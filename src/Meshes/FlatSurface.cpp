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

#include <Mesh/TriangleMeshBuilder.h>

namespace Magic3D
{
	
/** Build a flat surface batch
 * @param width the width of the surface
 * @param height the height of the surface
 * @param slices the number of squares on width
 * @param stacks the number of squares on height
 */
std::shared_ptr<TriangleMesh> TriangleMeshBuilderPTNT::buildFlatSurface(float width, float height, int slices, 
    int stacks, bool texRepeat, float texPerX, float texPerY)
{
    TriangleMeshBuilderPTNT mb(slices*stacks*6);
	
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
        mb.addVertex(
            PositionAttr(x, 0.0f, z),
            TexCoordAttr(texX*i, texY*j),
            NormalAttr(0.0f, 1.0f, 0.0f)
        );
		
		// bottom left
        mb.addVertex(
            PositionAttr(x, 0.0f, z + zStep),
            TexCoordAttr(texX*i, texY*(j + 1)),
            NormalAttr(0.0f, 1.0f, 0.0f)
        );
	
		// top right
        mb.addVertex(
            PositionAttr(x + xStep, 0.0f, z),
            TexCoordAttr(texX*(i + 1), texY*j),
            NormalAttr(0.0f, 1.0f, 0.0f)
        );
		
		// top right
        mb.addVertex(
            PositionAttr(x + xStep, 0.0f, z),
            TexCoordAttr(texX*(i + 1), texY*j),
            NormalAttr(0.0f, 1.0f, 0.0f)
        );

		// bottom left
        mb.addVertex(
            PositionAttr(x, 0.0f, z + zStep),
            TexCoordAttr(texX*i, texY*(j + 1)),
            NormalAttr(0.0f, 1.0f, 0.0f)
        );

		// bottom right
        mb.addVertex(
            PositionAttr(x + xStep, 0.0f, z + zStep),
            TexCoordAttr(texX*(i + 1), texY*(j + 1)),
            NormalAttr(0.0f, 1.0f, 0.0f)
        );
		
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

    mb.calculateNormals();
    mb.calculateTangents();
	
    return mb.build();
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};




