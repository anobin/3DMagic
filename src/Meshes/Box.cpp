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

    this->vertices.reserve(36);

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

    Scalar third = 1.0f / 3.0f;
    Scalar twoThirds = 2.0f / 3.0f;

    // unique points
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

    
	// 6 sides, 2 triangles per each
    // duplicate all points as we want unmerged normals for proper shading

	// top
    this->addVertex(points[TOP_LEFT]);
    this->addVertex(points[MID_TOP_LEFT]);
    this->addVertex(points[TOP_RIGHT]);
    this->addVertex(points[MID_TOP_LEFT]);
    this->addVertex(points[MID_TOP_RIGHT]);
    this->addVertex(points[TOP_RIGHT]);
    
    // bottom
    this->addVertex(points[BOT_LEFT]);
    this->addVertex(points[BOT_RIGHT]);
    this->addVertex(points[MID_BOT_LEFT]);
    this->addVertex(points[MID_BOT_LEFT]);
    this->addVertex(points[BOT_RIGHT]);
    this->addVertex(points[MID_BOT_RIGHT]);

    // left side
    this->addVertex(points[LEFT_TOP]);
    this->addVertex(points[LEFT_BOT]);
    this->addVertex(points[MID_TOP_LEFT]);
    this->addVertex(points[LEFT_BOT]);
    this->addVertex(points[MID_BOT_LEFT]);
    this->addVertex(points[MID_TOP_LEFT]);
    
    // right side
    this->addVertex(points[RIGHT_TOP]);
    this->addVertex(points[MID_TOP_RIGHT]);
    this->addVertex(points[RIGHT_BOT]);
    this->addVertex(points[RIGHT_BOT]);
    this->addVertex(points[MID_TOP_RIGHT]);
    this->addVertex(points[MID_BOT_RIGHT]);
	
    // front
    this->addVertex(points[MID_TOP_LEFT]);
    this->addVertex(points[MID_BOT_LEFT]);
    this->addVertex(points[MID_TOP_RIGHT]);
    this->addVertex(points[MID_TOP_RIGHT]);
    this->addVertex(points[MID_BOT_LEFT]);
    this->addVertex(points[MID_BOT_RIGHT]);

	// back
    this->addVertex(points[TOP_LEFT]);
    this->addVertex(points[TOP_RIGHT]);
    this->addVertex(points[BACK_LEFT]);
    this->addVertex(points[BACK_LEFT]);
    this->addVertex(points[TOP_RIGHT]);
    this->addVertex(points[BACK_RIGHT]);

    //generate faces to match with vertices
    for (int i = 0; i < 36; i += 3)
        this->faces.push_back(TriangleMesh::Face(i, i + 1, i + 2));

    this->calculateNormalsAndTangents();

    return *this;
}
	
	
	
	
	
	
	
	
};

















































