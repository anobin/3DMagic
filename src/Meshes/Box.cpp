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


#include <Mesh/TriangleMeshBuilder.h>
#include <Shapes/Vertex.h>

namespace Magic3D
{
	
/** Build a box batch
 * @param width the width of the box
 * @param height the height of the box
 * @param depth the depth of the box
 */
TriangleMeshBuilderPTNT& TriangleMeshBuilderPTNT::buildBox(float width, float height, float depth)
{
	// we always work with half lengths
	width = width/2;
	height = height/2;
	depth = depth/2;

    // 8 points is minimum for positions, but need 6 more for proper texture mapping
    this->vertices.reserve(14);
    this->faces.reserve(12);

    enum {
        BACK_LEFT = 0,
        BACK_RIGHT,
        TOP_LEFT,
        TOP_RIGHT,
        LEFT_TOP,
        MID_TOP_LEFT,
        MID_TOP_RIGHT,
        RIGHT_TOP,
        LEFT_BOT,
        MID_BOT_LEFT,
        MID_BOT_RIGHT,
        RIGHT_BOT,
        BOT_LEFT,
        BOT_RIGHT
    };

    // register vertices that have the same position, but different tex coords
    this->addDuplicateVertexIndices(LEFT_TOP, TOP_LEFT);
    this->addDuplicateVertexIndices(RIGHT_TOP, TOP_RIGHT);
    this->addDuplicateVertexIndices(LEFT_BOT, BOT_LEFT, BACK_LEFT);
    this->addDuplicateVertexIndices(RIGHT_BOT, BOT_RIGHT, BACK_RIGHT);

    Scalar third = 1.0f / 3.0f;
    Scalar twoThirds = 2.0f / 3.0f;

    VertexPTNT points[14];

    points[BACK_LEFT].position(-width, -height, -depth);
    points[BACK_LEFT].texCoord(third, 1);

    points[BACK_RIGHT].position(width, -height, -depth);
    points[BACK_RIGHT].texCoord(twoThirds, 1);

    points[TOP_LEFT].position(-width, height, -depth);
    points[TOP_LEFT].texCoord(third, 0.75f);

    points[TOP_RIGHT].position(width, height, -depth);
    points[TOP_RIGHT].texCoord(twoThirds, 0.75f);

    points[LEFT_TOP].position(-width, height, -depth);
    points[LEFT_TOP].texCoord(0, 0.5f);

    points[MID_TOP_LEFT].position(-width, height, depth);
    points[MID_TOP_LEFT].texCoord(third, 0.5f);

    points[MID_TOP_RIGHT].position(width, height, depth);
    points[MID_TOP_RIGHT].texCoord(twoThirds, 0.5f);

    points[RIGHT_TOP].position(width, height, -depth);
    points[RIGHT_TOP].texCoord(1, 0.5f);

    points[LEFT_BOT].position(-width, -height, -depth);
    points[LEFT_BOT].texCoord(0, 0.25f);

    points[MID_BOT_LEFT].position(-width, -height, depth);
    points[MID_BOT_LEFT].texCoord(third, 0.25f);

    points[MID_BOT_RIGHT].position(width, -height, depth);
    points[MID_BOT_RIGHT].texCoord(twoThirds, 0.25f);

    points[RIGHT_BOT].position(width, -height, -depth);
    points[RIGHT_BOT].texCoord(1, 0.25f);

    points[BOT_LEFT].position(-width, -height, -depth);
    points[BOT_LEFT].texCoord(third, 0);

    points[BOT_RIGHT].position(width, -height, -depth);
    points[BOT_RIGHT].texCoord(twoThirds, 0);

    for (VertexPTNT vertex : points)
        this->vertices.push_back(vertex);

    
	// 6 sides, 2 triangles per each

	// top
    this->addFace(TOP_LEFT, MID_TOP_LEFT, TOP_RIGHT);
    this->addFace(MID_TOP_LEFT, MID_TOP_RIGHT, TOP_RIGHT);
    
    // bottom
    this->addFace(BOT_LEFT, BOT_RIGHT, MID_BOT_LEFT);
    this->addFace(MID_BOT_LEFT, BOT_RIGHT, MID_BOT_RIGHT);

    // left side
    this->addFace(LEFT_TOP, LEFT_BOT, MID_TOP_LEFT);
    this->addFace(LEFT_BOT, MID_BOT_LEFT, MID_TOP_LEFT);
    
    // right side
    this->addFace(RIGHT_TOP, MID_TOP_RIGHT, RIGHT_BOT);
    this->addFace(RIGHT_BOT, MID_TOP_RIGHT, MID_BOT_RIGHT);
	
    // front
    this->addFace(MID_TOP_LEFT, MID_BOT_LEFT, MID_TOP_RIGHT);
    this->addFace(MID_TOP_RIGHT, MID_BOT_LEFT, MID_BOT_RIGHT);

	// back
    this->addFace(TOP_LEFT, TOP_RIGHT, BACK_LEFT);
    this->addFace(BACK_LEFT, TOP_RIGHT, BACK_RIGHT);
	
    this->calaculateNormalsFromFaces();
    this->calculateTangentsFromFaces();

    return *this;
}
	
	
	
	
	
	
	
	
};

















































