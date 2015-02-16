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
#ifndef MAGIC3D_MESH_BUILDER_H
#define MAGIC3D_MESH_BUILDER_H

#include <sstream>
#include <iomanip>

#include "../Exceptions/MagicException.h"
#include <Shaders\GpuProgram.h>
#include "VertexArray.h"
#include "../Util/Color.h"
#include "../Math/Math.h"
#include "../Util/magic_throw.h"
#include "../Util/magic_assert.h"
#include <Graphics\Mesh.h>

#include <vector>
#include <Shapes\Vertex.h>
#include <Shapes\Triangle.h>

namespace Magic3D
{

template<typename... AttrTypes>
class MeshBuilder
{
private:	
    // TODO: change to directly set data in mesh and not require it to be copied
    std::vector<Vertex<AttrTypes...>> vertices;
    VertexArray::Primitives primitive;

public:
	/** Standard Constructor
	 */
    inline MeshBuilder(int vertexCount, 
        VertexArray::Primitives primitive = VertexArray::Primitives::TRIANGLES) :
        primitive(primitive)
    {
        this->vertices.reserve(vertexCount);
    }
	
    // less efficient constructor
    inline MeshBuilder(VertexArray::Primitives primitive = VertexArray::Primitives::TRIANGLES) :
        primitive(primitive)
    {}

    inline MeshBuilder<AttrTypes...>& reset(VertexArray::Primitives primitive = VertexArray::Primitives::TRIANGLES)
    {
        this->primitive = primitive;
        this->vertices.clear();

        return *this;
    }

    inline MeshBuilder<AttrTypes...>& reset(int vertexCount, VertexArray::Primitives primitive = VertexArray::Primitives::TRIANGLES)
    {
        this->primitive = primitive;
        this->vertices.clear();
        this->vertices.reserve(vertexCount);

        return *this;
    }
	
    inline unsigned int vertexCount()
    {
        return vertexCount;
    }

    inline void addVertex(const Vertex<AttrTypes...>& vertex)
    {
        this->vertices.push_back(vertex);
    }

    template<typename... VectorOrAttrTypes>
    inline void addVertex(const VectorOrAttrTypes&... vectorsOrAttrs)
    {
        this->vertices.push_back(Vertex<AttrTypes...>(vectorsOrAttrs...));
    }

    inline void positionTransform(const Matrix4& matrix)
    {
        for (Vertex<AttrTypes...>& vertex : this->vertices)
        {
            vertex.position(Vector3(vertex.position().getData()).transform(matrix));
        }
    }

    // TODO: make this more efficient
    // TODO: make this only accessible when a normal and a position attribute is included
    // TODO: limit this to triangle primitive
    // TODO: switch mesh storage to indexed list to avoid comparison precision
    inline void calculateNormals()
    {
        std::unordered_map<std::string, Vector3> map;

        std::stringstream ss;
        ss << std::setprecision(2);

        // calculate normals for unique positions
        for (unsigned int i = 0; i < this->vertices.size(); i += 3)
        {
            Vector4 points[] = {
                this->vertices[i].position(),
                this->vertices[i + 1].position(),
                this->vertices[i + 2].position()
            };
            Vector3 faceNormal = Triangle(points[0], points[1], points[2]).normal;

            for (Vector4 point : points)
            {
                ss.str("");
                ss << point.x() << "-" << point.y() << "-" << point.z() << "-" << point.w();
                map[ss.str()] = map[ss.str()] + faceNormal;
            }
        }

        // set calculated normals on vertices
        for (Vertex<AttrTypes...>& vertex : this->vertices)
        {
            Vector4 point = vertex.position();
            ss.str("");
            ss << point.x() << "-" << point.y() << "-" << point.z() << "-" << point.w();
            vertex.normal(map[ss.str()].normalize());
        }
    }

    inline Vertex<AttrTypes...>& getVertex(int index)
    {
        return this->vertices[index];
    }
	
    inline std::shared_ptr<Mesh> build()
    {
        return std::make_shared<Mesh>(vertices, primitive);
    }
	
	/** Build a box mesh
     * @param width the width of the box
     * @param height the height of the box
     * @param depth the depth of the box
     */
	static std::shared_ptr<Mesh> buildBox(float width, float height, float depth);
	
	/** Build a 2D circle mesh
     * @param x the x coordinate of the center
     * @param y the y coordinate of the center
     * @param radius the radius of the circle
     * @param precisionAngle the angle between any two points on the edge of the
                            circle, the lower angle, the better looking
     */
    static std::shared_ptr<Mesh> build2DCircle(int x, int y, int radius, float precisionAngle);
	
	/** Build a flat surface
     * @param width the width of the surface
     * @param height the height of the surface
     * @param slices the number of squares on width
     * @param stacks the number of squares on height
     */
    static std::shared_ptr<Mesh> buildFlatSurface(float width, float height, int slices,
        int stacks, bool texRepeat, float texPerX, float texPerY);

    /** Build 2D rectangle
     * @param x the upper-left x coordinate of the rectangle
     * @param y the upper-left y coordinate of the rectangle
     * @param width the width of the rectangle
     * @param height the height of the rectangle
     */
    static std::shared_ptr<Mesh> build2DRectangle(int x, int y, int width, int height);
    
    /** Build sphere
     * @param radius the radius of the sphere
     * @param slices the number of squares on width
     * @param stacks the number of squares on height
     */
    MeshBuilder<AttrTypes...>& buildSphere(float radius, int slices, int stacks);
    
};

typedef MeshBuilder<PositionAttr, TexCoordAttr, NormalAttr> MeshBuilderPTN;
typedef MeshBuilder<PositionAttr, TexCoordAttr> MeshBuilderPT;


};




#endif
















