

#ifndef MAGIC3D_TRIANGLE_MESH_H
#define MAGIC3D_TRIANGLE_MESH_H

#include <map>
#include <vector>
#include <set>
#include <cmath>

#include "Math\Math.h"
#include "Shaders\GpuProgram.h"
#include <Shapes\Vertex.h>
#include <Shapes\Triangle.h>
#include <Geometry\Geometry.h>

namespace Magic3D
{

// TODO: fix the mutablility and out of sync flag to work correctly
class TriangleMesh : public Geometry
{
public:
    struct Face
    {
        unsigned int indices[3];

        inline Face() : Face(0, 0, 0) {}

        inline Face(unsigned int a, unsigned int b, unsigned int c)
        {
            indices[0] = a;
            indices[1] = b;
            indices[2] = c;
        }

        inline void set(unsigned int a, unsigned int b, unsigned int c)
        {
            indices[0] = a;
            indices[1] = b;
            indices[2] = c;
        }
    };

private:
    // attributes for vertices (on main memory)
    std::map<GpuProgram::AttributeType, std::vector<Scalar>> attributes;

    // attributes for vertices (on gpu memory)
    mutable std::map<GpuProgram::AttributeType, Buffer> gpuAttributes;
    // vertex array for vertices
    VertexArray vertexArray;

    std::vector<Face> faces;

    unsigned int vertexCount;

    mutable bool outOfSync;
    mutable std::shared_ptr<CollisionShape> collisionShape;

    inline void markDirty()
    {
        this->outOfSync = true;
        this->collisionShape = nullptr;
    }

public:
    inline TriangleMesh(unsigned int vertexCount, unsigned int faceCount,
        const std::set<GpuProgram::AttributeType>& attributeTypes):
        vertexCount(vertexCount), faces(faceCount), outOfSync(true)
    {
        // allocate all space needed for attribute data on main memory and gpu memory
        for (GpuProgram::AttributeType type : attributeTypes)
        {
            unsigned int componentCount = GpuProgram::attributeTypeCompCount[(int)type];
            unsigned int scalarCount = componentCount * vertexCount;
            unsigned int dataSize = scalarCount * sizeof(Scalar);

            // main memory
            std::vector<Scalar> list(
                scalarCount,
                0.0f // default to 0
            );
            attributes.insert(std::make_pair(
                type, std::move(list)
            ));

            // gpu memory
            Buffer buffer;
            buffer.allocate(
                dataSize,
                nullptr, // no data to start with
                Buffer::STATIC_DRAW
            );
            this->vertexArray.setAttributeArray(
                (int)type,
                componentCount,
                VertexArray::FLOAT, // TODO: make this dynamic with the type of Scalar
                buffer
            );
            this->gpuAttributes.insert(std::make_pair(
                type, std::move(buffer)
            ));
        }
    }

    inline TriangleMesh(const TriangleMesh& mesh) :
        attributes(mesh.attributes), faces(mesh.faces), vertexCount(mesh.vertexCount),
        outOfSync(true) 
    {
        for (auto it : this->attributes)
        {
            unsigned int componentCount = GpuProgram::attributeTypeCompCount[(int)it.first];
            unsigned int scalarCount = componentCount * vertexCount;
            unsigned int dataSize = scalarCount * sizeof(Scalar);

            // gpu memory
            Buffer buffer;
            buffer.allocate(
                dataSize,
                nullptr, // no data to start with
                Buffer::STATIC_DRAW
                );
            this->vertexArray.setAttributeArray(
                (int)it.first,
                componentCount,
                VertexArray::FLOAT, // TODO: make this dynamic with the type of Scalar
                buffer
                );
            this->gpuAttributes.insert(std::make_pair(
                it.first, std::move(buffer)
                ));
        }
    }

    inline unsigned int getVertexCount() const
    {
        return this->vertexCount;
    }

    inline void setAttributeData(unsigned int vertexIndex,
        GpuProgram::AttributeType type, const Scalar* data, unsigned int vertexCount = 1)
    {
        if ((vertexIndex + vertexCount) > this->vertexCount)
            throw_MagicException("out of bounds");

        Scalar* hereData = &this->attributes.find(type)->second[
            vertexIndex*GpuProgram::attributeTypeCompCount[(int)type]];

        memcpy(hereData, data, 
            GpuProgram::attributeTypeCompCount[(int)type] * sizeof(Scalar) * vertexCount);

        markDirty();
    }

    inline const Scalar* getAttributeData(unsigned int vertexIndex,
        GpuProgram::AttributeType type) const
    {
        if (vertexIndex >= this->vertexCount)
            throw_MagicException("out of bounds");

        return &this->attributes.find(type)->second[
            vertexIndex*GpuProgram::attributeTypeCompCount[(int)type]];
    }

    inline void setFaceData(unsigned int faceIndex, const Face* data, unsigned int count)
    {
        if ((faceIndex + count) > this->faces.size())
            throw_MagicException("out of bounds");

        memcpy(
            &this->faces[faceIndex],
            data,
            sizeof(Face) * count
        );

        markDirty();
    }

    inline const Face* getFaceData(unsigned int faceIndex) const
    {
        if (faceIndex >= this->faces.size())
            throw_MagicException("out of bounds");

        return &this->faces[faceIndex];
    }

    inline const Face& getFace(unsigned int index) const
    {
        return *this->getFaceData(index);
    }

    inline void setFace(unsigned int index, const Face& face)
    {
        this->setFaceData(index, &face, 1);
    }

    inline unsigned int getFaceCount() const
    {
        return this->faces.size();
    }

    inline const VertexArray& getVertexArray() const
    {
        // copy new data to gpu memory if needed
        if (outOfSync)
        {
            auto mainIt = this->attributes.begin();
            for (; mainIt != this->attributes.end(); mainIt++)
            {
                auto gpuIt = this->gpuAttributes.find(mainIt->first);
                gpuIt->second.fill(
                    0,
                    vertexCount * GpuProgram::attributeTypeCompCount[(int)mainIt->first] * sizeof(Scalar),
                    &mainIt->second[0]
                );
            }
            this->outOfSync = false;
        }

        return this->vertexArray;
    }

    inline bool hasType(GpuProgram::AttributeType type) const
    {
        return this->attributes.find(type) != this->attributes.end();
    }

    // TODO: stop mis-using the triangle mesh to store a lines mesh
    mutable std::shared_ptr<TriangleMesh> normalsMesh;
    mutable Scalar normalsLength;
    inline TriangleMesh& getNormalsMesh(Scalar length) const
    {
        if (normalsMesh != nullptr && this->normalsLength == length)
            return *normalsMesh;

        std::set<GpuProgram::AttributeType> types;
        types.insert(GpuProgram::AttributeType::VERTEX);
        normalsMesh = std::make_shared<TriangleMesh>(this->vertexCount*2, 0, types);

        for (unsigned int i = 0; i < this->vertexCount; i++)
        {
            Vector4 pos(this->getAttributeData(i, GpuProgram::AttributeType::VERTEX));
            Vector3 normal(this->getAttributeData(i, GpuProgram::AttributeType::NORMAL));

            Vector4 endPoint = pos + (normal.normalize() * length);
            endPoint.w(1.0f);

            normalsMesh->setAttributeData(
                i*2, 
                GpuProgram::AttributeType::VERTEX,
                pos.getData()
            );
            normalsMesh->setAttributeData(
                i*2+1, 
                GpuProgram::AttributeType::VERTEX,
                endPoint.getData()
            );
        }

        this->normalsLength = length;
        return *normalsMesh;
    }

    template<typename... AttrTypes>
    Vertex<AttrTypes...> getVertex(unsigned int index) const
    {
        return Vertex<AttrTypes...>(this->getAttributeData(index, AttrTypes::type)...);
    }

    template<typename... AttrTypes>
    void setVertex(unsigned int index, const Vertex<AttrTypes...>& vertex)
    {
        int _[] = { 0, (

            this->setAttributeData(index, AttrTypes::type, ((AttrTypes&)vertex).getData())

        , 0)... };
        (void)_;
    }

    inline void calculateNormalsAndTangents()
    {
        // clear any existing normal and tangent data
        for (unsigned int i = 0; i < this->vertexCount; i++)
        {
            auto vert = this->getVertex<NormalAttr, TangentAttr>(i);
            vert.normal(0,0,0);
            vert.tangent(0,0,0);
            this->setVertex(i, vert);
        }

        // calculate normals for unique positions
        for (TriangleMesh::Face& face : this->faces)
        {
            auto a = this->getVertex<PositionAttr, TexCoordAttr, NormalAttr, TangentAttr>(face.indices[0]);
            auto b = this->getVertex<PositionAttr, TexCoordAttr, NormalAttr, TangentAttr>(face.indices[1]);
            auto c = this->getVertex<PositionAttr, TexCoordAttr, NormalAttr, TangentAttr>(face.indices[2]);

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

            this->setVertex(face.indices[0], a);
            this->setVertex(face.indices[1], b);
            this->setVertex(face.indices[2], c);
        }

        // set calculated normals on vertices
        for (unsigned int i = 0; i < this->vertexCount; i++)
        {
            auto vert = this->getVertex<NormalAttr, TangentAttr>(i);
            vert.normal(vert.normal().normalize());
            vert.tangent(vert.tangent().normalize());
            this->setVertex(i, vert);
        }
    }

    inline std::vector<std::vector<unsigned int>> calculateDuplicateVertices(unsigned int precision = 3)
    {
        Scalar threshold = Scalar(std::pow(10, -int(precision)));
        Scalar inverseThreshold = Scalar(std::pow(10, precision));

        std::unordered_map<
            Vector4,
            std::vector<unsigned int>,
            std::function<int(const Vector4&)>,
            std::function<bool(const Vector4&, const Vector4&)>
        > map(
        this->vertexCount,
        [&](const Vector4& vec) -> int
        {
            return
                std::hash<int>()(int(vec.x() * inverseThreshold)) ^
                std::hash<int>()(int(vec.y() * inverseThreshold)) ^
                std::hash<int>()(int(vec.z() * inverseThreshold)) ^
                std::hash<int>()(int(vec.w() * inverseThreshold));
        },
        [&](const Vector4& a, const Vector4& b) -> bool
        {
            bool equal = 
                std::abs(a.x() - b.x()) < threshold &&
                std::abs(a.y() - b.y()) < threshold &&
                std::abs(a.z() - b.z()) < threshold &&
                std::abs(a.w() - b.w()) < threshold;
            return equal;
        }
        );

        for (unsigned int i = 0; i < this->vertexCount; i++)
        {
            auto vert = this->getVertex<PositionAttr>(i);
            map[vert.position()].push_back(i);
        }

        std::vector<std::vector<unsigned int>> duplicateVertexIndices;
        for (auto it : map)
        {
            if (it.second.size() > 1)
            {
                duplicateVertexIndices.push_back(std::move(it.second));
            }
        }

        return std::move(duplicateVertexIndices);
    }

    inline void mergeNormalsAndTangents(Scalar thresholdAngle = 60.0f, unsigned int precision = 3)
    {
        // merge normals for different points that are at the same location,
        // if the angle between them is smaller than the threshold angle
        Scalar thresholdAngleRads = thresholdAngle * Scalar(M_PI / 180);
        std::vector<std::vector<unsigned int>> duplicateVertexIndices =
            calculateDuplicateVertices(precision);
        for (auto& list : duplicateVertexIndices)
        {
            std::map<unsigned int, Vector3> joinedNormal;
            std::map<unsigned int, Vector3> joinedTangent;
            for (unsigned int i : list)
            {
                auto vert1 = this->getVertex<NormalAttr, TangentAttr>(i);
                joinedNormal[i] += vert1.normal();
                joinedTangent[i] += vert1.tangent();

                for (unsigned int j : list)
                {
                    if (i == j)
                        continue; // skip comparing vertex with self

                    auto& vert2 = this->getVertex<NormalAttr, TangentAttr>(j);

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
                auto vert = this->getVertex<NormalAttr, TangentAttr>(index);
                vert.normal(joinedNormal[index].normalize());
                vert.tangent(joinedTangent[index].normalize());
                this->setVertex(index, vert);
            }
        }
    }

    virtual void positionTransform(const Matrix4& matrix);

    virtual const CollisionShape& getCollisionShape() const;

    virtual const TriangleMesh& getTriangleMesh() const;

};


};




#endif