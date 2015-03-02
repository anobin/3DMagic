

#ifndef MAGIC3D_TRIANGLE_MESH_H
#define MAGIC3D_TRIANGLE_MESH_H

#include <map>
#include <vector>
#include <set>
#include "Math\Math.h"
#include "Shaders\GpuProgram.h"

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

    inline const Scalar* getAttributeData(unsigned int vertexIndex,
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

    inline const Face* getFaceData(unsigned int faceIndex)
    {
        if (faceIndex >= this->faces.size())
            throw_MagicException("out of bounds");

        return &this->faces[faceIndex];
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

};


};




#endif