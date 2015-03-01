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
std::shared_ptr<TriangleMesh> TriangleMeshBuilderPT::build2DRectangle(int x, int y, int width, int height)
{	
	// since this is a 2D model, we leave all z coords at 0
	
	// 6 points
	TriangleMeshBuilderPT mb(6);
	
	// bottom right
    mb.addVertex(
        PositionAttr((float)x + width, (float)y, 0.0f),
        TexCoordAttr(1.0f, 1.0f)
    );

	// top right
    mb.addVertex(
        PositionAttr((float)x + width, (float)y + height, 0.0f),
        TexCoordAttr(1.0f, 0.0f)
    );

	// top left
    mb.addVertex(
        PositionAttr((float)x, (float)y + height, 0.0f),
        TexCoordAttr(0.0f, 0.0f)
    );
	
	// bottom right
    mb.addVertex(
        PositionAttr((float)x + width, (float)y, 0.0f),
        TexCoordAttr(1.0f, 1.0f)
    );

	// top left
    mb.addVertex(
        PositionAttr((float)x, (float)y + height, 0.0f),
        TexCoordAttr(0.0f, 0.0f)
    );

	// bottom left
    mb.addVertex(
        PositionAttr((float)x, (float)y, 0.0f),
        TexCoordAttr(0.0f, 1.0f)
    );
	
	// end vertex data
    return mb.build();
}


};






