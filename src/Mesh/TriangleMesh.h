

#ifndef MAGIC3D_TRIANGLE_MESH_H
#define MAGIC3D_TRIANGLE_MESH_H

#include <map>
#include <vector>
#include <set>
#include "Math\Math.h"
#include "Shaders\GpuProgram.h"
#include <Shapes\Vertex.h>
#include <Shapes\Triangle.h>

namespace Magic3D
{

class TriangleMesh
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
    std::map<GpuProgram::AttributeType, Buffer> gpuAttributes;
    // vertex array for vertices
    VertexArray vertexArray;

    std::vector<Face> faces;

    unsigned int vertexCount;
    bool outOfSync;

public:
    TriangleMesh(unsigned int vertexCount, unsigned int faceCount,
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

    inline unsigned int getVertexCount()
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
        this->outOfSync = true;
    }

    inline Scalar* getAttributeData(unsigned int vertexIndex,
        GpuProgram::AttributeType type)
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
    }

    inline Face* getFaceData(unsigned int faceIndex)
    {
        if (faceIndex >= this->faces.size())
            throw_MagicException("out of bounds");

        return &this->faces[faceIndex];
    }

    inline Face& getFace(unsigned int index)
    {
        return *this->getFaceData(index);
    }

    inline unsigned int getFaceCount()
    {
        return this->faces.size();
    }

    inline const VertexArray& getVertexArray()
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

    inline bool hasType(GpuProgram::AttributeType type)
    {
        return this->attributes.find(type) != this->attributes.end();
    }

    // TODO: stop mis-using the triangle mesh to store a lines mesh
    std::shared_ptr<TriangleMesh> normalsMesh;
    Scalar normalsLength;
    inline TriangleMesh& getNormalsMesh(Scalar length)
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
    Vertex<AttrTypes...> getVertex(unsigned int index)
    {
        return Vertex<AttrTypes...>(this->getAttributeData(index, AttrTypes::type)...);
    }

    inline void positionTransform(const Matrix4& matrix)
    {
        for (unsigned int i = 0; i < this->vertexCount; i++)
        {
            auto vert = this->getVertex<PositionAttr>(i);
            vert.position(vert.position().transform(matrix));
        }
    }

    inline void calculateNormalsAndTangents()
    {
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
        }

        // set calculated normals on vertices
        for (unsigned int i = 0; i < this->vertexCount; i++)
        {
            auto vert = this->getVertex<NormalAttr, TangentAttr>(i);
            vert.normal(vert.normal().normalize());
            vert.tangent(vert.tangent().normalize());
        }
    }

};


};




#endif