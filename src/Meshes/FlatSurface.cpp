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
std::shared_ptr<TriangleMesh> TriangleMeshBuilder::buildFlatSurface(float width, float height, int slices, 
    int stacks, bool texRepeat, float texPerX, float texPerY)
{
    std::set<GpuProgram::AttributeType> attrs;
    attrs.insert(GpuProgram::AttributeType::VERTEX);
    attrs.insert(GpuProgram::AttributeType::TEX_COORD_0);
    attrs.insert(GpuProgram::AttributeType::NORMAL);
    attrs.insert(GpuProgram::AttributeType::TANGENT);

    int vertexCount = slices*stacks * 6;
    auto mesh = std::make_shared<TriangleMesh>(vertexCount, vertexCount / 3, attrs);
    int currentVertex = 0;
	
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
        auto vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x, 0.0f, z);
        vert.texCoord(texX*i, texY*j);
        mesh->setVertex(currentVertex, vert);
        currentVertex++;
		
		// bottom left
        vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x, 0.0f, z + zStep);
        vert.texCoord(texX*i, texY*(j + 1));
        mesh->setVertex(currentVertex, vert);
        currentVertex++;
	
		// top right
        vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x + xStep, 0.0f, z);
        vert.texCoord(texX*(i + 1), texY*j);
        mesh->setVertex(currentVertex, vert);
        currentVertex++;
		
		// top right
        vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x + xStep, 0.0f, z);
        vert.texCoord(texX*(i + 1), texY*j);
        mesh->setVertex(currentVertex, vert);
        currentVertex++;

		// bottom left
        vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x, 0.0f, z + zStep);
        vert.texCoord(texX*i, texY*(j + 1));
        mesh->setVertex(currentVertex, vert);
        currentVertex++;

		// bottom right
        vert = mesh->getVertex<PositionAttr, TexCoordAttr>(currentVertex);
        vert.position(x + xStep, 0.0f, z + zStep);
        vert.texCoord(texX*(i + 1), texY*(j + 1));
        mesh->setVertex(currentVertex, vert);
        currentVertex++;
		
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

    for (int i = 0; i < vertexCount; i += 3)
    {
        mesh->setFace(i / 3, TriangleMesh::Face(i, i + 1, i + 2));
    }

    mesh->calculateNormalsAndTangents();
	
    return mesh;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
};




