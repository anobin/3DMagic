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

#include "Mesh.h"

namespace Magic3D
{
    
class BatchBuilder;
    
/** Contains a collection of vertices on system memory, ready to be modified 
 * and/or copied to video memory as a mesh. Use the batchbuilder class to
 * build or modify a batch.
 */
class Batch
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
		Mesh::AttributeType type;
        
        inline AttributeData(): data(NULL), dataLen(0) {}
        inline ~AttributeData()
        {
            delete[] data;
        }
        inline void allocate(int vertexCount, Mesh::AttributeType type)
        {
            delete[] data;
            this->type = type;
            this->dataLen = vertexCount * Mesh::attributeTypeCompCount[(int)type] * sizeof(float);
            this->data = new float[ vertexCount * Mesh::attributeTypeCompCount[(int)type] ];
        }
    };
    
    /// list of attribute data
	AttributeData* data;
	
	/// number of verticies in mesh
	int vertexCount;
	
	/// number of attributes
	int attributeCount;
	
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
    inline Batch(): data(NULL), vertexCount(0), attributeCount(0) {}
    
    /// destructor
    virtual ~Batch();
    
    
    inline int getVertexCount()
    {
        return this->vertexCount;
    }
    
    
    
    
};   
    
    
    
    
    

    
};




#endif





