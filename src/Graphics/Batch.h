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
/** Header file for Batch class
 *
 * @file Batch.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_BATCH_H
#define MAGIC3D_BATCH_H

#include <stdlib.h>
#include <Resources/Resource.h>
#include <Shaders\GpuProgram.h>

#include <vector>
#include <memory>

#include <Math/Matrix4.h>

namespace Magic3D
{
    
class BatchBuilder;
    
/** Contains a collection of vertices on system memory, ready to be modified 
 * and/or copied to video memory as a mesh. Use the batchbuilder class to
 * build or modify a batch.
 */
class Batch : public Resource
{
protected:
    friend class BatchBuilder;
    friend class Mesh;
    
    /// data for attribute on system memory
    struct AttributeData
    {
        /// current data in batch
        float* data;
        /// current length of data (in bytes)
        int dataLen;
        /// auto-bound attribute type for data
		GpuProgram::AttributeType type;
        
        inline AttributeData(): data(NULL), dataLen(0) {}
        inline ~AttributeData()
        {
            delete[] data;
        }
        inline void allocate(int vertexCount, GpuProgram::AttributeType type)
        {
            delete[] data;
            this->type = type;
            this->dataLen = vertexCount * GpuProgram::attributeTypeCompCount[(int)type] * sizeof(float);
            this->data = new float[ vertexCount * GpuProgram::attributeTypeCompCount[(int)type] ];
        }
    };
    
    /// list of attribute data
	AttributeData* data;
	
	/// number of verticies in mesh
	int vertexCount;
	
	/// number of attributes
	int attributeCount;

	Mesh* mesh;
	
	/// (Re)allocate batch data
	inline void allocate(int vertexCount, int attributeCount)
	{
	    // free any previous data in batch
	    delete[] this->data;
	
	    // setup mesh
	    this->vertexCount = vertexCount;
	    this->attributeCount = attributeCount;
	    this->data = new AttributeData[attributeCount];
	}
    
public:
    /// standard constructor
    inline Batch(): data(NULL), vertexCount(0), attributeCount(0), mesh(nullptr) {}
    
    /// destructor
    virtual ~Batch();
    
    
    inline int getVertexCount()
    {
        return this->vertexCount;
    }

	void applyTransform(const Matrix4& matrix);
};   
    
    
    
class Batches : public Resource, public std::vector<std::shared_ptr<Batch>>
{
public:
	inline Batches() {}

	Batches(std::shared_ptr<Batch> batch)
	{
		this->push_back(batch);
	}

	inline void applyTransform(const Matrix4& matrix)
	{
		for(auto it : *this)
			it->applyTransform(matrix);
	}
};


    
};




#endif





