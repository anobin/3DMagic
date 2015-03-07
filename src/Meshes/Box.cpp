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

void addVertex(std::shared_ptr<TriangleMesh> mesh, int i, Vector3& pos, Vector2& coords)
{
    auto vert = mesh->getVertex<PositionAttr, TexCoordAttr>(i);
    vert.position(pos);
    vert.texCoord(coords);
}
	
/** Build a box batch
 * @param width the width of the box
 * @param height the height of the box
 * @param depth the depth of the box
 */
std::shared_ptr<TriangleMesh> TriangleMeshBuilder::buildBox(float width, float height, float depth)
{
	// we always work with half lengths
	width = width/2;
	height = height/2;
	depth = depth/2;

    std::set<GpuProgram::AttributeType> attrs;
    attrs.insert(GpuProgram::AttributeType::VERTEX);
    attrs.insert(GpuProgram::AttributeType::TEX_COORD_0);
    attrs.insert(GpuProgram::AttributeType::NORMAL);
    attrs.insert(GpuProgram::AttributeType::TANGENT);

    auto mesh = std::make_shared<TriangleMesh>(36, 12, attrs);

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
    Vector3 points[14];
    Vector2 texCoords[14];

    points[BACK_LEFT].set(-width, -height, -depth);
    texCoords[BACK_LEFT].set(third, 1);

    points[BACK_RIGHT].set(width, -height, -depth);
    texCoords[BACK_RIGHT].set(twoThirds, 1);

    points[TOP_LEFT].set(-width, height, -depth);
    texCoords[TOP_LEFT].set(third, 0.75f);

    points[TOP_RIGHT].set(width, height, -depth);
    texCoords[TOP_RIGHT].set(twoThirds, 0.75f);

    points[LEFT_TOP].set(-width, height, -depth);
    texCoords[LEFT_TOP].set(0, 0.5f);

    points[MID_TOP_LEFT].set(-width, height, depth);
    texCoords[MID_TOP_LEFT].set(third, 0.5f);

    points[MID_TOP_RIGHT].set(width, height, depth);
    texCoords[MID_TOP_RIGHT].set(twoThirds, 0.5f);

    points[RIGHT_TOP].set(width, height, -depth);
    texCoords[RIGHT_TOP].set(1, 0.5f);

    points[LEFT_BOT].set(-width, -height, -depth);
    texCoords[LEFT_BOT].set(0, 0.25f);

    points[MID_BOT_LEFT].set(-width, -height, depth);
    texCoords[MID_BOT_LEFT].set(third, 0.25f);

    points[MID_BOT_RIGHT].set(width, -height, depth);
    texCoords[MID_BOT_RIGHT].set(twoThirds, 0.25f);

    points[RIGHT_BOT].set(width, -height, -depth);
    texCoords[RIGHT_BOT].set(1, 0.25f);

    points[BOT_LEFT].set(-width, -height, -depth);
    texCoords[BOT_LEFT].set(third, 0);

    points[BOT_RIGHT].set(width, -height, -depth);
    texCoords[BOT_RIGHT].set(twoThirds, 0);

    
	// 6 sides, 2 triangles per each
    // duplicate all points as we want unmerged normals for proper shading

    int i = 0;

	// top
    addVertex(mesh, i++, points[TOP_LEFT], texCoords[TOP_LEFT]);
    addVertex(mesh, i++, points[MID_TOP_LEFT], texCoords[MID_TOP_LEFT]);
    addVertex(mesh, i++, points[TOP_RIGHT], texCoords[TOP_RIGHT]);
    addVertex(mesh, i++, points[MID_TOP_LEFT], texCoords[MID_TOP_LEFT]);
    addVertex(mesh, i++, points[MID_TOP_RIGHT], texCoords[MID_TOP_RIGHT]);
    addVertex(mesh, i++, points[TOP_RIGHT], texCoords[TOP_RIGHT]);
    
    // bottom
    addVertex(mesh, i++, points[BOT_LEFT], texCoords[BOT_LEFT]);
    addVertex(mesh, i++, points[BOT_RIGHT], texCoords[BOT_RIGHT]);
    addVertex(mesh, i++, points[MID_BOT_LEFT], texCoords[MID_BOT_LEFT]);
    addVertex(mesh, i++, points[MID_BOT_LEFT], texCoords[MID_BOT_LEFT]);
    addVertex(mesh, i++, points[BOT_RIGHT], texCoords[BOT_RIGHT]);
    addVertex(mesh, i++, points[MID_BOT_RIGHT], texCoords[MID_BOT_RIGHT]);

    // left side
    addVertex(mesh, i++, points[LEFT_TOP], texCoords[LEFT_TOP]);
    addVertex(mesh, i++, points[LEFT_BOT], texCoords[LEFT_BOT]);
    addVertex(mesh, i++, points[MID_TOP_LEFT], texCoords[MID_TOP_LEFT]);
    addVertex(mesh, i++, points[LEFT_BOT], texCoords[LEFT_BOT]);
    addVertex(mesh, i++, points[MID_BOT_LEFT], texCoords[MID_BOT_LEFT]);
    addVertex(mesh, i++, points[MID_TOP_LEFT], texCoords[MID_TOP_LEFT]);
    
    // right side
    addVertex(mesh, i++, points[RIGHT_TOP], texCoords[RIGHT_TOP]);
    addVertex(mesh, i++, points[MID_TOP_RIGHT], texCoords[MID_TOP_RIGHT]);
    addVertex(mesh, i++, points[RIGHT_BOT], texCoords[RIGHT_BOT]);
    addVertex(mesh, i++, points[RIGHT_BOT], texCoords[RIGHT_BOT]);
    addVertex(mesh, i++, points[MID_TOP_RIGHT], texCoords[MID_TOP_RIGHT]);
    addVertex(mesh, i++, points[MID_BOT_RIGHT], texCoords[MID_BOT_RIGHT]);
	
    // front
    addVertex(mesh, i++, points[MID_TOP_LEFT], texCoords[MID_TOP_LEFT]);
    addVertex(mesh, i++, points[MID_BOT_LEFT], texCoords[MID_BOT_LEFT]);
    addVertex(mesh, i++, points[MID_TOP_RIGHT], texCoords[MID_TOP_RIGHT]);
    addVertex(mesh, i++, points[MID_TOP_RIGHT], texCoords[MID_TOP_RIGHT]);
    addVertex(mesh, i++, points[MID_BOT_LEFT], texCoords[MID_BOT_LEFT]);
    addVertex(mesh, i++, points[MID_BOT_RIGHT], texCoords[MID_BOT_RIGHT]);

	// back
    addVertex(mesh, i++, points[TOP_LEFT], texCoords[TOP_LEFT]);
    addVertex(mesh, i++, points[TOP_RIGHT], texCoords[TOP_RIGHT]);
    addVertex(mesh, i++, points[BACK_LEFT], texCoords[BACK_LEFT]);
    addVertex(mesh, i++, points[BACK_LEFT], texCoords[BACK_LEFT]);
    addVertex(mesh, i++, points[TOP_RIGHT], texCoords[TOP_RIGHT]);
    addVertex(mesh, i++, points[BACK_RIGHT], texCoords[BACK_RIGHT]);

    //generate faces to match with vertices
    for (int i = 0; i < 36; i += 3)
        mesh->getFace(i / 3).set(i, i + 1, i + 2);

    mesh->calculateNormalsAndTangents();

    return mesh;
}
	
	
	
	
	
	
	
	
};

















































