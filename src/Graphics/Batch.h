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

#include <vector>
#include <memory>

namespace Magic3D
{
    
class BatchBuilder;
    
/** Contains a collection of vertices on system memory, ready to be modified 
 * and/or copied to video memory as a mesh. Use the batchbuilder class to
 * build or modify a batch.
 */
class Batch : public Resource
{
public:
    /** different auto-bound attributes types that can be present in mesh data.
     * The shader to render this mesh data does not have to support the exact
     * set of attribute types that are in this mesh, but an attempt will be made
     * to bound the attribute types that are present to the shader with the 
     * specified names and component numbers (type is always float). 
     */
    enum AttributeType
    {
        VERTEX = 0,     // vec4 "vertex"
        NORMAL,         // vec3 "normal"
        COLOR,          // vec4 "color"
        COLOR2,         // vec4 "color2"
        TEX_COORD_0,    // vec2 "texcoord0"
        TEX_COORD_1,    // vec2 "texcoord1"
        TEX_COORD_2,    // vec2 "texcoord2"
        TEX_COORD_3,    // vec2 "texcoord3"
        TEX_COORD_4,    // vec2 "texcoord4"
        TEX_COORD_5,    // vec2 "texcoord5"
        TEX_COORD_6,    // vec2 "texcoord6"
        TEX_COORD_7,    // vec2 "texcoord7"
        TANGENT,        // vec3 "tangent"
        BINORMAL,       // vec3 "binormal"
        MAX_ATTRIBUTE_TYPES
    };
    
    /// names of shader variables for each of the auto-bound attribute types
    static const char* attributeTypeNames[ MAX_ATTRIBUTE_TYPES ];
    
    /// number of components for shader variables for each of the auto-bound attribute types
    static const int attributeTypeCompCount[ MAX_ATTRIBUTE_TYPES ]; 
    
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
		AttributeType type;
        
        inline AttributeData(): data(NULL), dataLen(0) {}
        inline ~AttributeData()
        {
            delete[] data;
        }
        inline void allocate(int vertexCount, AttributeType type)
        {
            delete[] data;
            this->type = type;
            this->dataLen = vertexCount * attributeTypeCompCount[(int)type] * sizeof(float);
            this->data = new float[ vertexCount * attributeTypeCompCount[(int)type] ];
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
    
    Mesh* getMesh();
    
};   
    
    
    
class Batches : public Resource, public std::vector<std::shared_ptr<Batch>>
{

};
    

    
};




#endif





