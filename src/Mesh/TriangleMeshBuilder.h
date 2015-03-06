
#ifndef MAGIC3D_TRIANGLE_MESH_BUILDER_H
#define MAGIC3D_TRIANGLE_MESH_BUILDER_H


#include <sstream>
#include <iomanip>
#include <functional>

#include "../Exceptions/MagicException.h"
#include <Shaders\GpuProgram.h>
#include "../Util/Color.h"
#include "../Math/Math.h"
#include "../Util/magic_throw.h"
#include "../Util/magic_assert.h"
#include <Mesh\TriangleMesh.h>
#include <Shapes\Vertex.h>
#include <Shapes\Triangle.h>

#include <vector>

namespace Magic3D
{

template<typename... AttrTypes>
class TriangleMeshBuilder
{
private:
    // TODO: change to directly set data in mesh and not require it to be copied
    std::vector<Vertex<AttrTypes...>> vertices;

    std::vector<TriangleMesh::Face> faces;

    std::vector<std::vector<unsigned int>> duplicateVertexIndices;

public:
    /** Standard Constructor
    */
    inline TriangleMeshBuilder(int vertexCount, int faceCount = 0)
    {
        this->vertices.reserve(vertexCount);
        if (faceCount == 0)
            faceCount = vertexCount / 3;
        this->faces.reserve(faceCount);
    }

    inline TriangleMeshBuilder<AttrTypes...>& reset()
    {
        this->vertices.clear();
        this->faces.clear();
        this->duplicateVertexIndices.clear();

        return *this;
    }

    inline TriangleMeshBuilder<AttrTypes...>& reset(int vertexCount, int faceCount = 0)
    {
        this->vertices.clear();
        this->vertices.reserve(vertexCount);

        if (faceCount == 0)
            faceCount = vertexCount / 3;
        this->faces.clear();
        this->faces.reserve(faceCount);

        this->duplicateVertexIndices.clear();

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

    inline void addFace(const TriangleMesh::Face& face)
    {
        this->faces.push_back(face);
    }

    inline void addFace(unsigned int a, unsigned int b, unsigned int c)
    {
        this->faces.push_back(TriangleMesh::Face(a, b, c));
    }

    inline void addDuplicateVertexIndices(const std::vector<unsigned int> duplicateList)
    {
        this->duplicateVertexIndices.push_back(duplicateList);
    }

    inline void addDuplicateVertexIndices(unsigned int index1, unsigned int index2)
    {
        std::vector<unsigned int> list;
        list.push_back(index1);
        list.push_back(index2);
        this->duplicateVertexIndices.push_back(std::move(list));
    }

    inline void addDuplicateVertexIndices(unsigned int index1, unsigned int index2,
        unsigned int index3)
    {
        std::vector<unsigned int> list;
        list.push_back(index1);
        list.push_back(index2);
        list.push_back(index3);
        this->duplicateVertexIndices.push_back(std::move(list));
    }

    inline TriangleMeshBuilder<AttrTypes...>& positionTransform(const Matrix4& matrix)
    {
        for (Vertex<AttrTypes...>& vertex : this->vertices)
        {
            vertex.position(Vector3(vertex.position().getData()).transform(matrix));
        }

        return *this;
    }

    inline void calculateDuplicateVertices()
    {
        // TODO: define hash function and equals on Vector4 directly
        std::unordered_map<
            Vector4, 
            std::vector<unsigned int>,
            std::function<int(const Vector4&)>,
            std::function<bool(const Vector4&, const Vector4&)>
        > map(
            this->vertices.size(),
            [&](const Vector4& vec) -> int 
            { 
                return
                    std::hash<Scalar>()(vec.x()) ^
                    std::hash<Scalar>()(vec.y()) ^
                    std::hash<Scalar>()(vec.z()) ^
                    std::hash<Scalar>()(vec.w());
            },
            [&](const Vector4& a, const Vector4& b) -> bool
            {
                return
                    a.x() == b.x() &&
                    a.y() == b.y() &&
                    a.z() == b.z() &&
                    a.w() == b.w();
            }
        );

        for (unsigned int i = 0; i < this->vertices.size(); i++)
        {
            auto& vert = this->vertices[i];
            map[vert.position()].push_back(i);
        }

        for (auto it : map)
        {
            if (it.second.size() > 1)
            {
                this->duplicateVertexIndices.push_back(it.second);
            }
        }
    }

    inline void calculateNormalsAndTangents(Scalar thresholdAngle = 60.0f)
    {
        // calculate normals for unique positions
        // TODO: split vertex into multiple vertices if shared surface 
        //       normals exceed threshold angle? could be slow
        for (TriangleMesh::Face& face : this->faces)
        {
            auto& a = this->vertices[face.indices[0]];
            auto& b = this->vertices[face.indices[1]];
            auto& c = this->vertices[face.indices[2]];

            Vector3 faceNormal = Triangle(a.position(), b.position(), c.position()).normal;

            a.normal(a.normal() + faceNormal);
            b.normal(b.normal() + faceNormal);
            c.normal(c.normal() + faceNormal);

            Vector4 BA = b.position() - a.position();
            Vector4 CA = c.position() - a.position();

            Vector2 tBA = b.texCoord() - a.texCoord();
            Vector2 tCA = c.texCoord() - a.texCoord();
            Scalar area = (tBA.x() * tCA.y()) - (tBA.y() * tCA.x());

            Vector3 faceTangent;

            if (area != 0.0f)
            {
                Scalar delta = 1.0f / area;
                faceTangent = Vector3(
                    delta * ((BA.x() * tCA.y()) + (CA.x() * -tBA.y())),
                    delta * ((BA.y() * tCA.y()) + (CA.y() * -tBA.y())),
                    delta * ((BA.z() * tCA.y()) + (CA.z() * -tBA.y()))
                    );
            }

            a.tangent(a.tangent() + faceTangent);
            b.tangent(b.tangent() + faceTangent);
            c.tangent(c.tangent() + faceTangent);
        }

        // merge normals for different points that are at the same location,
        // if the angle between them is smaller than the threshold angle
        Scalar thresholdAngleRads = thresholdAngle * Scalar(M_PI / 180);
        for (auto& list : this->duplicateVertexIndices)
        {
            std::map<unsigned int, Vector3> joinedNormal;
            std::map<unsigned int, Vector3> joinedTangent;
            for (unsigned int i : list)
            {
                auto& vert1 = this->vertices[i];
                joinedNormal[i] += vert1.normal();
                joinedTangent[i] += vert1.tangent();

                for (unsigned int j : list)
                {
                    if (i == j)
                        continue; // skip comparing vertex with self

                    auto& vert2 = this->vertices[j];

                    Scalar angle = vert1.normal().angleBetween(vert2.normal());
                    if (angle <= thresholdAngleRads)
                    {
                        joinedNormal[i] += vert2.normal();
                        joinedTangent[i] += vert2.tangent();
                    }
                }
            }
            for (unsigned int index : list)
            {
                this->vertices[index].normal(joinedNormal[index]);
                this->vertices[index].tangent(joinedTangent[index]);
            }
        }

        // set calculated normals on vertices
        for (unsigned int i = 0; i < this->vertices.size(); i++)
        {
            vertices[i].normal(vertices[i].normal().normalize());
            vertices[i].tangent(vertices[i].tangent().normalize());
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

    inline void calculateTangents()
    {
        std::unordered_map<std::string, Vector3> map;

        std::stringstream ss;
        ss << std::setprecision(2);

        // calculate normals for unique positions
        for (unsigned int i = 0; i < this->vertices.size(); i += 3)
        {
            Vector2 points[] = {
                this->vertices[i].texCoord(),
                this->vertices[i + 1].texCoord(),
                this->vertices[i + 2].texCoord()
            };

            Vector4 pointsPos[] = {
                this->vertices[i].position(),
                this->vertices[i + 1].position(),
                this->vertices[i + 2].position()
            };

            Vector4 BA = pointsPos[1] - pointsPos[0];
            Vector4 CA = pointsPos[2] - pointsPos[0];

            Vector2 tBA = points[1] - points[0];
            Vector2 tCA = points[2] - points[0];
            Scalar area = (tBA.x() * tCA.y()) - (tBA.y() * tCA.x());

            Vector3 faceTangent;

            if (area != 0.0f)
            {
                Scalar delta = 1.0f / area;
                faceTangent = Vector3(
                    delta * ((BA.x() * tCA.y()) + (CA.x() * -tBA.y())),
                    delta * ((BA.y() * tCA.y()) + (CA.y() * -tBA.y())),
                    delta * ((BA.z() * tCA.y()) + (CA.z() * -tBA.y()))
                    );
            }

            for (Vector4 point : pointsPos)
            {
                ss.str("");
                ss << point.x() << "-" << point.y() << "-" << point.z() << "-" << point.w();
                map[ss.str()] = map[ss.str()] + faceTangent;
            }
        }

        // set calculated normals on vertices
        for (Vertex<AttrTypes...>& vertex : this->vertices)
        {
            Vector4 point = vertex.position();
            ss.str("");
            ss << point.x() << "-" << point.y() << "-" << point.z() << "-" << point.w();
            vertex.tangent(map[ss.str()].normalize());
        }

    }

    inline Vertex<AttrTypes...>& getVertex(int index)
    {
        return this->vertices[index];
    }

    private:
        template<typename... AttrTypeEnums>
        void collectTypes(std::set<GpuProgram::AttributeType>& types, 
            GpuProgram::AttributeType type,
            AttrTypeEnums... moreTypes)
        {
            types.insert(type);

            collectTypes(types, moreTypes...);
        }
        void collectTypes(std::set<GpuProgram::AttributeType>& types)
        {
            return; // on purpose
        }

        template<typename AttrType, typename... AttrTypes>
        void fillVertex(TriangleMesh& mesh, unsigned int vertexIndex, 
            AttrType& attr,
            AttrTypes... moreAttrs)
        {
            mesh.setAttributeData(vertexIndex, attr.type, attr.getData());

            fillVertex(mesh, vertexIndex, moreAttrs...);
        }
        void fillVertex(TriangleMesh& mesh, unsigned int vertexIndex)
        {
            return; // on purpose
        }

    public:
   
    std::shared_ptr<TriangleMesh> build()
    {
        std::set<GpuProgram::AttributeType> types;
        collectTypes(types, AttrTypes::type...);

        unsigned int faceCount = this->faces.size();
        if (faceCount == 0)
            faceCount = this->vertices.size() / 3;

        auto mesh = std::make_shared<TriangleMesh>(
            this->vertices.size(),
            faceCount,
            types
            );

        for (unsigned int i = 0; i < this->vertices.size(); i++)
        {
            auto& vertex = this->vertices[i];

            fillVertex(*mesh, i, (AttrTypes&)vertex...);
        }

        // generate face data if none was set
        // TODO: remove this when all generators changed
        if (this->faces.size() == 0)
        {
            for (unsigned int i = 0; i < faceCount; i++)
            {
                this->faces.push_back(TriangleMesh::Face(
                    i * 3,
                    i * 3 + 1,
                    i * 3 + 2
                    ));
            }
        }

        mesh->setFaceData(0, &this->faces[0], this->faces.size());

        return mesh;
    }

    /** Build a box mesh
    * @param width the width of the box
    * @param height the height of the box
    * @param depth the depth of the box
    */
    TriangleMeshBuilder<AttrTypes...>& buildBox(float width, float height, float depth);

    /** Build a 2D circle mesh
    * @param x the x coordinate of the center
    * @param y the y coordinate of the center
    * @param radius the radius of the circle
    * @param precisionAngle the angle between any two points on the edge of the
    circle, the lower angle, the better looking
    */
    static std::shared_ptr<TriangleMesh> build2DCircle(int x, int y, int radius, float precisionAngle);

    /** Build a flat surface
    * @param width the width of the surface
    * @param height the height of the surface
    * @param slices the number of squares on width
    * @param stacks the number of squares on height
    */
    static std::shared_ptr<TriangleMesh> buildFlatSurface(float width, float height, int slices,
        int stacks, bool texRepeat, float texPerX, float texPerY);

    /** Build 2D rectangle
    * @param x the upper-left x coordinate of the rectangle
    * @param y the upper-left y coordinate of the rectangle
    * @param width the width of the rectangle
    * @param height the height of the rectangle
    */
    static std::shared_ptr<TriangleMesh> build2DRectangle(int x, int y, int width, int height);

    /** Build sphere
    * @param radius the radius of the sphere
    * @param slices the number of squares on width
    * @param stacks the number of squares on height
    */
    TriangleMeshBuilder<AttrTypes...>& buildSphere(float radius, int slices, int stacks);

};

typedef TriangleMeshBuilder<PositionAttr, TexCoordAttr, NormalAttr> TriangleMeshBuilderPTN;
typedef TriangleMeshBuilder<PositionAttr, TexCoordAttr, NormalAttr, TangentAttr> TriangleMeshBuilderPTNT;
typedef TriangleMeshBuilder<PositionAttr, TexCoordAttr> TriangleMeshBuilderPT;


};








#endif