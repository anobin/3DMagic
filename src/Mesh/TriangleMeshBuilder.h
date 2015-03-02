
#ifndef MAGIC3D_TRIANGLE_MESH_BUILDER_H
#define MAGIC3D_TRIANGLE_MESH_BUILDER_H


#include <sstream>
#include <iomanip>

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

    inline TriangleMeshBuilder<AttrTypes...>& positionTransform(const Matrix4& matrix)
    {
        for (Vertex<AttrTypes...>& vertex : this->vertices)
        {
            vertex.position(Vector3(vertex.position().getData()).transform(matrix));
        }

        return *this;
    }

    inline void calaculateNormalsFromFaces()
    {
        std::unordered_map<unsigned int, Vector3> map;

        // calculate normals for unique positions
        for (TriangleMesh::Face& face : this->faces)
        {
            Vector4 points[] = {
                this->vertices[face.indices[0]].position(),
                this->vertices[face.indices[1]].position(),
                this->vertices[face.indices[2]].position()
            };
            Vector3 faceNormal = Triangle(points[0], points[1], points[2]).normal;

            map[face.indices[0]] = map[face.indices[0]] + faceNormal;
            map[face.indices[1]] = map[face.indices[1]] + faceNormal;
            map[face.indices[2]] = map[face.indices[2]] + faceNormal;
        }

        // set calculated normals on vertices
        for (unsigned int i = 0; i < this->vertices.size(); i++)
        {
            vertices[i].normal(map[i].normalize());
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

    inline void calculateTangentsFromFaces()
    {
        std::unordered_map<unsigned int, Vector3> map;

        // calculate normals for unique positions
        for (TriangleMesh::Face& face : this->faces)
        {
            Vector2 points[] = {
                this->vertices[face.indices[0]].texCoord(),
                this->vertices[face.indices[1]].texCoord(),
                this->vertices[face.indices[2]].texCoord()
            };

            Vector4 pointsPos[] = {
                this->vertices[face.indices[0]].position(),
                this->vertices[face.indices[1]].position(),
                this->vertices[face.indices[2]].position()
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

            map[face.indices[0]] = map[face.indices[0]] + faceTangent;
            map[face.indices[1]] = map[face.indices[1]] + faceTangent;
            map[face.indices[2]] = map[face.indices[2]] + faceTangent;
        }

        // set calculated normals on vertices
        for (unsigned int i = 0; i < this->vertices.size(); i++)
        {
            vertices[i].tangent(map[i].normalize());
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