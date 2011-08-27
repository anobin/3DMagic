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
#include "Batch.h"
#include "VertexArray.h"
#include "Texture.h"

#include <vector>
#include <string.h>

namespace Magic3D
{
    
class Mesh;

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
		const Buffer& buffer;
		/// auto-bound attribute type for data
		const Batch::AttributeType& type;
	
	protected:
	    friend class Mesh;
	    
	    Buffer _buffer;
	    Batch::AttributeType _type;
		inline AttributeData(): buffer(_buffer), type(_type) {}
	};

private:	  
	/// list of attribute data
	AttributeData* attributeData;
	
	/// number of verticies in mesh
	int vertexCount;
	
	/// number of attributes
	int attributeCount;
	
	/// (Re)allocate mesh data
	inline void allocate(int vertexCount, int attributeCount)
	{
	    // free any previous data in mesh
	    delete[] this->attributeData;
	
	    // setup mesh
	    this->vertexCount = vertexCount;
	    this->attributeCount = attributeCount;
	    this->attributeData = new AttributeData[attributeCount];
	}
	
public:
    /// Standard Constructor
	inline Mesh(): attributeData(NULL), vertexCount(0) , attributeCount(0) {}
    
	/// destructor
	~Mesh();

	inline const AttributeData* getAttributeData() const
	{
	     return attributeData;   
	}
	
	inline int getAttributeCount() const
	{
	    return attributeCount;
	}
	
	inline int getVertexCount() const
	{
	    return vertexCount;
	}
	
	void copyBatchIn(const Batch& batch);
	
};


};




#endif
















