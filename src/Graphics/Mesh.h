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
/** Header file for Mesh class
 *
 * @file Mesh.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_MESH_H
#define MAGIC3D_MESH_H

#include "../Exceptions/MagicException.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include <Shaders/GpuProgram.h>
#include <CollisionShapes\SphereCollisionShape.h>
#include <Shapes\Vertex.h>

#include <vector>
#include <memory>
#include <string.h>

namespace Magic3D
{

template<typename... AttrTypes>
class MeshBuilder;

/** Contains the attribute data for a batch of vertices as well as properties,
 * such as texture, that are used to setup the shader to render the vertices.
 */
class Mesh
{
public:
	/// data for a attribute, ready to be bound to a shader
	struct AttributeData
	{
	    /// attribute data in a buffer on graphics memory
		Buffer buffer;
		/// auto-bound attribute type for data
		GpuProgram::AttributeType type;
		/// current data in batch
		float* data;
		/// current length of data (in bytes)
		int dataLen;

		inline AttributeData() : data(NULL), dataLen(0) {}

		inline ~AttributeData()
		{
			delete[] data;
		}
		inline void allocate(int vertexCount, GpuProgram::AttributeType type)
		{
			delete[] data;
			this->type = type;
			this->dataLen = vertexCount * GpuProgram::attributeTypeCompCount[(int)type] * sizeof(float);
			this->data = new float[vertexCount * GpuProgram::attributeTypeCompCount[(int)type]];
		}
	};

private:	
    template<typename... AttrTypes>
	friend class MeshBuilder;

	/// list of attribute data
	AttributeData* attributeData;
	
	/// number of verticies in mesh
	int vertexCount;
	
	/// number of attributes
	int attributeCount;

	VertexArray::Primitives primitive;

	VertexArray* vertexArray;

	inline void allocate(int vertexCount, int attributeCount)
	{
		// free any previous data in batch
		delete[] this->attributeData;

		// setup mesh
		this->vertexCount = vertexCount;
		this->attributeCount = attributeCount;
		this->attributeData = new AttributeData[attributeCount];
	}

	void copyBatchIn();

    std::shared_ptr<Mesh> visibleNormals;

    inline void allocateAttrs(int index)
    {
        return; // on purpose
    }

    template<typename... AttrTypeIds>
    inline void allocateAttrs(int index, GpuProgram::AttributeType id, AttrTypeIds... ids)
    {
        this->attributeData[index].allocate(this->vertexCount, id);

        allocateAttrs(++index, ids...);
    }

    inline void fillInAttr(int attributeIndex, int vertexIndex)
    {
        return; // on purpose
    }

    template<typename VectorType, typename... VectorTypes>
    inline void fillInAttr(int attributeIndex, int vertexIndex, VectorType vector,
        VectorTypes... vectors)
    {
        int compCount = GpuProgram::attributeTypeCompCount[
            (int)this->attributeData[attributeIndex].type
        ];
        memcpy(
            &this->attributeData[attributeIndex].data[compCount*vertexIndex],
            vector,
            sizeof(Scalar)*compCount
            );

        fillInAttr(++attributeIndex, vertexIndex, vectors...);
    }
	
public:
    /// Standard Constructor
	inline Mesh(): attributeData(nullptr), vertexCount(0), 
		attributeCount(0), vertexArray(nullptr), visibleNormals(nullptr) {}

    inline Mesh(const Mesh& mesh) :
        vertexCount(mesh.vertexCount),
        attributeCount(mesh.attributeCount),
        primitive(mesh.primitive),
        vertexArray(nullptr), attributeData(nullptr)
    {
        this->allocate(vertexCount, attributeCount);
        for (int i = 0; i < attributeCount; i++)
        {
            this->attributeData[i].allocate(vertexCount, mesh.attributeData[i].type);
            memcpy(this->attributeData[i].data, mesh.attributeData[i].data,
                mesh.attributeData[i].dataLen);
        }
    }

    template<typename... AttrTypes>
    inline Mesh(const std::vector<Vertex<AttrTypes...>>& vertices, VertexArray::Primitives primitive) : 
        vertexArray(nullptr), attributeData(nullptr), primitive(primitive)
    {
        this->allocate(vertices.size(), Vertex<AttrTypes...>::attributeCount);
        
        this->allocateAttrs(0, AttrTypes::type...);

        for (unsigned int i = 0; i < vertices.size(); i++)
        {
            this->fillInAttr(
                0,
                i,
                vertices[i].AttrTypes::getData()...
            );
        }
    }

	/// destructor
	~Mesh();
	
	inline int getAttributeCount() const
	{
	    return attributeCount;
	}
	
	inline int getVertexCount() const
	{
	    return vertexCount;
	}

    // TODO: change to work for all vertex types
    inline const VertexPTN getVertex(int index) const
    {
        if (this->attributeCount == 2)
        {
            return VertexPTN(
                PositionAttr(&this->attributeData[0].data[index * 4]),
                TexCoordAttr(&this->attributeData[1].data[index * 2]),
                NormalAttr()
            );
        }
        else
        {
            return VertexPTN(
                PositionAttr(&this->attributeData[0].data[index * 4]),
                TexCoordAttr(&this->attributeData[1].data[index * 2]),
                NormalAttr(&this->attributeData[2].data[index * 3])
                );
        }
    }

	inline VertexArray::Primitives getPrimitive() const
	{
		return this->primitive;
	}

    Mesh& getVisibleNormals();

	const VertexArray& getVertexArray();

    std::shared_ptr<Mesh> applyTransform(const Matrix4& matrix) const;
	
};


class Meshes : public Resource, public std::vector<std::shared_ptr<Mesh>>
{
private:
    std::shared_ptr<SphereCollisionShape> boundingSphere;

    std::shared_ptr<Mesh> boundingSphereMesh;

public:
	inline Meshes(): boundingSphere(nullptr), boundingSphereMesh(nullptr) {}

	inline Meshes(std::shared_ptr<Mesh> mesh)
	{
		this->push_back(mesh);
	}

    const SphereCollisionShape& getBoundingSphere();

    // used for debugging and developer tools
    Mesh& getBoundingSphereMesh();

    inline std::shared_ptr<Meshes> applyTransform(const Matrix4& matrix) const
    {
        auto meshes = std::make_shared<Meshes>();
        for (auto mesh : *this)
            meshes->push_back(mesh->applyTransform(matrix));
        return meshes;
    }
};


};




#endif
















