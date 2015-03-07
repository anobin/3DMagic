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

#include <Mesh/TriangleMeshBuilder.h>

namespace Magic3D
{

/** Build 2D rectangle batch
 * @param x the upper-left x coordinate of the rectangle
 * @param y the upper-left y coordinate of the rectangle
 * @param width the width of the rectangle
 * @param height the height of the rectangle
 */
std::shared_ptr<TriangleMesh> TriangleMeshBuilder::build2DRectangle(int x, int y, int width, int height)
{	
	// since this is a 2D model, we leave all z coords at 0
	
    std::set<GpuProgram::AttributeType> attrs;
    attrs.insert(GpuProgram::AttributeType::VERTEX);
    attrs.insert(GpuProgram::AttributeType::TEX_COORD_0);

	// 6 points
    auto mesh = std::make_shared<TriangleMesh>(6, 2, attrs);
	
	// bottom right
    auto vert = mesh->getVertex<PositionAttr, TexCoordAttr>(0);
    vert.position((float)x + width, (float)y, 0.0f);
    vert.texCoord(1.0f, 1.0f);

	// top right
    vert = mesh->getVertex<PositionAttr, TexCoordAttr>(1);
    vert.position((float)x + width, (float)y + height, 0.0f);
    vert.texCoord(1.0f, 0.0f);

	// top left
    vert = mesh->getVertex<PositionAttr, TexCoordAttr>(2);
    vert.position((float)x, (float)y + height, 0.0f);
    vert.texCoord(0.0f, 0.0f);
	
	// bottom right
    vert = mesh->getVertex<PositionAttr, TexCoordAttr>(3);
    vert.position((float)x + width, (float)y, 0.0f);
    vert.texCoord(1.0f, 1.0f);

	// top left
    vert = mesh->getVertex<PositionAttr, TexCoordAttr>(4);
    vert.position((float)x, (float)y + height, 0.0f);
    vert.texCoord(0.0f, 0.0f);

	// bottom left
    vert = mesh->getVertex<PositionAttr, TexCoordAttr>(5);
    vert.position((float)x, (float)y, 0.0f);
    vert.texCoord(0.0f, 1.0f);
	
    mesh->getFace(0).set(0, 1, 2);
    mesh->getFace(1).set(3, 4, 5);

    return mesh;
}


};






