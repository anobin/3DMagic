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

#include <vector>
#include <memory>
#include <string.h>

namespace Magic3D
{
    
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
	
public:
    /// Standard Constructor
	inline Mesh(): attributeData(nullptr), vertexCount(0), 
		attributeCount(0), vertexArray(nullptr) {}

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

	inline VertexArray::Primitives getPrimitive() const
	{
		return this->primitive;
	}

	const VertexArray& getVertexArray();

    void applyTransform(const Matrix4& matrix);
	
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

    inline void applyTransform(const Matrix4& matrix)
    {
        for (auto mesh : *this)
            mesh->applyTransform(matrix);
    }
};


};




#endif
















